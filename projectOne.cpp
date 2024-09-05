#include <iostream>
#include <fstream>

using namespace std;

ifstream inputFile("input1.txt");


class SparseRow {
protected:
    int row;   // Row#
    int col;   // Column#
    int value; // We will assume that all our values will be integers
public:
    SparseRow(); // default constructor; row=-1; col=-1; value=0
    SparseRow(int r, int c, int v);
    void display(); // print Row#, Column#, value
    friend ostream& operator<<(ostream& s, const SparseRow& sr);
    int getRow();
    int getCol();
    int getVal();
    void setRow(int r);
    void setCol(int c);
    void setVal(int v);
};

SparseRow::SparseRow() : row(-1), col(-1), value(0) {}


SparseRow::SparseRow(int r, int c, int v){
    row = r;
    col = c;
    value = v;
}

void SparseRow::display() {
    cout << getRow() << ", " << getCol() << ", " << getVal() << endl;
}

ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << sr.row << ", " << sr.col << ", " << sr.value;
    return s;
}

void SparseRow::setRow(int r) {
    row = r;}

void SparseRow::setCol(int c) {
    col = c;
}

void SparseRow::setVal(int v) {
    value = v;
}

int SparseRow::getRow() {
    return row;
}

int SparseRow::getCol() {
    return col;
}

int SparseRow::getVal() {
    return value;
}

class SparseMatrix {
protected:
    int noRows;         // Number of rows of the original matrix
    int noCols;         // Number of columns of the original matrix
    int commonValue;    // read from input
    int noNonSparseValues;
    SparseRow* myMatrix; // Array
public:
    SparseMatrix();
    SparseMatrix(int n, int m, int cv);
    SparseMatrix(int n, int m, int cv, int noNSV);
    ~SparseMatrix(); // Destructor to free memory
    SparseMatrix* Transpose(); // Matrix Transpose
    SparseMatrix* Multiply(SparseMatrix& M);
    SparseMatrix* Add(SparseMatrix& M);
    friend ostream& operator<<(ostream& s, const SparseMatrix& sm);
    void displayMatrix(); // Display the matrix in its original format
    int getRows();
    int getCols();
};

int SparseMatrix::getCols(){
    return noCols;
}

int SparseMatrix::getRows(){
    return noRows;
}

SparseMatrix::SparseMatrix() {
    noRows = -1;
    noCols = -1;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = nullptr;
}

SparseMatrix::SparseMatrix(int n, int m, int cv, int noNSV) {
    this->noRows = n;  // Correct initialization
    this->noCols = m;
    commonValue = cv;
    noNonSparseValues = noNSV;

    myMatrix = new SparseRow[noNonSparseValues];

    int temp = 0;
    int nonSparseCount = 0;

    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
            inputFile >> temp;
            if (temp != commonValue) {
                myMatrix[nonSparseCount++] = SparseRow(i, j, temp);
            }
        }
    }
}

SparseMatrix::~SparseMatrix() {
    delete[] myMatrix;
}

