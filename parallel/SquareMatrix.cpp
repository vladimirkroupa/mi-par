/* 
 * File:   SquareMatrix.cpp
 */

#include "SquareMatrix.h"
#include <algorithm>

SquareMatrix::SquareMatrix(int size) {
	this->matrixSize = size;
	this->matrix = new bool* [matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new bool[matrixSize];
		std::fill_n(matrix[i], matrixSize, 0);
	}
}

SquareMatrix::SquareMatrix(const SquareMatrix& orig) {
	this->matrixSize = orig.matrixSize;
	this->matrix = new bool* [matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new bool[matrixSize];
		std::copy(orig.matrix[i], orig.matrix[i] + matrixSize, matrix[i]);
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