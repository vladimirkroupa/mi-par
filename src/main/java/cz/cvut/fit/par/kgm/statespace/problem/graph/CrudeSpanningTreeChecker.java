package cz.cvut.fit.par.kgm.statespace.problem.graph;

import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;
import cz.cvut.fit.par.kgm.statespace.problem.graph.refactor.VertexCountingVisitor;

public class CrudeSpanningTreeChecker implements SpanningTreeChecker {

	@Override
	public boolean isSpanningTreeOf(UndirectedGraph tree, UndirectedGraph graph) {
		VertexCountingVisitor visitor = new VertexCountingVisitor();
		visitor.goDFS(tree, 0); // start from 0
		int treeVertices = visitor.result();
		return (graph.vertexCount() == treeVertices);
	}

}
