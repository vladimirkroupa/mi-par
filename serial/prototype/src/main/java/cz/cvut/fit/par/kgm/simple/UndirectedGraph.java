package cz.cvut.fit.par.kgm.simple;

import java.util.ArrayList;
import java.util.List;

import cz.cvut.fit.par.kgm.common.SimpleList;


public class UndirectedGraph {

	private final Matrix adjacencyMatrix;
	
	public UndirectedGraph(GraphBuilder graphBuilder) {
		this.adjacencyMatrix = graphBuilder.build();
	}
		
	public int vertexCount() {
		return adjacencyMatrix.size();
	}
	
	public List<Edge> edgeCandidates(SimpleList<Edge> tree, int[] vertexDegrees) {
		Matrix adjacencyCopy = adjacencyMatrix.clone();
		for (int i = 0; i < tree.size(); i++) {
			Edge edge = tree.get(i);
			adjacencyCopy.set(edge.vertex1, edge.vertex2, false);
			adjacencyCopy.set(edge.vertex2, edge.vertex1, false);
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
				if (adjacencyCopy.get(vertexFrom, vertexTo) == true) {
					newEdges.add(new Edge(vertexFrom, vertexTo));
				}
			}
		}
		
		return newEdges;
	}
	
	public boolean areConnected(int vertexIndex1, int vertexIndex2) {
		return adjacencyMatrix.get(vertexIndex1, vertexIndex2); 
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
