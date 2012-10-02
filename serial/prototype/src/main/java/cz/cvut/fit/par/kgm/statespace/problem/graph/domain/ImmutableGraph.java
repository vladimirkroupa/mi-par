package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.Scanner;


/**
 * Undirected graph backed by adjacency matrix.
 *
 */
//TODO: rozdelit interface na modifikace grafu a cteni?
public class ImmutableGraph extends AbstractGraph {

	private final boolean[][] adjacencyMatrix;
	
	public ImmutableGraph(Builder graphBuilder) {
		this.adjacencyMatrix = graphBuilder.adjacencyMatrix;
	}
	
	public ImmutableGraph(int size) {
		adjacencyMatrix = new boolean[size][];
		for (int rowI = 0; rowI < size; rowI++) {
			adjacencyMatrix[rowI] = new boolean[size];
			for (int colI = 0; colI < size; colI++) {
				adjacencyMatrix[rowI][colI] = false;
			}
		}
	}
	
	@Override
	public int vertexCount() {
		return adjacencyMatrix.length;
	}
	
	@Override
	public int degreeOf(int vertexIndex) {
		int rowI = vertexIndex;
		int degree = 0;
		for (int colI = 0; colI < vertexCount(); colI++) {
			if (adjacencyMatrix[rowI][colI]) {
				degree++;
			}
		}
		return degree;
	}

	@Override
	public boolean areConnected(int vertexIndex1, int vertexIndex2) {
		return adjacencyMatrix[vertexIndex1][vertexIndex2]; 
	}

	@Override
	public UndirectedGraph connect(Edge edge) {
		return new PlaybackGraph(this, edge);
	}

	// TODO: validovat, redukovat na trojuhel asi ne
	//TODO: validovat i po zmenach grafu
	public static class Builder {
		
		private final boolean[][] adjacencyMatrix;
		
		public Builder(String adjacencyMatrixString) {
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

}