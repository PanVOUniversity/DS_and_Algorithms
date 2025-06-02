#!/bin/bash


g++ -o shannon_codec main.cpp Encoder.cpp Decoder.cpp ShannonCoding.cpp -std=c++17


if [ $? -eq 0 ]; then
    echo "Компиляция завершена успешно. Исполняемый файл: shannon_codec"
else
    echo "Ошибка компиляции."
    exit 1
fi

