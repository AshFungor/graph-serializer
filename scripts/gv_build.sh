#!/bin/sh

printf "Building pdf file from $1.gv\n"

path="data/proposal/"
file_path="$path$1"
dot -Tpdf "$file_path.gv" -o "$file_path.pdf"