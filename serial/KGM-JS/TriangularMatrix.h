/* 
 * File:   TriangularMatrix.h
 */

#ifndef TRIANGULARMATRIX_H
#define	TRIANGULARMATRIX_H

class TriangularMatrix {
public:
	TriangularMatrix(int size);
	TriangularMatrix(const TriangularMatrix& orig);
	virtual ~TriangularMatrix();
	bool& operator() (unsigned row, unsigned col);
	bool operator() (unsigned row, unsigned col) const; 
	int size();
private:
	bool * flatArray;
	int arraySize;
	int emulatedSize;
	int flatIndex(int row, int col) const;
};

#endif	/* TRIANGULARMATRIX_H */

