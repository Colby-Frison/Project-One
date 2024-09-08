#include <iostream>

using namespace std;
 
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

Part 2 of the project is at line 403 
Part 3 of the projcet is at line 918

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
            cin >> temp;
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
        return new SparseMatrix();
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
        return new SparseMatrix();
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
    if (myMatrix != nullptr) {
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
}

int main() {

    int n, m, cv, noNSV;//craete variables to read to

    //read file to varibles
    //I could not for the life of me get this to work so I had to use 
    //<fstream>, but I'm hoping when redirected input is used it all works as intened
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);//reads matrix

    // Populate secondOne with data
    cin >> n >> m >> cv >> noNSV;
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
    
    //delete matrixes created
    delete firstOne;
    delete secondOne;

    return 0;//end
}


/*

**Part 2**

I will be documenting my LLM usage here

I didnt really use AI a lot in the project the only time I used it was to implement the multiply add and display matrix methods

For the most part these methods are purely just logic and math so I saw the usage of a LLM to help solve the problem
Far more beneficial than slaving away to for a long time just to come to the same solution

for the prompt I had copy and pasted the entirety of the code with the unimplented methods in place 
then I then wrote under that to implement the methods add, multiply, and displayMatrix from the sparse matrix class.

At first the add and multiply did not work so I asked it to return in matrix form instead of sprase matrix, then it worked, in honesty I dont
know if the problme I aws fasing was a bug in teh code my my final implementastion worked, so i didn't touch it

I also asked it how to reidrect input, but I still could not get it to work
The last thing I asked which is documentedf at the bottom of the following exhcange, is why the porgram would end after completion
which ended up just being I didnt clode the input file

To make the exchange more readable I've added lines between each question and response

and if instead of reading prompts and resonpses in this, here is teh link for the chatroom: https://chatgpt.com/share/6f9a8b07-9311-4477-b6fb-d01ebafdb649 

Here are the prompts minus my copy and pasted code:
_________________________________________________________________________________
_________________________________________________________________________________
_________________________________________________________________________________

me: 
---
code
---

I would like for you to implement the SparseRow::DisplayMatrix() method

_________________________________________________________________________________

LLM:
Certainly! To implement the SparseMatrix::displayMatrix() method, you need to consider the following:

Matrix Representation: In a sparse matrix, only the non-zero elements are stored. To display the matrix in its full format, you need to reconstruct the full matrix from these non-zero elements.

Output Format: You will print the matrix as a grid where each cell corresponds to the values at their respective positions.

Here’s how you can implement SparseMatrix::displayMatrix():

cpp
Copy code
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
Explanation:
Initialize the Full Matrix: Create a 2D array where each element is initialized to the commonValue. This represents the matrix in its default state.

Fill with Non-Zero Values: Use the myMatrix array to update the values of the non-zero elements in the 2D array.

Print the Matrix: Iterate through the 2D array and print each element, ensuring the output format resembles a matrix.

Memory Management: After printing the matrix, deallocate the memory used by the 2D array to avoid memory leaks.

Make sure your matrix's noRows, noCols, and myMatrix values are correctly set before calling this method. If there are issues or additional requirements, let me know!

_________________________________________________________________________________________________

me: 
Now implement these two methods from the code, without using any addition imports

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    // Implement the multiplication logic here
    return new SparseMatrix(); // Placeholder return
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
    // Implement the addition logic here
    return new SparseMatrix(); // Placeholder return
}

____________________________________________________________________________________________________

LLM:
To implement matrix multiplication and addition for sparse matrices, we need to handle the operations while dealing with the fact that only non-zero elements are stored. Here’s how you can implement these methods.

Matrix Addition
Matrix addition is straightforward. To add two sparse matrices, we need to:

Ensure that both matrices have the same dimensions.
Add corresponding non-zero elements from both matrices.
Construct a new sparse matrix to store the result.
Matrix Multiplication
Matrix multiplication is more complex. To multiply two sparse matrices, we need to:

Ensure that the number of columns in the first matrix matches the number of rows in the second matrix.
Compute the dot product of rows and columns to get the resulting matrix.
Construct a new sparse matrix to store the result.
Here’s how you can implement these methods:

cpp
Copy code
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
Explanation:
Matrix Addition:

Dimension Check: Ensure that both matrices have the same dimensions.
Create Result Matrix: Allocate space for the resulting sparse matrix.
Initialize Temporary Matrix: Fill the temporary matrix with the commonValue.
Add Values: Update the temporary matrix with values from both matrices.
Store Non-Zero Values: Populate the result matrix with non-zero values.
Cleanup: Free the temporary matrix.
Matrix Multiplication:

Dimension Check: Ensure that the number of columns in the first matrix matches the number of rows in the second matrix.
Create Result Matrix: Allocate space for the resulting sparse matrix.
Initialize Temporary Matrix: Fill the temporary matrix with zeros.
Compute Multiplication: Update the temporary matrix with the product of values.
Store Non-Zero Values: Populate the result matrix with non-zero values.
Cleanup: Free the temporary matrix.
With these methods, you can perform matrix operations efficiently using the sparse matrix representation. If you have any questions or need further adjustments, let me know!

_________________________________________________________________________________________________________

me:
have the result be in matrix form instead of sparse matrix form

__________________________________________________________________________________________________________

LLM:
To modify the Add and Multiply methods so that the result is represented in matrix form (i.e., a full matrix where all elements, including zeros, are stored), you need to adjust the implementation to build a complete matrix and return it in that format.

Matrix Addition in Full Matrix Form
For matrix addition, instead of storing non-zero values in a sparse format, you will store the entire matrix (including zeros) in a full matrix form.

Matrix Multiplication in Full Matrix Form
For matrix multiplication, you'll also store the result as a full matrix, including all zeros.

Here's how you can implement these methods:

cpp
Copy code
SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
    // Check for compatible dimensions
    if (noRows != M.getRows() || noCols != M.getCols()) {
        cout << "Error: Matrices have different dimensions, cannot add." << endl;
        return nullptr;
    }

    // Create a new sparse matrix for the result with full matrix form
    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0);
    
    // Create a temporary 2D array to hold the sum of matrix values
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

    // Store the matrix in the result object
    result->noNonSparseValues = 0; // Initialize with zero, will not be used
    result->myMatrix = new SparseRow[noRows * noCols];
    
    int index = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < noCols; ++j) {
            if (tempMatrix[i][j] != commonValue) {
                result->myMatrix[index++] = SparseRow(i, j, tempMatrix[i][j]);
            }
        }
    }
    // Update the non-zero values count
    result->noNonSparseValues = index;

    // Cleanup
    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;

    return result;
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    // Check for compatible dimensions
    if (noCols != M.getRows()) {
        cout << "Error: Matrices have incompatible dimensions, cannot multiply." << endl;
        return nullptr;
    }

    // Create a new sparse matrix for the result with full matrix form
    SparseMatrix* result = new SparseMatrix(noRows, M.getCols(), 0, 0);
    
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

    // Store the matrix in the result object
    result->noNonSparseValues = 0; // Initialize with zero, will not be used
    result->myMatrix = new SparseRow[noRows * M.getCols()];
    
    int index = 0;
    for (int i = 0; i < noRows; ++i) {
        for (int j = 0; j < M.getCols(); ++j) {
            if (tempMatrix[i][j] != 0) {
                result->myMatrix[index++] = SparseRow(i, j, tempMatrix[i][j]);
            }
        }
    }
    // Update the non-zero values count
    result->noNonSparseValues = index;

    // Cleanup
    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;

    return result;
}
Explanation:
Matrix Addition:

Dimension Check: Verify that the matrices have the same dimensions.
Create Result Matrix: Initialize the result matrix.
Initialize Temporary Matrix: Fill the temporary matrix with the commonValue.
Add Values: Update the temporary matrix with values from both matrices.
Store Non-Zero Values: Populate the result matrix with non-zero values.
Cleanup: Free the temporary matrix.
Matrix Multiplication:

Dimension Check: Verify that the number of columns in the first matrix matches the number of rows in the second matrix.
Create Result Matrix: Initialize the result matrix.
Initialize Temporary Matrix: Fill the temporary matrix with zeros.
Compute Multiplication: Calculate the matrix product and update the temporary matrix.
Store Non-Zero Values: Populate the result matrix with non-zero values.
Cleanup: Free the temporary matrix.
These implementations create full matrices where all elements, including zeros, are stored in the result. If you need further adjustments or have more questions, feel free to ask!

___________________________________________________________________________________________

me:
Last thing, why doesnt the program end when it is done

_____________________________________________________________________________________________

LLM:
If your program doesn't end as expected, it could be due to several potential issues. Here are some common reasons and troubleshooting tips:

1. Unclosed Files
Ensure that all files are properly closed. In your code, inputFile.close() should close the file, but if there's any issue with file handling, it might cause problems. Ensure that the file is properly opened and closed without issues.

2. Memory Leaks
Make sure all dynamically allocated memory is properly freed. You've used new to allocate memory for arrays and matrices, so verify that you are freeing all allocated memory correctly. In your code, you've handled cleanup for most of the dynamically allocated memory, but double-check for any missing deletions.

3. Infinite Loops
Verify that there are no infinite loops or conditions that might cause the program to hang. Since your main function looks straightforward, this is less likely, but it’s good to check.

4. Unfinished Operations
If there are unfinished operations or operations that cause the program to wait indefinitely, such as incorrect file operations or unhandled exceptions, the program might not end as expected.

5. Program Termination Issues
If you are running the program in an environment where it might not terminate correctly (e.g., some IDEs or command-line environments), try running the compiled binary directly from the command line to see if it behaves differently.

Debugging Steps
Check for Warnings or Errors: Compile your code with warnings enabled (-Wall for GCC/Clang) and check for any warnings or errors that might indicate issues.

Add Debug Statements: Add cout statements or use a debugger to trace the program's execution and identify where it might be hanging or not terminating.

Ensure Proper Cleanup: Verify that all resources are cleaned up properly. For example, ensure all dynamically allocated arrays are deleted, and check that no memory is leaked.

Test Minimal Case: Simplify your program to a minimal working example to isolate the problem. This can help you identify if the issue is in a specific part of the code.

Here’s an additional check to ensure proper cleanup in your main function:

cpp
Copy code
int main() {
    int n, m, cv, noNSV;

    // Open file
    if (!inputFile.is_open()) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }

    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    inputFile >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);

    // Process matrices
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

    // Matrix operations
    SparseMatrix* resultAdd = firstOne->Add(*secondOne);
    if (resultAdd) {
        cout << "Matrix addition result" << endl;
        cout << *resultAdd;
        resultAdd->displayMatrix();
        delete resultAdd;
    }

    SparseMatrix* resultMult = firstOne->Multiply(*secondOne);
    if (resultMult) {
        cout << "Matrix multiplication result" << endl;
        cout << *resultMult;
        resultMult->displayMatrix();
        delete resultMult;
    }

    // Cleanup
    delete firstOne;
    delete secondOne;
    inputFile.close();

    return 0;
}
Summary
Ensure that all files are closed properly, memory is freed, and no infinite loops or unfinished operations are present. Debugging and simplifying the code can help isolate and resolve the issue. If the program still doesn't end, consider running it in a different environment or checking for any external factors that might be affecting its execution.

______________________________________________________________________________________________________________

**Part 3**

To debug the code I bassically just used the provided input code, and checked it with the output code,
early in the coding process while i didnt have all the code done, or there were sitll bugs with exsiting code
I used the feature in vscode where you can set a marker of where you wan the code to stop then you can increment 
forward line by line to see exactly where and what the poblem is.

Theres also a section that shows what varibles are created, what values are read where they are assigned and so on.
I usewd this feature to make sure all the values were being stored correctly at the right time.

One instance where this was really useful was when I was implementing the sparseMatrix constructor (I think), I kept 
having an issue were it just wouldnt print anyhting out, and I had no clue why, after incrementing line by line, I found 
it was just contructing the sparseRows and my computer was just really slow. I then FOUNd out it was taking this long becuase 
every time I ran the program I would click f5 a lot instead of once, so it would run the program many times over, since then 
I've laerned my lesson. 

Another major issue I had in the development process was getting redirecting input ot work. it was giving me values most of the 
time but it was never the correct values istead it would only be '0202' no matter what file I read. very wierd.
I did the line increment thing to see where the error was along with print statments scattered about to read what was being read my the program
I could never figure out the issue so i switched to temporarily using fstream to read the file, which as you can see I have deleted before 
final commit to github/project submit

after I got input1 working correctly i went through all the other provided input files, luckly more me the LLM had anticipated secnarios 
were matrix size was not compatible for add or multiply methods and had already had a backup, I just had to change the print statments 
to match the output files, along with having it reutrn a nullPtr matrix so the program would still continue.

I then continued to the last few input files, ensuring functionality. 

I then finished the program by removing the dreaded fstream implemenations, along with writing parts 2 and 3 of the project.

If for some reason the code doesn't work the program will pe commit to github with a time stamp. :)

*/