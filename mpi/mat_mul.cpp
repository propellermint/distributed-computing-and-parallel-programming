#include <iostream>
#include <time.h>
#include <random>
#include <fstream>

using namespace std;




float RandomDouble(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

int N = 2000;

int main()
{   
    double **a, **b, **mult;   
    ofstream myfile;
    myfile.open ("matmul_out.txt");
    srand(time(NULL));
    a = new double*[N];
    b = new double*[N];
    int i, j, k;

    // Storing elements of first matrix.
    for(i = 0; i < N; ++i)
    {   a[i] = new double[N];
        for(j = 0; j < N; ++j)
        {
            //cout << "Enter element a" << i + 1 << j + 1 << " : ";
            //cin >> a[i][j];
            a[i][j] = RandomDouble(1, 5);
        }
    }

    // Storing elements of second matrix.
    //cout << endl << "Enter elements of matrix 2:" << endl;
    for(i = 0; i < N; ++i)
    {   b[i] = new double[N];
        for(j = 0; j < N; ++j)
        {
            //cout << "Enter element b" << i + 1 << j + 1 << " : ";
            //cin >> b[i][j];
            b[i][j] = RandomDouble(0, 1);
        }
    }

    // Initializing elements of matrix mult to 0.
    mult = new double*[N];
    for(i = 0; i < N; ++i)
    {   mult[i] = new double[N];
        for(j = 0; j < N; ++j)
        {
            mult[i][j]=0;
        }
    }

    // Multiplying matrix a and b and storing in array mult.
    for(i = 0; i < N; ++i)
        for(j = 0; j < N; ++j)
            for(k = 0; k < N; ++k)
            {
                mult[i][j] += a[i][k] * b[k][j];
            }

    // Displaying the multiplication of two matrix.
    //cout << endl << "Output Matrix: " << endl;
    for(i = 0; i < N; ++i)
    {
    for(j = 0; j < N; ++j)
    {
        //cout << " " << mult[i][j] ;
        myfile << " " << mult[i][j];
        //if(j == N - 1)
            //cout << endl;
    
    }
    myfile << endl;
    }
    myfile.close();
    return 0;
}