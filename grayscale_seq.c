#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

void aplicar_grayscale(const char* entrada, const char* saida) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr;

    FILE* infile = fopen(entrada, "rb");
    if (!infile) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", entrada);
        exit(1);
    }

    FILE* outfile = fopen(saida, "wb");
    if (!outfile) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída: %s\n", saida);
        fclose(infile);
        exit(1);
    }

    JSAMPARRAY buffer;
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = cinfo.output_width;
    cinfo_out.image_height = cinfo.output_height;
    cinfo_out.input_components = 1; // grayscale
    cinfo_out.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo_out);
    jpeg_start_compress(&cinfo_out, TRUE);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        // Alocar buffer para grayscale
        JSAMPROW gray_row = (JSAMPROW)malloc(cinfo.output_width);

        for (unsigned int i = 0, j = 0; i < row_stride; i += 3, j++) {
            unsigned char r = buffer[0][i];
            unsigned char g = buffer[0][i+1];
            unsigned char b = buffer[0][i+2];
            unsigned char gray = 0.3*r + 0.59*g + 0.11*b;
            gray_row[j] = gray;
        }

        jpeg_write_scanlines(&cinfo_out, &gray_row, 1);
        free(gray_row);
    }

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);
    fclose(outfile);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <imagem_entrada.jpg> <imagem_saida.jpg>\n", argv[0]);
        return 1;
    }

    aplicar_grayscale(argv[1], argv[2]);
    printf("Imagem convertida com sucesso: %s -> %s\n", argv[1], argv[2]);
    return 0;
}
