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
	
	boolean areConnected(int vertexIndex1, int vertexIndex2);
	
	UndirectedGraph connect(Edge edge);

	List<Edge> possibleEdges();
	
}
