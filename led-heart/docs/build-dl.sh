#!/usr/bin/env bash

make html latex

cd _build/latex
pdflatex LEDHeart.tex
cd ../..

for i in _build/html/_images/*; do
  mogrify -resize 384x $i
done
