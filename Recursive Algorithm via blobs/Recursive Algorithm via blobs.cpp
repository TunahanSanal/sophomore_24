#include <iostream>//Standart input-output
#include <fstream>//file protocols
#include <vector>//array typical process
#include <iomanip>//formatted output

using namespace std;

const int ROWS = 100, COLS = 100;
int number_Rows, number_Cols, count = 0;
bool passed[ROWS][COLS]; 
char net[ROWS][COLS];

struct Blob {
    double center_of_x;
    double center_of_y;
    int cout_pixel;
};

vector<Blob> blobs; //Definition of vector

int bx[] = {0, 0, -1, 1}; //directional ways of left and right
int by[] = {-1, 1, 0, 0}; //directional ways of up and down

//Need a validation control for bounds
bool validation(int x, int y) {
    return x >= 0 && x < number_Rows && y >= 0 && y < number_Cols && net[x][y] == 'x' && !passed[x][y];
}

//for analyzing the BLOBS that use the search function 
void search(int x, int y, int &cout_pixel, double &summation_X, double &summation_Y) {
    int i, newestX,newestY;
	passed[x][y] = true;
    cout_pixel++;
    summation_X += x;
    summation_Y += y;

    for (i = 0; i < 4; i++) {
        newestX = x + bx[i];
        newestY = y + by[i];
        if (validation(newestX, newestY)) {
            search(newestX, newestY, cout_pixel, summation_X, summation_Y);
        }
    }
}

//Every program must include the main function
main() {
    string name;
    int t,m;
    cout << "Enter the filename: ";
    cin >> name;

//Code's part of running blobs1.txt file
    ifstream inputFile(name.c_str()); 
    if (!inputFile) {
        cerr << "!!!ERROR!!!" << endl;
        return 1;
    }

//Determine the network of blobs1.txt
    inputFile >> number_Rows >> number_Cols; 

//Validation control for size    
    if (number_Rows > ROWS || number_Cols > COLS) {
        cerr << "!!!APPROACHED TO MAX SIZE FORM!!!" << endl;
        return 1;
    }

    
    inputFile.ignore();  //Before the next line,ignoring the some characters
    for (t = 0; t < number_Rows; t++) {
        inputFile.getline(net[t], COLS); 
        passed[t][0] = false; //indicate that there is an uncrossed part
        for (m = 0; m < number_Cols; m++) {
            passed[t][m] = false; //indicate that there is an uncrossed part
        }
    }

	//Close the blobs1.txt
    inputFile.close();

    //Printing the file as requested from us
    cout << "   01234567890123456789" << endl;
    cout << "  +--------------------+" << endl;
    for (t = 0; t < number_Rows; t++) {
        cout << t << " |";
        for (m = 0; m < number_Cols; m++) {
            cout << net[t][m];
        }
        cout << "|" << t << endl;
    }
    cout << "  +--------------------+" << endl;
    cout << "   01234567890123456789" << endl;


	//Scan and discover the blobs
    for (t = 0; t < number_Rows; t++) {
        for (m = 0; m < number_Cols; m++) {
            if (net[t][m] == 'x' && !passed[t][m]) {
                int cout_pixel = 0;
                double summation_X = 0, summation_Y = 0;
                
                search(t, m, cout_pixel, summation_X, summation_Y);
                Blob blob;
                blob.cout_pixel = cout_pixel;
                blob.center_of_x = summation_X / cout_pixel;
                blob.center_of_y = summation_Y / cout_pixel;
                blobs.push_back(blob); //with using push_back function, addition of blob to the list
            }
        }
    }

	//printing part of the code for fitting form
    cout << "\n" << endl;
    cout << "+------+------------+---------+-----------+";
    cout << "\n|\t\t\t\t\t  |" << endl;
    cout << "| BLOB | NoOfPixels | CoM Row | CoM Column|";
    cout << "\n|\t\t\t\t\t  |" << endl;
    cout << "+------+------------+---------+-----------+";
    cout << "\n" << endl;
    
    for (t = 0; t < blobs.size(); t++) {
        cout << "| " << setw(4) << t + 1 << " | " << setw(10) << blobs[t].cout_pixel << " | "
             << fixed << setprecision(2) << setw(7) << blobs[t].center_of_x << " | "
             << setw(9) << blobs[t].center_of_y << " |\n";
    }
    cout << "+------+------------+---------+-----------+";
    cout << "\n" << endl;
    
}

