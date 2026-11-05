
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Matrix {
private:
    int size;
    int* data;

public:
    explicit Matrix(int n = 0) : size(n), data(n ? new int[n * n]() : nullptr) {}

    Matrix(const Matrix& other) : Matrix(other.size) {
        if (size) copy(other.data, other.data + size * size, data);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            Matrix temp(other);
            swap(size, temp.size);
            swap(data, temp.data);
        }
        return *this;
    }

    ~Matrix() { delete[] data; }

    int getSize() const { return size; }

    bool validIndex(int index) const {
        return index >= 0 && index < size;
    }

    int get(int row, int col) const {
        if (!validIndex(row) || !validIndex(col))
            throw out_of_range("Invalid matrix index");
        return data[row * size + col];
    }

    void set(int row, int col, int value) {
        if (!validIndex(row) || !validIndex(col))
            throw out_of_range("Invalid matrix index");
        data[row * size + col] = value;
    }

    bool read(istream& input) {
        for (int i = 0; i < size * size; ++i) {
            if (!(input >> data[i]))
                return false;
        }
        return true;
    }

    void display() const {
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                cout << setw(4) << get(row, col);
            }
            cout << '\n';
        }
    }

    // Practice 2: Matrix addition
    Matrix operator+(const Matrix& other) const {
        if (size != other.size)
            throw invalid_argument("Matrix sizes do not match");

        Matrix result(size);

        for (int i = 0; i < size * size; ++i) {
            result.data[i] = data[i] + other.data[i];
        }

        return result;
    }

    // Practice 3: Matrix multiplication
    Matrix operator*(const Matrix& other) const {
        if (size != other.size)
            throw invalid_argument("Matrix sizes do not match");

        Matrix result(size);

        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                for (int k = 0; k < size; ++k) {
                    result.data[row * size + col] +=
                        get(row, k) * other.get(k, col);
                }
            }
        }

        return result;
    }

    // Practice 4: Main diagonal
    int mainDiagonalSum() const {
        int total = 0;

        for (int i = 0; i < size; ++i) {
            total += get(i, i);
        }

        return total;
    }

    // Practice 4: Secondary diagonal
    int secondaryDiagonalSum() const {
        int total = 0;

        for (int i = 0; i < size; ++i) {
            total += get(i, size - 1 - i);
        }

        return total;
    }

    // Practice 5: Swap rows
    void swapRows(int first, int second) {
        if (!validIndex(first) || !validIndex(second))
            throw out_of_range("Invalid row index");

        for (int col = 0; col < size; ++col) {
            swap(data[first * size + col],
                 data[second * size + col]);
        }
    }

    // Practice 6: Swap columns
    void swapColumns(int first, int second) {
        if (!validIndex(first) || !validIndex(second))
            throw out_of_range("Invalid column index");

        for (int row = 0; row < size; ++row) {
            swap(data[row * size + first],
                 data[row * size + second]);
        }
    }
};

// Passing by value keeps the original matrix unchanged
void showSwappedRows(Matrix matrix, int first = 0, int second = 1) {
    matrix.swapRows(first, second);
    matrix.display();
}

void showSwappedColumns(Matrix matrix, int first = 0, int second = 1) {
    matrix.swapColumns(first, second);
    matrix.display();
}

// Practice 7: Update an element
void showUpdatedMatrix(Matrix matrix, int row = 0,
                       int col = 0, int value = 100) {
    matrix.set(row, col, value);
    matrix.display();
}

int main() {
    string filename;

    cout << "Enter input filename: ";
    cin >> filename;
    cout << '\n';

    ifstream file(filename.c_str());

    if (!file) {
        cerr << "Error opening file: " << filename << '\n';
        return 1;
    }

    int n;
    if (!(file >> n) || n <= 0) {
        cerr << "Invalid matrix size.\n";
        return 1;
    }

    Matrix A(n), B(n);

    if (!A.read(file) || !B.read(file)) {
        cerr << "The file does not contain two complete matrices.\n";
        return 1;
    }

    // Practice 1
    cout << "Matrix A:\n";
    A.display();

    cout << "\nMatrix B:\n";
    B.display();

    // Practice 2
    cout << "\nA + B:\n";
    (A + B).display();

    // Practice 3
    cout << "\nA * B:\n";
    (A * B).display();

    // Practice 4
    cout << "\nDiagonal sums for Matrix A:\n";
    cout << "Main diagonal sum: "
         << A.mainDiagonalSum() << '\n';
    cout << "Secondary diagonal sum: "
         << A.secondaryDiagonalSum() << '\n';

    // Practices 5 and 6
    if (n >= 3) {
        cout << "\nProblem 5 - Rows 0 and 2 swapped:\n";
        showSwappedRows(A, 0, 2);

        cout << "\nProblem 6 - Columns 0 and 2 swapped:\n";
        showSwappedColumns(A, 0, 2);
    }
    else if (n >= 2) {
        cout << "\nProblem 5 - Rows 0 and 1 swapped:\n";
        showSwappedRows(A);

        cout << "\nProblem 6 - Columns 0 and 1 swapped:\n";
        showSwappedColumns(A);
    }
    else {
        cout << "\nProblem 5 - Rows 0 and 0 swapped:\n";
        showSwappedRows(A, 0, 0);

        cout << "\nProblem 6 - Columns 0 and 0 swapped:\n";
        showSwappedColumns(A, 0, 0);
    }

    // Practice 7
    cout << "\nProblem 7 - Updated matrix:\n";

    if (n >= 3)
        showUpdatedMatrix(A, 1, 2, 99);
    else
        showUpdatedMatrix(A);

    return 0;
}
