import subprocess
import time
from statistics import mean

# Arquivos .exe e filtros
filtros = {
    "grayscale": {
        "seq": "grayscale_seq.exe",
        "par": "grayscale_par.exe",
        "saida": "grayscale_resultados.txt"
    },
    "inversor": {
        "seq": "inversor_seq.exe",
        "par": "inversor_par.exe",
        "saida": "inversor_resultados.txt"
    }
}

# Imagens para teste
imagens = [f"{i}k.jpg" for i in range(1, 9)]

# Número de execuções por imagem
REPETICOES = 500

# Função para medir tempo médio de execução
def medir_tempo(executavel, imagem, saida_img):
    tempos = []
    for _ in range(REPETICOES):
        inicio = time.time()
        subprocess.run([executavel, imagem, saida_img], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        fim = time.time()
        tempos.append(fim - inicio)
    return mean(tempos)

# Executar testes
for filtro_nome, dados in filtros.items():
    resultados = []

    print(f"Testando filtro: {filtro_nome}")

    for imagem in imagens:
        print(f"  Imagem: {imagem}...")

        saida_seq = f"saida_seq_{filtro_nome}_{imagem}"
        saida_par = f"saida_par_{filtro_nome}_{imagem}"

        tempo_seq = medir_tempo(dados["seq"], imagem, saida_seq)
        tempo_par = medir_tempo(dados["par"], imagem, saida_par)

        linha = f"{imagem}: SEQ = {tempo_seq:.6f}s | PAR = {tempo_par:.6f}s"
        print("    " + linha)
        resultados.append(linha)

    # Escreve os resultados em arquivo
    with open(dados["saida"], "w") as f:
        f.write(f"Resultados para filtro: {filtro_nome.upper()}\n")
        f.write("\n".join(resultados))

print("✅ Testes finalizados.")
