package cz.cvut.fit.par.kgm.simple;

import java.util.ArrayList;
import java.util.List;

import cz.cvut.fit.par.kgm.common.SimpleList;


public class UndirectedGraph {

	private final boolean[][] adjacencyMatrix;
	
	public UndirectedGraph(GraphBuilder graphBuilder) {
		this.adjacencyMatrix = graphBuilder.adjacencyMatrix;
	}
		
	public int vertexCount() {
		return adjacencyMatrix.length;
	}
	
	public List<Edge> edgeCandidates(SimpleList<Edge> tree, int[] vertexDegrees) {
		boolean[][] adjacencyCopy = copyArray(adjacencyMatrix);
		for (int i = 0; i < tree.size(); i++) {
			Edge edge = tree.get(i);
			adjacencyCopy[edge.vertex1][edge.vertex2] = false;
			adjacencyCopy[edge.vertex2][edge.vertex1] = false;
		}
		
		List<Edge> newEdges = new ArrayList<Edge>();
		for (int vertexFrom = 0; vertexFrom < vertexCount(); vertexFrom++) {
			if (vertexDegrees[vertexFrom] == 0) {
				continue;
			}
			for (int vertexTo = 0; vertexTo < vertexCount(); vertexTo++) {
				if (vertexDegrees[vertexTo] != 0) {
					continue;
				}
				if (adjacencyCopy[vertexFrom][vertexTo] == true) {
					newEdges.add(new Edge(vertexFrom, vertexTo));
				}
			}
		}
		
		return newEdges;
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
	
	public boolean areConnected(int vertexIndex1, int vertexIndex2) {
		return adjacencyMatrix[vertexIndex1][vertexIndex2]; 
	}
	
	public List<Edge> edgesAdjacentTo(int vertex) {
		List<Edge> edges = new ArrayList<>();
		for (int vertex2 = 0; vertex2 < vertexCount(); vertex2++) {
			if (areConnected(vertex, vertex2)) {
				edges.add(new Edge(vertex, vertex2));
			}
		}
		return edges;
	}
	
}
