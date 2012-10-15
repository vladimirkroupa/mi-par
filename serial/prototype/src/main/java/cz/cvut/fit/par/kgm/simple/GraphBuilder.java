package cz.cvut.fit.par.kgm.simple;

import java.util.Scanner;

import com.google.common.base.Preconditions;

// TODO: validovat, redukovat na trojuhel asi ne
//TODO: validovat i po zmenach grafu
public class GraphBuilder {
	
	public final boolean[][] adjacencyMatrix;
	
	public Matrix build() {
		return new StandardMatrix(adjacencyMatrix);
	}
	
	public GraphBuilder(String adjacencyMatrixString) {
		Scanner sc = new Scanner(adjacencyMatrixString);
		if (! sc.hasNextLine()) {
			sc.close();
			throw new IllegalArgumentException("Vertex count missing on first line.");
		}
		String firstLine = sc.nextLine();
		int vertexCount = Integer.parseInt(firstLine);
		adjacencyMatrix = new boolean[vertexCount][];
		for (int rowI = 0; rowI < vertexCount; rowI++) {
			if (! sc.hasNextLine()) {
				sc.close();
				throw new IllegalArgumentException("Adjacency matrix doesn't have number of rows expected.");
			}
			String line = sc.nextLine();
			if (line.trim().length() < vertexCount) {
				sc.close();
				throw new IllegalArgumentException("Adjacency matrix doesn't have number of columns expected.");
			}
			boolean[] adjMatrixRow = parseLine(line);
			adjacencyMatrix[rowI] = adjMatrixRow;
		}
		sc.close();
	}
	
	public GraphBuilder(int size) {
		adjacencyMatrix = new boolean[size][];
		for (int rowI = 0; rowI < size; rowI++) {
			adjacencyMatrix[rowI] = new boolean[size];
			for (int colI = 0; colI < size; colI++) {
				adjacencyMatrix[rowI][colI] = false;
			}
		}
	}
	
	public GraphBuilder withEdge(int vertex1, int vertex2) {
		Preconditions.checkArgument(vertex1 < adjacencyMatrix.length);
		Preconditions.checkArgument(vertex2 < adjacencyMatrix.length);
		
		adjacencyMatrix[vertex1][vertex2] = true;
		adjacencyMatrix[vertex2][vertex1] = true;
		return this;
	}
	
	private boolean[] parseLine(String line) {
		boolean[] vector = new boolean[line.length()];
		for (int index = 0; index < line.length(); index++) {
			int number = Integer.parseInt(String.valueOf(line.charAt(index)));
			
			if (number == 1) {
				vector[index] = true;
			} else if (number == 0) {
				vector[index] = false;
			} else {
				throw new IllegalArgumentException("Encountered illegal number " + number + " in adjacency matrix");
			}
		}
		return vector;
	}
	
}