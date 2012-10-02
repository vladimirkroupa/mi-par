package cz.cvut.fit.par.kgm.statespace.problem.graph.refactor;

import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public class VertexCountingVisitor extends TreeDFSVisitor {

	private int vertexCount = 0;
	
	@Override
	boolean visit(int vertex, UndirectedGraph tree) {
		vertexCount++;
		return false; // do not stop
	}
	
	public int result() {
		return vertexCount;
	}
	
}
