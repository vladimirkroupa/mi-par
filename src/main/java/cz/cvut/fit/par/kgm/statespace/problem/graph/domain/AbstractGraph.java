package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

public abstract class AbstractGraph implements Graph {

	@Override
	public int maxDegree() {
		int max = 0;
		for (int vertexI = 0; vertexI < vertexCount(); vertexI++) {
			int degree = degreeOf(vertexI);
			if (degree > max) {
				max = degree;
			}
		}
		return max;
	}
	
	@Override
	public boolean isSpanningTreeOf(Graph graph) {
		throw new UnsupportedOperationException("Not implemented yet.");
	}

}