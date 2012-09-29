package cz.cvut.fit.par.kgm.statespace.problem.graph;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Graph;
import cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee;

public class SpanningTreeProblemReferee implements cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee<Graph> {

	private final Graph originalGraph;
	
	static private final int MIN_PRICE_POSSIBLE = 2;
	
	public SpanningTreeProblemReferee(Graph originalGraph) {
		this.originalGraph = originalGraph;
	}

	@Override
	public boolean isSolution(StateSpaceNode<Graph> candidate) {
		Graph candidateGraph = candidate.state(); 
		return candidateGraph.isSpanningTreeOf(originalGraph);
	}

	@Override
	public boolean isBestPossible(StateSpaceNode<Graph> candidate) {
		return (candidate.evaluate() == MIN_PRICE_POSSIBLE);
	}

	@Override
	public boolean betterThan(StateSpaceNode<Graph> firstCompared, StateSpaceNode<Graph> secondCompared) {
		return firstCompared.evaluate() < secondCompared.evaluate();
	}


}
