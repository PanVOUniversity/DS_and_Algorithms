# Gaussian Solver with Eigen and CSV

This project provides a modular C++ solution for solving systems of linear equations using the Gaussian method (via Eigen), with input and output in CSV format. It includes:
- CSV reading/writing utilities
- A reproducible random system generator
- Modular tests

## Structure

- `solver.cpp` — Main program: reads a system from CSV, solves it, writes the solution to CSV
- `csv_io.h`, `csv_io.cpp` — CSV input/output utilities for Eigen matrices/vectors
- `generate_system.cpp` — Utility to generate random reproducible systems in CSV
- `test_solver.cpp` — Modular test for the solver

## Requirements
- C++ compiler (g++)
- [Eigen3](https://eigen.tuxfamily.org/) (install with `sudo apt install libeigen3-dev` on Ubuntu)

## Build

From the project root:

```
g++ -I /usr/include/eigen3 -o solver Gaus_solving/solver.cpp Gaus_solving/csv_io.cpp
g++ -I /usr/include/eigen3 -o test_solver Gaus_solving/test_solver.cpp Gaus_solving/csv_io.cpp
g++ -o generate_system Gaus_solving/generate_system.cpp
```

## Usage

### 1. System generation

```
./generate_system <size> <output.csv> <seed>
# Пример:
./generate_system 3 system.csv 42
```

### 2. System solving

```
./solver <input.csv> <output.csv>
# Пример:
./solver system.csv solution.csv
```

### 3. Tests

```
./test_solver
```

## Формат CSV

- Входной файл: каждая строка — коэффициенты уравнения, последняя колонка — свободный член (b)
- Выходной файл: по одному значению решения на строку

**Пример входа:**
```
2,1,8
1,3,13
```
(решается система: 2x + y = 8, x + 3y = 13)

**Пример выхода:**
```
3
4
```

## Примечания
- Для больших систем используйте генератор с нужным seed для воспроизводимости
- Решение использует аппаратную векторизацию через Eigen (если поддерживается компилятором/CPU) 