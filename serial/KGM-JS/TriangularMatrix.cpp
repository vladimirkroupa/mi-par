/* 
 * File:   TriangularMatrix.cpp
 */

#include "TriangularMatrix.h"
#include <algorithm>

TriangularMatrix::TriangularMatrix(int size) {
	this->emulatedSize = size;
	this->arraySize = size * (size + 1) / 2;
	this->flatArray = new bool[arraySize];
	std::fill_n(flatArray, arraySize, 0);
}

TriangularMatrix::TriangularMatrix(const TriangularMatrix& orig) {
	this->emulatedSize = orig.emulatedSize;
	this->arraySize = orig.arraySize;
	bool * arrayCopy = new bool[arraySize];
	std::copy(orig.flatArray, orig.flatArray + orig.arraySize, arrayCopy);
	this->flatArray = arrayCopy;
}

TriangularMatrix::~TriangularMatrix() {
	delete [] flatArray;
}

bool TriangularMatrix::operator ()(unsigned row, unsigned col) const {
	int newIndex = flatIndex(row, col);
	return flatArray[newIndex];
}

bool& TriangularMatrix::operator ()(unsigned row, unsigned col) {
	int newIndex = flatIndex(row, col);
	return flatArray[newIndex];
}

int TriangularMatrix::flatIndex(int row, int col) const {
	return emulatedSize * row + col - (row * (row + 1) / 2);
}

int TriangularMatrix::size() {
	return emulatedSize;
}