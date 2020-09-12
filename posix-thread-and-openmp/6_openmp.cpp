#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstdlib>
#include <omp.h>
using namespace std;


double RandomDouble(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

int NUM_THREADS = 8;

int main() {
    double **A, *b; 
    const int n = 2000; // размерность матрицы
    A = new double*[n]; // матрица коэффициентов
    //double sum, sum1;
    b = new double[n]; // вектор-столбец свободных членов
    srand(time(NULL));
    // Ввод столбца свободных членов
    cout << "b: ";

    omp_set_num_threads(NUM_THREADS);

    #pragma parallel omp for
    for (int i = 0; i < n; i++){
        b[i] = RandomDouble(1, 10);
        //cin >> b[i];
    }


    // Ввод матрицы A
    cout << "A: ";
    cout << endl;

    for (int i = 0; i < n; i++) {
        A[i] = new double[n];
        #pragma omp for
        for (int j = 0; j < n; j++){
            A[i][j] = RandomDouble(1, 10);
            //cin >> A[i][j];

        }
    }
    

    double **L, **U;

    L = new double*[n];
    U = new double*[n];

    #pragma omp for 
    for (int i = 0; i < n; i++){
    	L[i] = new double[n];
        U[i] = new double[n];
	}

    int i, j, k; 
    double sum;
    #pragma omp parallel for firstprivate (j, k, sum) lastprivate(i) shared(A, L, U)
    //передача переменных команде pragma для дальнейшей успешной работы
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            U[0][i] = A[0][i];
            L[i][0] = A[i][0] / U[0][0];
            double sum = 0.;
            // единственное, что мы можем ускорить это вычисление суммы -
                //директивой "reduction"
            #pragma parallel omp for reduction(+:sum)
            for (int k = 0; k < i; k++){
                sum += L[i][k] * U[k][j];
            }
            U[i][j] = A[i][j] - sum;
            
            if (i > j){
                L[j][i] = 0;
            }
            else{
                sum = 0;
                
                #pragma parallel omp for reduction(+:sum)
                for (int k = 0; k < i; k++){
                    sum += L[j][k] * U[k][i];
                }
                L[j][i] = (A[j][i] - sum) / U[i][i];
                
            }
        }
    }
    // Печать матрицы U
    /*cout << endl;
    for (int i = 0; i < n; i++) { //перебор строк
        for (int j = 0; j < n; j++) { //вывод одной строки
        cout << U[i][j] << " ";
    }
    
    cout << endl; 
    }
    // Печать матрицы L
    cout << endl;
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) { 
        cout << L[i][j] << " ";
    }
    
    cout << endl; 
    }*/


// Объявление векторов и сумм для их вычисления
//double y[n];
//double x[n];
double *y, *x;
double sum2, sum3;
// Вычисление y
y = new double[n];
x = new double[n];
y[0] = b[0];

for (int h = 1; h < n; h++){
    sum2 = 0;
    #pragma omp for
    for (int p = 0; p < h; p++){
        sum2 += L[h][p] * y[p];
    }
    y[h] = b[h] - sum2;
}

// Вычисление x
x[n - 1] = y[n - 1] / U[n - 1][n - 1];

for (int i = 2; i <= n; i++){
    sum3 = 0;
    #pragma omp for
    for (int p = 0; p < i; p++){
        sum3 += U[n - i][n - p] * x[n - p];
    }
    x[n - i] = (1 / U[n - i][n - i])*(y[n - i] - sum3);
}


cout << endl;

// Вывод искомого вектора x
cout << "X: ";
for (int j = 0; j < n; j++){
    cout << x[j] << " ";
}



return 0;
}
