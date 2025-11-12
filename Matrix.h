// Elliott Erhard EECS Lab 11
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

class Matrix {
private:
    int n;
    int* data;
    void allocate(int size);

public:
    Matrix();
    Matrix(int sz);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    ~Matrix();

    int size() const;
    int get(int r, int c) const;
    void set(int r, int c, int val);
    void readFromStream(std::istream& is);
    void print(std::ostream& os = std::cout, int colWidth = 4) const;

    Matrix operator+(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;

    int sumMainDiagonal() const;
    int sumSecondaryDiagonal() const;

    void swapRowsInPlace(int r1, int r2);
    void swapColsInPlace(int c1, int c2);
    void update(int r = 0, int c = 0, int value = 100);

    static std::pair<Matrix, Matrix> readTwoMatricesFromFile(const std::string& filename);

    friend Matrix swappedRows(Matrix m, int r1, int r2);
    friend Matrix swappedCols(Matrix m, int c1, int c2);
};

#endif