SparseMatrix* SparseMatrix::Transpose() {
    int temp = 0;
    for(int i = 0; i < noNonSparseValues; i++){
        temp = myMatrix[i].getRow();
        myMatrix[i].setRow(myMatrix[i].getCol());
        myMatrix[i].setCol(temp);  
    }

    return this; // Placeholder return
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    // Check for compatible dimensions
    if (noCols != M.getRows()) {
        cout << "Error: Matrices have incompatible dimensions, cannot multiply." << endl;
        return nullptr;
    }

    // Create a new sparse matrix for the result
    SparseMatrix* result = new SparseMatrix(noRows, M.getCols(), commonValue, 0);
    int maxNonSparseValues = noRows * M.getCols(); // Upper bound
    result->myMatrix = new SparseRow[maxNonSparseValues];

    // Create a temporary 2D array to hold multiplication results
    int** tempMatrix = new int*[noRows];
    for (int i = 0; i < noRows; ++i) {
        tempMatrix[i] = new int[M.getCols()];
        for (int j = 0; j < M.getCols(); ++j) {
            tempMatrix[i][j] = 0;
        }
    }

    // Compute the multiplication
    for (int i = 0; i < noNonSparseValues; ++i) {
        int r1 = myMatrix[i].getRow();
        int c1 = myMatrix[i].getCol();
        int v1 = myMatrix[i].getVal();
        for (int j = 0; j < M.noNonSparseValues; ++j) {
            int r2 = M.myMatrix[j].getRow();
            int c2 = M.myMatrix[j].getCol();
            int v2 = M.myMatrix[j].getVal();
            if (c1 == r2) {
                tempMatrix[r1][c2] += v1 * v2;
            }
        }
    }

    // Store non-zero values in result
    int nonSparseCount = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < M.getCols(); ++j) {
            if (tempMatrix[i][j] != 0) {
                result->myMatrix[nonSparseCount++] = SparseRow(i, j, tempMatrix[i][j]);
            }
        }
    }
    result->noNonSparseValues = nonSparseCount;

    // Cleanup
    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;

    return result;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
    // Check for compatible dimensions
    if (noRows != M.getRows() || noCols != M.getCols()) {
        cout << "Error: Matrices have different dimensions, cannot add." << endl;
        return nullptr;
    }

    // Create a new sparse matrix for the result
    int maxNonSparseValues = noNonSparseValues + M.noNonSparseValues; // Upper bound
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0);
    result->myMatrix = new SparseRow[maxNonSparseValues];
    
    // Create a temporary 2D array to hold sum of matrix values
    int** tempMatrix = new int*[noRows];
    for (int i = 0; i < noRows; ++i) {
        tempMatrix[i] = new int[noCols];
        for (int j = 0; j < noCols; ++j) {
            tempMatrix[i][j] = commonValue;
        }
    }

    // Fill the temporary matrix with values from the current matrix
    for (int i = 0; i < noNonSparseValues; ++i) {
        int r = myMatrix[i].getRow();
        int c = myMatrix[i].getCol();
        int v = myMatrix[i].getVal();
        tempMatrix[r][c] = v;
    }

    // Add values from matrix M
    for (int i = 0; i < M.noNonSparseValues; ++i) {
        int r = M.myMatrix[i].getRow();
        int c = M.myMatrix[i].getCol();
        int v = M.myMatrix[i].getVal();
        tempMatrix[r][c] += v;
    }

    // Store non-zero values in result
    int nonSparseCount = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            if (tempMatrix[i][j] != commonValue) {
                result->myMatrix[nonSparseCount++] = SparseRow(i, j, tempMatrix[i][j]);
            }
        }
    }
    result->noNonSparseValues = nonSparseCount;

    // Cleanup
    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;

    return result;
}

ostream& operator<<(ostream& s, const SparseMatrix& sm) {
    for (int i = 0; i < sm.noNonSparseValues; i++) {
        s << sm.myMatrix[i] << endl;
    }
    return s;
}

void SparseMatrix::displayMatrix() {
    // Create a 2D array to hold the full matrix values initialized to the common value
    int** fullMatrix = new int*[noRows];
    for (int i = 0; i < noRows; ++i) {
        fullMatrix[i] = new int[noCols];
        for (int j = 0; j < noCols; ++j) {
            fullMatrix[i][j] = commonValue;
        }
    }

    // Fill the 2D array with non-zero values from sparse matrix
    for (int i = 0; i < noNonSparseValues; ++i) {
        int r = myMatrix[i].getRow();
        int c = myMatrix[i].getCol();
        int v = myMatrix[i].getVal();
        fullMatrix[r][c] = v;
    }

    // Print the full matrix
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            cout << fullMatrix[i][j] << " ";
        }
        cout << endl;
    }

    // Free the allocated memory
    for (int i = 0; i < noRows; ++i) {
        delete[] fullMatrix[i];
    }
    delete[] fullMatrix;
}

int main() {

    int n, m, cv, noNSV;

    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    // Populate secondOne with data

    cout << "First one in sparse matrix format" << endl;
    cout << *firstOne;
    cout << "After transpose" << endl;
    firstOne->Transpose();
    cout << *firstOne;
    firstOne->Transpose();
    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix();
    cout << "Second one in sparse matrix format" << endl;
    cout << *secondOne;
    cout << "After transpose" << endl;
    secondOne->Transpose();
    cout << *secondOne;
    secondOne->Transpose();
    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix();
    cout << "Matrix addition result" << endl;
    firstOne->Add(*secondOne)->displayMatrix();
    cout << "Matrix multiplication result" << endl;
    firstOne->Multiply(*secondOne)->displayMatrix();
    
    inputFile.close();

    delete firstOne;
    delete secondOne;

    return 0;
}
