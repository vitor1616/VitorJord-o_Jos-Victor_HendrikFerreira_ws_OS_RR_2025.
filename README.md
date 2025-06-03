# 🖼️ Processamento Paralelo de Imagens com Multithreading

Este repositório contém implementações de dois filtros de imagem (grayscale e inversor de cores), desenvolvidos em C tanto em versões sequenciais quanto paralelas, além de scripts para benchmark e visualização dos resultados.

---

## 📁 Estrutura do Repositório

```
.
├── Seminário_SO.pdf          # Orientação deste trabalho
├── grayscale_seq.c           # Filtro grayscale sequencial
├── grayscale_par.c           # Filtro grayscale paralelo
├── inversor_seq.c            # Filtro inversor sequencial
├── inversor_par.c            # Filtro inversor paralelo
├── benchmark.py              # Script de benchmark automatizado
├── plot_resultado.py         # Gera gráficos com os resultados
├── grayscale_resultados.txt  # Resultados do filtro grayscale
├── inversor_resultados.txt   # Resultados do filtro inversor
├── 1k.jpg                    # Imagens de entrada
├── 2k.jpg
├── ...
├── 8k.jpg
└── README.md                 # Este arquivo
```

---

## ⚙️ Pré-requisitos

- Compilador C (`gcc`)
- Biblioteca libjpeg (`libjpeg-dev`)
- Python 3
- Pacotes Python:
  ```bash
  pip install matplotlib
  ```

---

## 🛠️ Compilação

Compile os programas C usando o `gcc`:

```bash
gcc grayscale_seq.c -o grayscale_seq.exe -ljpeg
gcc grayscale_par.c -o grayscale_par.exe -ljpeg
gcc inversor_seq.c -o inversor_seq.exe -ljpeg
gcc inversor_par.c -o inversor_par.exe -ljpeg
```


---

## 🚀 Execução dos Benchmarks

Execute o script para medir os tempos médios de execução:

```bash
python benchmark.py
```

Os resultados serão salvos em:
- `grayscale_resultados.txt`
- `inversor_resultados.txt`

---

## 📊 Visualização dos Resultados

Para gerar e visualizar os gráficos:

```bash
python plot_resultado.py
```

Isso irá gerar:
- `grafico_grayscale.png`
- `grafico_inversor.png`

---

## 🖼️ Conjunto de Imagens

O conjunto de teste está composto por 8 imagens:
- `1k.jpg`, `2k.jpg`, ..., `8k.jpg`
- Todas localizadas na pasta raiz.

---

## 📌 Objetivo

Avaliar e comparar o desempenho de filtros de imagem aplicados a arquivos JPEG nas versões **sequencial** e **paralela**.
