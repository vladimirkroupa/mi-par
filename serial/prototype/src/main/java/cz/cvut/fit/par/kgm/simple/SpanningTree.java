package cz.cvut.fit.par.kgm.simple;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class SpanningTree {

	private final List<Edge> edges;

	public SpanningTree() {
		edges = new ArrayList<>();
	}
	
	public SpanningTree(SpanningTree existing) {
		this();
		edges.addAll(existing.edges);
	}

	public void addEdge(Edge edge) {
		if (edges.contains(edge)) {
			throw new IllegalArgumentException("Tree already contains edge " + edge);
		}
		edges.add(edge);
	}

	public int edgeCount() {
		return edges.size();
	}
	
	public boolean contains(Edge edge) {
		for (Edge treeEdge : edges) {
			if (treeEdge.equals(edge)) {
				return true;
			}
		}
		return false;
	}
	
	public List<Edge> edges() {
		return Collections.unmodifiableList(edges);
	}
	
	public boolean containsOnlyOneVertexOf(Edge edge) {
		boolean vert1Found = false;
		boolean vert2Found = false;
				
		for (Edge treeEdge : edges) {
			if (treeEdge.contains(edge.vertex1)) {
				vert1Found = true;
			}
			if (treeEdge.contains(edge.vertex2)) {
				vert2Found = true;
			}
		}
		return vert1Found ^ vert2Found;
	}
	
	public boolean isSpanningTreeOf(UndirectedGraph graph) {
		// spanning tree of a graph must have |V| - 1 edges
		if (edges.size() != (graph.vertexCount() - 1)) {
			return false;
		}
		
		boolean[] verticesConnected = new boolean[graph.vertexCount()];
		// does not detect cycles
		for (int vertex = 0; vertex < graph.vertexCount(); vertex++) {
			for (Edge edge : edges) {
				if (vertex == edge.vertex1 || vertex == edge.vertex2) {
					verticesConnected[vertex] = true;
				}
			}
		}
		
		for (int i = 0; i < verticesConnected.length; i++) {
			if (! verticesConnected[i]) {
				return false;
			}
		}
		return true;
	}
	
	
	@Override
	public String toString() {
		StringBuffer result = new StringBuffer();
		for (Edge e : edges) {
			result.append(e);
			result.append(System.lineSeparator());
		}
		return result.toString();
	}
	
	
	
}
