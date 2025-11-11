// Elliott Erhard EECS Lab 11
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

class Matrix {
private:
    int n;       // dimension (n x n)
    int* data;   // row-major single buffer of size n*n

    void allocate(int size) {
        if (size <= 0) { data = nullptr; n = 0; return; }
        data = new int[size];
    }

public:
    // Default
    Matrix(): n(0), data(nullptr) {}

    // Construct n x n initialized to 0
    Matrix(int sz): n(sz) {
        allocate(n*n);
        for (int i = 0; i < n*n; ++i) data[i] = 0;
    }

    // Copy constructor (deep copy)
    Matrix(const Matrix& other) {
        n = other.n;
        if (n > 0) {
            allocate(n*n);
            for (int i = 0; i < n*n; ++i) data[i] = other.data[i];
        } else data = nullptr;
    }

    // Copy assignment
    Matrix& operator=(const Matrix& other) {
        if (this == &other) return *this;
        delete [] data;
        n = other.n;
        if (n > 0) {
            allocate(n*n);
            for (int i = 0; i < n*n; ++i) data[i] = other.data[i];
        } else data = nullptr;
        return *this;
    }

    // Destructor
    ~Matrix() {
        delete [] data;
    }

    int size() const { return n; }

    // Access (const and non-const)
    int get(int r, int c) const {
        if (r < 0 || r >= n || c < 0 || c >= n) throw out_of_range("Index out of range");
        return data[r*n + c];
    }
    void set(int r, int c, int val) {
        if (r < 0 || r >= n || c < 0 || c >= n) throw out_of_range("Index out of range");
        data[r*n + c] = val;
    }

    // Read matrix values (expects exactly n*n values to be available in stream)
    void readFromStream(istream& is) {
        if (n <= 0) return;
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < n; ++c)
                is >> data[r*n + c];
    }

    // Pretty print with aligned columns
    void print(ostream& os = cout, int colWidth = 4) const {
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                os << setw(colWidth) << get(r,c);
            }
            os << '\n';
        }
    }

    // Operator + : matrix addition (member)
    Matrix operator+(const Matrix& rhs) const {
        if (n != rhs.n) throw invalid_argument("Matrix sizes do not match for addition");
        Matrix res(n);
        for (int i = 0; i < n*n; ++i) res.data[i] = this->data[i] + rhs.data[i];
        return res;
    }

    // Operator * : matrix multiplication (member)
    Matrix operator*(const Matrix& rhs) const {
        if (n != rhs.n) throw invalid_argument("Matrix sizes do not match for multiplication");
        Matrix res(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                long long acc = 0;
                for (int k = 0; k < n; ++k) {
                    acc += static_cast<long long>(get(i,k)) * rhs.get(k,j);
                }
                res.set(i,j, static_cast<int>(acc));
            }
        }
        return res;
    }

    // Sum of main diagonal
    int sumMainDiagonal() const {
        int s = 0;
        for (int i = 0; i < n; ++i) s += get(i,i);
        return s;
    }

    // Sum of secondary diagonal
    int sumSecondaryDiagonal() const {
        int s = 0;
        for (int i = 0; i < n; ++i) s += get(i, n - 1 - i);
        return s;
    }

    // Swap rows (in-place)
    void swapRowsInPlace(int r1, int r2) {
        if (r1 < 0 || r2 < 0 || r1 >= n || r2 >= n) {
            throw out_of_range("Row index out of bounds");
        }
        if (r1 == r2) return;
        for (int c = 0; c < n; ++c) {
            int tmp = get(r1,c);
            set(r1,c, get(r2,c));
            set(r2,c, tmp);
        }
    }

    // Swap columns (in-place)
    void swapColsInPlace(int c1, int c2) {
        if (c1 < 0 || c2 < 0 || c1 >= n || c2 >= n) {
            throw out_of_range("Column index out of bounds");
        }
        if (c1 == c2) return;
        for (int r = 0; r < n; ++r) {
            int tmp = get(r,c1);
            set(r,c1, get(r,c2));
            set(r,c2, tmp);
        }
    }

    // Utility: returns a copy with swapped rows (pass-by-value usage)
    friend Matrix swappedRows(Matrix m, int r1 = 0, int r2 = 1) {
        if (m.n == 0) return m;
        if (r1 < 0 || r2 < 0 || r1 >= m.n || r2 >= m.n) {
            // invalid indices: just return original copy and let caller print message
            return m;
        }
        m.swapRowsInPlace(r1, r2);
        return m;
    }

    // Utility: returns a copy with swapped columns
    friend Matrix swappedCols(Matrix m, int c1 = 0, int c2 = 1) {
        if (m.n == 0) return m;
        if (c1 < 0 || c2 < 0 || c1 >= m.n || c2 >= m.n) {
            return m;
        }
        m.swapColsInPlace(c1, c2);
        return m;
    }

    // Update element (in-place)
    void update(int r = 0, int c = 0, int value = 100) {
        if (r < 0 || c < 0 || r >= n || c >= n) throw out_of_range("Index out of bounds");
        set(r,c,value);
    }

    // Factory: read two matrices from file. Returns pair of matrices.
    static pair<Matrix,Matrix> readTwoMatricesFromFile(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            throw runtime_error("Could not open file: " + filename);
        }
        int N;
        fin >> N;
        if (!fin) throw runtime_error("Invalid file format (missing N)");
        Matrix A(N), B(N);
        A.readFromStream(fin);
        B.readFromStream(fin);
        return {A,B};
    }
};

