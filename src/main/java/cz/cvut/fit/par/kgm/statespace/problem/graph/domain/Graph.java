package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

/**
 * Undirected graph.
 *
 */
public interface Graph {

	int vertexCount();
	
	int degreeOf(int vertexIndex);
	
	int maxDegree();
	
	boolean areConnected(int vertexIndex1, int vertexIndex2);
	
	Graph connect(int vertexIndex1, int vertexIndex2);
	
	boolean isSpanningTreeOf(Graph graph);
	
}
