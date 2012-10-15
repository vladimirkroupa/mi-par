package cz.cvut.fit.par.kgm.simple;

public interface Matrix {

	boolean get(int row, int col);
	
	void set(int row, int col, boolean value);
	
	int size();
	
	Matrix clone();
	
}
