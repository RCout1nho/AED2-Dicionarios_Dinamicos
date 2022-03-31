#!/bin/bash

sudo gcc src/usaDicionario.c src/ddinamico.c src/obasico/lista.c src/obasico/vdinamico.c -o program -lm && ./program < $1 > out/$2.txt