// Helper: prompt and safely get integer (no extra validation here)
int getIntFromUser(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. " << prompt;
    }
    return x;
}

int main() {
    cout << "=== EECS 348 Lab 11 - Matrix class operations ===\n";
    cout << "Enter input filename (contains N and two N x N matrices): ";
    string filename;
    cin >> filename;

    try {
        auto pp = Matrix::readTwoMatricesFromFile(filename);
        Matrix A = pp.first;
        Matrix B = pp.second;

        cout << "\nMatrix A:\n";
        A.print(cout, 4);

        cout << "\nMatrix B:\n";
        B.print(cout, 4);

        // Addition
        cout << "\nA + B =\n";
        Matrix sum = A + B;
        sum.print(cout, 4);

        // Multiplication
        cout << "\nA * B =\n";
        Matrix prod = A * B;
        prod.print(cout, 4);

        // Diagonal sums
        cout << "\nDiagonal sums (Matrix A):\n";
        cout << "Main diagonal sum: " << A.sumMainDiagonal() << '\n';
        cout << "Secondary diagonal sum: " << A.sumSecondaryDiagonal() << '\n';

        // Swap rows (by value) default 0 and 1
        cout << "\nSwap rows (by value) default 0 and 1 on A (original A remains unchanged):\n";
        Matrix swappedR = swappedRows(A); // pass-by-value inside function, returns modified copy
        swappedR.print(cout, 4);
        cout << "Confirm original A (unchanged):\n";
        A.print(cout, 4);

        // Swap columns (by value) default 0 and 1
        cout << "\nSwap columns (by value) default 0 and 1 on B (original B remains unchanged):\n";
        Matrix swappedC = swappedCols(B);
        swappedC.print(cout, 4);
        cout << "Confirm original B (unchanged):\n";
        B.print(cout, 4);

        // Update row/col (defaults r=0,c=0,val=100) — demonstrate default
        cout << "\nUpdate A[0][0] to 100 (in-place) and display:\n";
        Matrix A_copy = A; // keep original if we want
        A_copy.update(); // defaults apply
        A_copy.print(cout, 4);

        // Also allow user to customize an update (interactive)
        cout << "\nOptional: perform a custom update on B.\n";
        char choice;
        cout << "Do you want to update an element in B? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            int rr = getIntFromUser("Enter row index (0-based): ");
            int cc = getIntFromUser("Enter column index (0-based): ");
            int val = getIntFromUser("Enter new value: ");
            try {
                Matrix B_copy = B;
                B_copy.update(rr, cc, val);
                cout << "B after update:\n";
                B_copy.print(cout, 4);
            } catch (const exception& e) {
                cout << "Update failed: " << e.what() << '\n';
            }
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
