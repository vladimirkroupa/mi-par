/* 
 * File:   SquareMatrix.cpp
 */

#include "SquareMatrix.h"
#include <algorithm>

using namespace std;

SquareMatrix::SquareMatrix(int size) {
	this->matrixSize = size;
	this->matrix = new bool* [matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new bool[matrixSize];
		fill_n(matrix[i], matrixSize, 0);
	}
}

SquareMatrix::SquareMatrix(int size, int * array) {
	this->matrixSize = size;
	this->matrix = new bool* [matrixSize];
	int elem = 0;
	for (int i = 0; i < size; i++) {
		matrix[i] = new bool[matrixSize];
		for (int j = 0; j < size; j++) {
			matrix[i][j] = array[elem] > 0 ? 1 : 0;
			elem++;
		}
	}
}

SquareMatrix::SquareMatrix(const SquareMatrix& orig) {
	this->matrixSize = orig.matrixSize;
	this->matrix = new bool* [matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new bool[matrixSize];
		copy(orig.matrix[i], orig.matrix[i] + matrixSize, matrix[i]);
	}
}

SquareMatrix::~SquareMatrix() {
	for (int i = 0; i < matrixSize; i++) {
		delete [] matrix[i];
	}
	delete [] matrix;
}

bool SquareMatrix::operator ()(unsigned row, unsigned col) const {
	return matrix[row][col];
}

bool& SquareMatrix::operator ()(unsigned row, unsigned col) {
	return matrix[row][col];
}

int SquareMatrix::size() {
	return matrixSize;
}

bool SquareMatrix::getMatrixElem(int i) {
	int row = i / matrixSize;
	int col = i % matrixSize;
	return matrix[row][col];
}
