#!/bin/bash
for i in 1 2 4 8; do
	echo "Testando tempos $i threads"
	for j in 1 2 3 4 5 6 7 8 9 10; do
		echo "Teste $j"
		./multiplicacao_de_matrizes 15000 15000 $i
		echo ""
	done
	echo ""
done 
