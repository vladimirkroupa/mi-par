/* 
 * File:   SquareMatrix.h
 */

#ifndef SQUAREMATRIX_H
#define	SQUAREMATRIX_H

class SquareMatrix {
public:
	SquareMatrix(int size);
	SquareMatrix(int size, bool * array);
	SquareMatrix(const SquareMatrix& orig);
	virtual ~SquareMatrix();
	bool& operator() (unsigned row, unsigned col);
	bool operator() (unsigned row, unsigned col) const; 
	int size();
	bool getMatrixElem(int i);
private:
	bool ** matrix;
	int matrixSize;
};

#endif	/* SQUAREMATRIX_H */

