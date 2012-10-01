package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.List;


/**
 * Undirected graph.
 *
 */
public interface UndirectedGraph {

	int vertexCount();
	
	int degreeOf(int vertexIndex);
	
	int maxDegree();

	List<Integer> adjacent(int vertex);
	
	boolean areConnected(int vertexIndex1, int vertexIndex2);
	
	boolean contains(Edge edge);
	
	UndirectedGraph connect(Edge edge);

	List<Edge> possibleEdges();
	
	boolean isSpanningTreeOf(UndirectedGraph graph);

}
