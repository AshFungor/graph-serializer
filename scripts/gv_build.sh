#!/bin/sh

path="data/proposal/"
printf 'Building...\n'
file_path="$path$1"
dot -Tpdf "$file_path.gv" -o "$file_path.pdf"