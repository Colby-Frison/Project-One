#include <iostream>
#include <fstream> //needed for file input

using namespace std;

ifstream inputFile("input1.txt"); 
//could not get redirect input to work

/*****************************************************************************************************
             ** Colby Frison ** CS - 2413 -- Data Structures ** 9/5/2024 **
*******************************************************************************************************

Instructions so I don't forgot how this works :)

A sparse matrix is a kind of matrix where only non standard values are held
This is done by storing the row and col number along wiht its value

The sparseRow class stores each individual non standard number with its position
To make the matrix an array of type SparseRows are made

Then to make the standard matrix format its just a matter of formatting the data

Another important detail is how data is read from the txt file:
the txt file is formatted so that the first four numbers determine the number of rows then cols, 
what the standard value is, and lastly how many non-standard values there are

Besides that its pretty standard, getters and setters, constructors and destructors, etc.

Ther is an overload operator for << too so it I ca just "cout << Row" istead of a long line of 
getting values

The last aspects are the transpose, add, and multiply methods. I'll go more in depth at their method

******************************************************************************************************/



// sparseRow class, already explained what it was in desc. above
// only have default methods in here cuz thats what the project instructions say to do
// and it its better for code readability (i think, I forgot what prof. said)
class SparseRow {
protected:
    int row;   // Row#
    int col;   // Column#
    int value; // We will assume that all our values will be integers
public:
    SparseRow(); // default constructor, sets -1 , -1 , 0
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

//default contructor
SparseRow::SparseRow(){ 
    row = -1;
    col = -1;
    value = 0;
}

//sets vals to given nums
SparseRow::SparseRow(int r, int c, int v){
    row = r;
    col = c;
    value = v;
}


// honestly don't know why I need this since I have the << overload
// but it was in the provided premade class so idk
// Also it gets mad at me if I dont put void ... idk
void SparseRow::display() {
    cout << getRow() << ", " << getCol() << ", " << getVal() << endl;
}


//Overload operator :)
// creates string to print basically
ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << sr.row << ", " << sr.col << ", " << sr.value;
    return s;
}


// getters and setters
void SparseRow::setRow(int r) {row = r;}

void SparseRow::setCol(int c) {col = c;}

void SparseRow::setVal(int v) {value = v;}

int SparseRow::getRow() {return row;}

int SparseRow::getCol() {return col;}

int SparseRow::getVal() {return value;}



//sparse matrix class, once again already kinda explained
//all deafults again
//with the constructor methodslittle confused cuz the provided code snippits
//say to use the one with 3 varaibles, but the noNSV is kinda important so idk
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

//getters and setters
int SparseMatrix::getCols(){return noCols;}

int SparseMatrix::getRows(){return noRows;}


//dfefault constructr
SparseMatrix::SparseMatrix() {
    noRows = -1;
    noCols = -1;
    commonValue = 0;
    noNonSparseValues = 0;
    myMatrix = nullptr;
}

//constructor with vals
SparseMatrix::SparseMatrix(int n, int m, int cv, int noNSV) {
    noRows = n;  
    noCols = m;
    commonValue = cv;
    noNonSparseValues = noNSV;

    myMatrix = new SparseRow[noNonSparseValues]; //initialize matrix with size = number of nonstandard

    int temp = 0;
    int nonSparseCount = 0;

    //nested loop going through input input file using rows and cols
    //pulls temp val when not the standard val
    //then creates new sparseRow object at location in myMAtrix arr
    for (int i = 0; i < noRows; i++) {
        for (int j = 0; j < noCols; j++) {
            inputFile >> temp;
            if (temp != commonValue) {
                myMatrix[nonSparseCount++] = SparseRow(i, j, temp);
            }
        }
    }
}

//Destructor
SparseMatrix::~SparseMatrix() {
    delete[] myMatrix;
}


//Transpose matrix method
//Ok, so I have no clue if this was the intended way or not, but i litterally just swithc the values
//Then I just print them sparseMatrix like normal then I transpose again so its back to normal for the add and multiply
//Don't know if this is the intened way, but it works
SparseMatrix* SparseMatrix::Transpose() {
    int temp = 0;
    for(int i = 0; i < noNonSparseValues; i++){
        temp = myMatrix[i].getRow();
        myMatrix[i].setRow(myMatrix[i].getCol());
        myMatrix[i].setCol(temp);  
    }

    return this;
}


// multiply method >:( 
//litterally just matrix mulitplication
SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    // Check for compatible dimensions; if cols != rows, no work so throw error
    if (noCols != M.getRows()) {
        cout << "Matrix mulitplication is not possible" << endl;
    }

    // Create a new sparse matrix for the result
    SparseMatrix* result = new SparseMatrix(noRows, M.getCols(), commonValue, 0);
    int maxNonSparseValues = noRows * M.getCols(); // Upper bound
    result->myMatrix = new SparseRow[maxNonSparseValues];//initilize

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

    // delete stuff
    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;

    return result;
}

//Matrix add method >:(
//pretty similar to last one but just adding now
SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
    // Check for compatible dimensions
    if (noRows != M.getRows() || noCols != M.getCols()) {
        cout << "Matrix addition is not possible" << endl;
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

//<< overload again
ostream& operator<<(ostream& s, const SparseMatrix& sm) {
    for (int i = 0; i < sm.noNonSparseValues; i++) {
        s << sm.myMatrix[i] << endl;
    }
    return s;
}

//Dispaly in standard matrix form
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

    int n, m, cv, noNSV;//craete variables to read to

    //read file to varibles
    //I could not for the life of me get this to work so I had to use 
    //<fstream>, but I'm hoping when redirected input is used it all works as intened
    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);//reads matrix

    // Populate secondOne with data
    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    

    cout << "First one in sparse matrix format" << endl;
    cout << *firstOne;//print matrix in sparse form
    cout << "After transpose" << endl;
    firstOne->Transpose(); //flip
    cout << *firstOne;
    firstOne->Transpose(); //flip back
    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix(); //display as matrix
    cout << "Second one in sparse matrix format" << endl;
    cout << *secondOne;//displat in sparce form
    cout << "After transpose" << endl;
    secondOne->Transpose(); //flip
    cout << *secondOne;
    secondOne->Transpose(); // flip back
    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix(); // disp in matrix form
    cout << "Matrix addition result" << endl;
    firstOne->Add(*secondOne)->displayMatrix(); // display add results in matrix form, probably could just have the method do this, but oh well
    cout << "Matrix multiplication result" << endl;
    firstOne->Multiply(*secondOne)->displayMatrix(); // disp multi results
    
    inputFile.close(); //close file (that I wasnt supposed to use heheheh)

    //delete matrixes created
    delete firstOne;
    delete secondOne;

    return 0;//end
}
