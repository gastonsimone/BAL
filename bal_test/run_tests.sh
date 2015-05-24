#!/bin/bash

export PATH=.:$PATH

make

for x in `ls *_test`
do
	echo "Ejecutando $x..."
	$x >./$x.out 2>$x.error
done

echo "Los siguientes tests produjeron salida en la standard error:"
find ./ -size 1 -name "*.error" -print
echo "Ejecucion de BAL tests finalizada"

exit 0

