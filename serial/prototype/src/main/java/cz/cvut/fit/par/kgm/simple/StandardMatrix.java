package cz.cvut.fit.par.kgm.simple;

public class StandardMatrix implements Matrix {

	private boolean[][] matrix;
	
	public StandardMatrix(boolean[][] matrix) {
		this.matrix = matrix;
	}

	@Override
	public boolean get(int row, int col) {
		return matrix[row][col];
	}

	@Override
	public void set(int row, int col, boolean value) {
		matrix[row][col] = value;
	}

	@Override
	public int size() {
		return matrix.length;
	}

	@Override
	public Matrix clone() {
		return new StandardMatrix(copyArray(matrix));
	}
	
	
	boolean[][] copyArray(boolean[][] original) {
		boolean[][] copy = new boolean[original.length][]; 
		for (int i = 0; i < original.length; i++) {
			copy[i] = new boolean[original.length];
			for (int j = 0; j < original.length; j++) {
				copy[i][j] = original[i][j];
			}
		}
		return copy;
	}
	
}
