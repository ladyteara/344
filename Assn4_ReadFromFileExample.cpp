// Compile this program from the command line using     
//      g++ -std=c++11 ModelSampleProgram.cpp
// Compile this program in an IDE (CodeBlocks, VisualStudio, etc.) by 
//     setting compiler to use C++11 standard

// Each of these include files is required to use some function 
// that is defined in that file. 
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

// This line allows commonly-used functions to be used without specifying the 
// library in which that function is defined. For instance, this line allows
// the use of "cout" rather than the full specification "cout"
using namespace std;

int treeLevelMaximum = 0; 
int treeLevelCounter = 0; 
int wordsInserted = 0;

//--------------------------------------------------------------------------
class BinarySearchTreeNode{
	public:
		BinarySearchTreeNode( ) // Constructor. Any setup operation you wish for the class.
        {
 
        } // end constructor
		BinarySearchTreeNode(string d) // Constructor. Any setup operation you wish for the class.
        {
            data = d;
        } // end constructor

        string getData() { return data; }
		
		// ----------------------------------------------------------------
		// Precondition: There exists at least one node in the tree already;
		// the root node was created in main() as a special case. 
		// Outcome: If this data is not already in the tree, it is added as a new node.
		void insert(string d)  // Insert a new node into the tree.
		{
		
			if (treeLevelCounter++ > treeLevelMaximum) treeLevelMaximum++;
		
			// cout << "insert:   the data to insert is " << d << endl;

			if (data.compare(d) == 0) // This data is already in the tree.
			{
				// cout << "insert:    ---- data " << d << " is already in tree! " << endl;
				return;
			} 
			else if (data.compare(d) > 0 and left == NULL) // This data is a new left child
			{
				left = new BinarySearchTreeNode(d);
				//cout << "insert:  " << d <<  "   was inserted as a left child at level " << treeLevelCounter << endl;
				wordsInserted++;
			}  
			else if (data.compare(d) < 0 and right == NULL) // This data is a new right child
			{
				///////////// COMPLETE ME!
			}  
			else if (data.compare(d) > 0) // This data goes in the left subtree
			{
				///////////// COMPLETE ME!
			}  
			else if (data.compare(d) < 0) // This data goes in the right subtree
			{
				///////////// COMPLETE ME!
			}

		} // end insert()

		
		// ----------------------------------------------------------------
		// Print the contents of the tree.
		void  print()
		{
			if (left != NULL) (*left).print();  // left->print();
			cout << data << endl;
			if (right != NULL) (*right).print();  // right->print();
		}

		// ----------------------------------------------------------------
		// Graph the contents of the tree.
		void  graph(int leftX, int rightX, int y)
		{
			if (left != NULL) 
			{
				cout << "line " << ((leftX+rightX)/2) << " " << y << " " << ((leftX+((leftX+rightX)/2))/2) << " " << (y+40) << endl;
				///////////// COMPLETE ME! using a recursive call
			}
			
			cout << "text " << ((leftX+rightX)/2) << " " << y << " 10 " << data << endl;
			
			if (right != NULL) 
			{
				cout << "line " << ((leftX+rightX)/2) << " " << y << " " << ((rightX+((leftX+rightX)/2))/2) << " " << (y+40) << endl;
				///////////// COMPLETE ME! using a recursive call
			}
		} // end graph()

		
    private:
        string data;
        BinarySearchTreeNode  *left = NULL;
        BinarySearchTreeNode  *right = NULL;
}; // end class BinarySearchTreeNode
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------

// Pass arguments or parameters from command-line execution. argc is the count of
// those parameters, including the executable filename. argv[] is an array of the 
// parameters.
int main (int argc, char *argv[])
{
    string token;
    int data;
    ifstream fin;
	int wordsInFile = 0;

	
	BinarySearchTreeNode *root = NULL;

	
    // Check the number of arguments. Expected: filename of a file
    if (argc != 2)  // This check is often hardcoded
    {   // If failure in parameters, offer advice for correction
        cout << "\nThis program uses command-line argument.\n";
        cout << "Usage: a.exe <filename>\n";
        exit(0);
    }


    try  // All lines within this block are part of the same exception handler
    {
        fin.open(argv[1]);  // Open the file for reading
    } 
    catch (exception& ex) 
    {
        cout << ex.what();  // display standard explanation of the exception
        exit(0);  // exit the program
    }
            
   
    // Read from the file, one token at a time. If the type of token is known, it
    // can be read into a corresponding variable type, such as 
    //          in >> x;    // Read the first item into an integer variable x.
    //          in >> str;  // Read the next item into a string variable str.
            
    while (fin >> token)
    {
        // Do something with the token
        //cout << "token is " << token << endl;
		
		wordsInFile++;
		treeLevelCounter = 0;  // Reset this data's counter in the tree
	
		// Initial case: Nothing is yet in the tree. This data is the root.
		if (root == NULL) // 
		{
			root = new BinarySearchTreeNode(token);
		}
		else 
		{
			(*root).insert(token);  // root->insert(token);
		}
	
    } // end while
                
    fin.close();  // Close the file after use


	// (*root).print();  // root->print();
	
	/*
	cout << endl;
	cout << "comment Number of data in the input file is " << wordsInFile << endl;
	cout << "comment Number of data inserted in tree is " << wordsInserted << endl;
	cout << "comment Maximum depth of any data in tree is " << treeLevelMaximum << endl;
	*/
	
	cout << "800 600 lightgray" << endl;
	
	(*root).graph( 0,  800, 40);  // TBD hardcoded numbers
	
} // end main


