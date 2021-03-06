#include <iostream>
#include <ctime>
#include "MultCuda.cuh"
//colsA == rowsB

void matrMult(float* A, float* B, float* C, int rowsA, int colsA, int colsB)
{
	for (int i = 0; i < rowsA; ++i) {
		for (int j = 0; j < colsB; ++j) {
			for (int k = 0; k < colsA; ++k) {
				C[i*colsB + j] += A[i*colsA + k] * B[k*colsB + j];
			}
		}
	}
}

bool chechEq(float *A, float *B, int rows, int cols) 
{
	for (int i = 0; i < rows; ++i) {
		for (int j=0; j < cols; ++j) {
			if (abs(A[i*cols + j] - B[i*cols + j]) > 1) { 
				printf("%10.4d  %10.4d\n", A[i*cols + j], B[i*cols + j]);
				return false; }
		}
	}
	return true;
}

void fillMatr(float* M, int rows, int cols)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			M[i*cols + j] = rand() % 10 - 5;
		}
	}
}

void printMatr(float *M, int rows, int cols) 
{
	if (rows*cols<257) {
	printf("Matr:\n");
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				printf("%6.2f\t", M[i*cols + j]);
			}
			printf("\n");
		}
	}
}

int main(int argc, char** argv)
{
	int rowsA = atoi(argv[1]);
	int colsA = atoi(argv[2]);
	int colsB = atoi(argv[3]);

	//int rowsA = 320;
	//int colsA = 320;
	//int colsB = 320;



	float *A = new float[rowsA*colsA], *B = new float[colsA*colsB], *C = new float[rowsA * colsB],
	*Ad = NULL, *Bd = NULL, *Cd = NULL, *Ch = new float[rowsA*colsB], *Ch1 = new float[rowsA*colsB];
	fillMatr(A, rowsA, colsA);
	fillMatr(B, colsA, colsB);
	printMatr(A, rowsA, colsA);
	printMatr(B, colsA, colsB);
	time_t start = clock();
	printf("start cpu\n");
	for (int i = 0; i < rowsA*colsB; ++i) C[i] = 0;
	matrMult(A, B, C, rowsA, colsA, colsB);
	time_t end = clock();
	time_t cpu_time = (end - start) / CLOCKS_PER_SEC;
	printMatr(C, rowsA, colsB);
	printf("start gpuMult\n");
	gpuMatrMult(Ad,Bd, Cd, rowsA, colsA, colsB, A, B, Ch1);
	printf("start Mul\n");
	Mul(A, B, rowsA, colsA, colsB, Ch);
	printf(chechEq(C, Ch, rowsA, colsB) ? "eq\n" : "notEq\n");
	printf(chechEq(C, Ch1, rowsA, colsB) ? "eq\n" : "notEq\n");
	delete[]C; delete[]A; delete[]B; delete[]Cd; delete[]Ch; delete[]Ch1;





	printf("CPU_Time: %7.4f", cpu_time);

	return 0;
}
