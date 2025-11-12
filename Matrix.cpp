//Elliott Erhard EECS Lab 11
#include "Matrix.h"
using namespace std;

void Matrix::allocate(int size) {
    if (size <= 0) { data = nullptr; n = 0; return; }
    data = new int[size];
}

Matrix::Matrix() : n(0), data(nullptr) {}

Matrix::Matrix(int sz) : n(sz) {
    allocate(n*n);
    for (int i = 0; i < n*n; ++i) data[i] = 0;
}

Matrix::Matrix(const Matrix& other) {
    n = other.n;
    if (n > 0) {
        allocate(n*n);
        for (int i = 0; i < n*n; ++i) data[i] = other.data[i];
    } else data = nullptr;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;
    delete [] data;
    n = other.n;
    if (n > 0) {
        allocate(n*n);
        for (int i = 0; i < n*n; ++i) data[i] = other.data[i];
    } else data = nullptr;
    return *this;
}

Matrix::~Matrix() {
    delete [] data;
}

int Matrix::size() const { return n; }

int Matrix::get(int r, int c) const {
    if (r < 0 || r >= n || c < 0 || c >= n) throw out_of_range("Index out of range");
    return data[r*n + c];
}

void Matrix::set(int r, int c, int val) {
    if (r < 0 || r >= n || c < 0 || c >= n) throw out_of_range("Index out of range");
    data[r*n + c] = val;
}

void Matrix::readFromStream(istream& is) {
    if (n <= 0) return;
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            is >> data[r*n + c];
}

void Matrix::print(ostream& os, int colWidth) const {
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c)
            os << setw(colWidth) << get(r,c);
        os << '\n';
    }
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (n != rhs.n) throw invalid_argument("Matrix sizes do not match for addition");
    Matrix res(n);
    for (int i = 0; i < n*n; ++i) res.data[i] = data[i] + rhs.data[i];
    return res;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (n != rhs.n) throw invalid_argument("Matrix sizes do not match for multiplication");
    Matrix res(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k)
                sum += get(i,k) * rhs.get(k,j);
            res.set(i,j,sum);
        }
    return res;
}

int Matrix::sumMainDiagonal() const {
    int s = 0;
    for (int i = 0; i < n; ++i) s += get(i,i);
    return s;
}

int Matrix::sumSecondaryDiagonal() const {
    int s = 0;
    for (int i = 0; i < n; ++i) s += get(i,n-1-i);
    return s;
}

void Matrix::swapRowsInPlace(int r1, int r2) {
    if (r1 < 0 || r2 < 0 || r1 >= n || r2 >= n) throw out_of_range("Row index out of range");
    if (r1 == r2) return;
    for (int c = 0; c < n; ++c) {
        int tmp = get(r1,c);
        set(r1,c,get(r2,c));
        set(r2,c,tmp);
    }
}

void Matrix::swapColsInPlace(int c1, int c2) {
    if (c1 < 0 || c2 < 0 || c1 >= n || c2 >= n) throw out_of_range("Column index out of range");
    if (c1 == c2) return;
    for (int r = 0; r < n; ++r) {
        int tmp = get(r,c1);
        set(r,c1,get(r,c2));
        set(r,c2,tmp);
    }
}

void Matrix::update(int r, int c, int value) {
    if (r < 0 || c < 0 || r >= n || c >= n) throw out_of_range("Index out of range");
    set(r,c,value);
}

pair<Matrix, Matrix> Matrix::readTwoMatricesFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) throw runtime_error("Could not open file " + filename);
    int N; fin >> N;
    Matrix A(N), B(N);
    A.readFromStream(fin);
    B.readFromStream(fin);
    return {A,B};
}

Matrix swappedRows(Matrix m, int r1, int r2) {
    if (r1 >= 0 && r2 >= 0 && r1 < m.n && r2 < m.n) m.swapRowsInPlace(r1,r2);
    return m;
}

Matrix swappedCols(Matrix m, int c1, int c2) {
    if (c1 >= 0 && c2 >= 0 && c1 < m.n && c2 < m.n) m.swapColsInPlace(c1,c2);
    return m;
}
