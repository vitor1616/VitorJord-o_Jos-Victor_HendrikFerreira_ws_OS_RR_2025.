#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct {
    JSAMPARRAY image_buffer;
    int start_line;
    int num_lines;
    int width;
} ThreadData;

void *invert_colors(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = 0; i < data->num_lines; ++i) {
        JSAMPROW row = data->image_buffer[data->start_line + i];
        for (int j = 0; j < data->width * 3; ++j) {
            row[j] = 255 - row[j];
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <entrada.jpg> <saida.jpg>\n", argv[0]);
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE *outfile = fopen(argv[2], "wb");
    if (!outfile) {
        perror("Erro ao abrir arquivo de saída");
        fclose(infile);
        return 1;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int row_stride = width * cinfo.output_components;

    JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * height);
    for (int i = 0; i < height; i++) {
        buffer[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);
    }

    for (int i = 0; i < height; i++) {
        jpeg_read_scanlines(&cinfo, &buffer[i], 1);
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int lines_per_thread = height / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].image_buffer = buffer;
        thread_data[t].start_line = t * lines_per_thread;
        thread_data[t].num_lines = (t == NUM_THREADS - 1)
            ? height - thread_data[t].start_line
            : lines_per_thread;
        thread_data[t].width = width;
        pthread_create(&threads[t], NULL, invert_colors, &thread_data[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr_out;
    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = width;
    cinfo_out.image_height = height;
    cinfo_out.input_components = 3;
    cinfo_out.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo_out);
    jpeg_start_compress(&cinfo_out, TRUE);

    for (int i = 0; i < height; i++) {
        jpeg_write_scanlines(&cinfo_out, &buffer[i], 1);
        free(buffer[i]);
    }

    free(buffer);
    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    fclose(outfile);

    return 0;
}
