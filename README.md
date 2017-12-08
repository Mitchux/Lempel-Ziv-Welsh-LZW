# Lempel-Ziv-Welsh-LZW-
Implementação de um compressor e um descompressor aplicado a texto em geral.

Projeto de desenvolvimento de um compressor (e respectivo descompressor) de texto usando o
algoritmo Lempel-Ziv-Welsh (LZW) com tamanho de dicionário fixo.

O compressor opera por linha de comando, aceitando como parâmetro o nome e caminho do
arquivo TXT original, e também o nome e caminho do arquivo a ser gerado. O descompressor,
por sua vez, também será operado por linha de comando, aceitando ambos os parâmetros também
(arquivo binário de entrada e arquivo TXT de saída). O seguinte formato de execução deverá ser
obedecido:
Para compressão:

encode -i arquivo_original.txt -o arquivo_binario.bin

Para descompressão:

decode -i arquivo_binario.bin -o arquivo_descomprimido.txt
