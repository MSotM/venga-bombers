#!/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

FILE=${2:-technical-design.tex}

case "$1" in
    clean)
        latexmk -c
        ;;
    clean-all)
        latexmk -C
        ;;
    watch)
        latexmk -pvc -lualatex ${FILE}
        ;;
    build|*)
        latexmk -lualatex ${FILE}
        ;;
esac
