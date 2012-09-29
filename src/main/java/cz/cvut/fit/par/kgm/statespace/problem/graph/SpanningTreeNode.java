package cz.cvut.fit.par.kgm.statespace.problem.graph;

import java.util.List;

import cz.cvut.fit.par.kgm.statespace.AbstractStateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Graph;

public class SpanningTreeNode extends AbstractStateSpaceNode<Graph> {

	private final Graph graph;
	
	public SpanningTreeNode(Graph graph) {
		this.graph = graph;
	}

	@Override
	public int evaluate() {
		return graph.maxDegree();
	}

	@Override
	public List<StateSpaceNode<Graph>> expandStates() {
		throw new UnsupportedOperationException("TODO");
	}

	@Override
	public Graph state() {
		return graph;
	}

}
