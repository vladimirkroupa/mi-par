package cz.cvut.fit.par.kgm.simple;

public class TriangularMatrix implements Matrix {

	private final int emulatedSize;
	private boolean[] array;
	
	public TriangularMatrix(int size) {
		this.emulatedSize = size;
		int arraySize = size * (size + 1) / 2;
		this.array = new boolean[arraySize];
	}

	private TriangularMatrix(TriangularMatrix original) {
		boolean[] arrCopy = new boolean[original.array.length];
		System.arraycopy(original.array, 0, arrCopy, 0, original.array.length);
		this.array = arrCopy;
		this.emulatedSize = original.emulatedSize;
	}
	
	@Override
	public boolean get(int row, int col) {
		return array[index(row, col)];
	}

	@Override
	public void set(int row, int col, boolean value) {
		array[index(row, col)] = value;
	}
	
	private int index(int row, int col) {
		return emulatedSize * row + col - (row * (row + 1) / 2);
	}

	@Override
	public int size() {
		return emulatedSize;
	}

	@Override
	public TriangularMatrix clone() {
		return new TriangularMatrix(this);
	}

}
