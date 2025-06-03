#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

void inverter_cores(const char *arquivo_entrada, const char *arquivo_saida) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *infile = fopen(arquivo_entrada, "rb");
    if (!infile) {
        fprintf(stderr, "Erro ao abrir arquivo de entrada %s\n", arquivo_entrada);
        exit(1);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int largura = cinfo.output_width;
    int altura = cinfo.output_height;
    int canais = cinfo.output_components;

    unsigned char *linha = (unsigned char *)malloc(largura * canais);

    // Cria imagem de saída
    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr_out;

    FILE *outfile = fopen(arquivo_saida, "wb");
    if (!outfile) {
        fprintf(stderr, "Erro ao criar arquivo de saída %s\n", arquivo_saida);
        fclose(infile);
        free(linha);
        exit(1);
    }

    cinfo_out.err = jpeg_std_error(&jerr_out);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = largura;
    cinfo_out.image_height = altura;
    cinfo_out.input_components = canais;
    cinfo_out.in_color_space = cinfo.out_color_space;

    jpeg_set_defaults(&cinfo_out);
    jpeg_start_compress(&cinfo_out, TRUE);

    while (cinfo.output_scanline < altura) {
        jpeg_read_scanlines(&cinfo, &linha, 1);

        for (int i = 0; i < largura * canais; i++) {
            linha[i] = 255 - linha[i]; // Inversão da cor
        }

        jpeg_write_scanlines(&cinfo_out, &linha, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    fclose(outfile);

    free(linha);

    printf("Inversão de cores concluída com sucesso: %s -> %s\n", arquivo_entrada, arquivo_saida);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <imagem_entrada.jpg> <imagem_saida.jpg>\n", argv[0]);
        return 1;
    }

    inverter_cores(argv[1], argv[2]);

    return 0;
}
