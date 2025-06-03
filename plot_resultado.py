import matplotlib.pyplot as plt

def ler_resultados(nome_arquivo):
    imagens = []
    tempos_seq = []
    tempos_par = []

    with open(nome_arquivo, 'r') as f:
        linhas = f.readlines()[1:]  # pula o cabeçalho
        for linha in linhas:
            partes = linha.strip().split()
            imagem = partes[0][:-1]  # remove ":"
            tempo_seq = float(partes[3][:-1])  # remove "s"
            tempo_par = float(partes[7][:-1])  # remove "s"
            imagens.append(imagem)
            tempos_seq.append(tempo_seq)
            tempos_par.append(tempo_par)

    return imagens, tempos_seq, tempos_par

# Lê os arquivos de resultados
imagens_gs, seq_gs, par_gs = ler_resultados("grayscale_resultados.txt")
imagens_inv, seq_inv, par_inv = ler_resultados("inversor_resultados.txt")

# Gráfico para grayscale
plt.figure(figsize=(7, 5))
plt.plot(imagens_gs, seq_gs, marker='o', label='Sequencial')
plt.plot(imagens_gs, par_gs, marker='s', label='Paralelo')
plt.title("Desempenho do Filtro Grayscale")
plt.xlabel("Imagem")
plt.ylabel("Tempo médio (s)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_grayscale.png")
plt.show()

# Gráfico para inversor
plt.figure(figsize=(7, 5))
plt.plot(imagens_inv, seq_inv, marker='o', label='Sequencial')
plt.plot(imagens_inv, par_inv, marker='s', label='Paralelo')
plt.title("Desempenho do Filtro Inversor")
plt.xlabel("Imagem")
plt.ylabel("Tempo médio (s)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("grafico_inversor.png")
plt.show()
