//Elliott Erhard EECS Lab 11
#include "Matrix.h"
#include <iostream>
#include <string>
using namespace std;

int getInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(10000,'\n');
        cout << "Invalid input. " << prompt;
    }
    return x;
}

int main() {
    cout << "=== EECS 348 Lab 11 - Matrix Operations ===\n";
    string filename;
    cout << "Enter input filename: ";
    cin >> filename;

    try {
        auto pairAB = Matrix::readTwoMatricesFromFile(filename);
        Matrix A = pairAB.first;
        Matrix B = pairAB.second;

        cout << "\nMatrix A:\n"; A.print();
        cout << "\nMatrix B:\n"; B.print();

        cout << "\nA + B:\n"; (A + B).print();
        cout << "\nA * B:\n"; (A * B).print();

        cout << "\nMain diagonal sum of A: " << A.sumMainDiagonal() << endl;
        cout << "Secondary diagonal sum of A: " << A.sumSecondaryDiagonal() << endl;

        cout << "\nSwapping rows 0 and 1 in A (copy):\n";
        swappedRows(A,0,1).print();

        cout << "\nSwapping cols 0 and 1 in B (copy):\n";
        swappedCols(B,0,1).print();

        cout << "\nUpdating A[0][0] to 100:\n";
        A.update();
        A.print();

        cout << "\nWould you like to update an element in B? (y/n): ";
        char ch; cin >> ch;
        if (ch=='y'||ch=='Y') {
            int r = getInt("Row index: ");
            int c = getInt("Column index: ");
            int val = getInt("New value: ");
            B.update(r,c,val);
            cout << "Updated B:\n"; B.print();
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
