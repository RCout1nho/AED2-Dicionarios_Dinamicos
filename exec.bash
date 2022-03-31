#!/bin/bash

sudo gcc usaDicionario.c ddinamico.c obasico/lista.c obasico/vdinamico.c -o program -lm && ./program < $1