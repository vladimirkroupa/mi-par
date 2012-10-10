package cz.cvut.fit.par.kgm.simple;

import java.util.ArrayList;
import java.util.List;


public class UndirectedGraph {

	private final boolean[][] adjacencyMatrix;
	
	public UndirectedGraph(GraphBuilder graphBuilder) {
		this.adjacencyMatrix = graphBuilder.adjacencyMatrix;
	}
		
	public int vertexCount() {
		return adjacencyMatrix.length;
	}
	
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

	public boolean contains(Edge edge) {
		return areConnected(edge.vertex1, edge.vertex2);
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
	
	public List<Edge> edgeCandidates(SpanningTree tree) {
		boolean[] vertices = new boolean[vertexCount()];
		for (Edge edge : tree.edges()) {
			vertices[edge.vertex1] = true;
			vertices[edge.vertex2] = true;
		}
		throw new UnsupportedOperationException("TODO");
	}

	public List<Edge> edges() {
		List<Edge> edges = new ArrayList<>();
		for (int vert1 = 0; vert1 < vertexCount(); vert1++) {
			for (int vert2 = vert1 + 1; vert2 < vertexCount(); vert2++) {
				if (areConnected(vert1, vert2)) {
					edges.add(new Edge(vert1, vert2));
				}
			}
		}
		return edges;
	}
	
	
}
