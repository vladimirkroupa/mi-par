package cz.cvut.fit.par.kgm.statespace.problem.graph;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;
import cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee;

public class SpanningTreeProblemReferee implements SolutionReferee<UndirectedGraph> {

	private final SpanningTreeChecker spanningTreeChecker;
	private UndirectedGraph originalGraph;
	
	static private final int MIN_PRICE_POSSIBLE = 2;
	
	public SpanningTreeProblemReferee(SpanningTreeChecker spanningTreeChecker) {
		this.spanningTreeChecker = spanningTreeChecker;
	}
	
	public void setOriginalGraph(UndirectedGraph originalGraph) {
		this.originalGraph = originalGraph;
	}

	@Override
	public boolean isSolution(StateSpaceNode<UndirectedGraph> candidate) {
		UndirectedGraph candidateTree = candidate.state(); 
		return spanningTreeChecker.isSpanningTreeOf(candidateTree, originalGraph);
	}

	@Override
	public boolean isBestPossible(StateSpaceNode<UndirectedGraph> candidate) {
		return (candidate.evaluate() == MIN_PRICE_POSSIBLE);
	}

	@Override
	public boolean betterThan(StateSpaceNode<UndirectedGraph> firstCompared, StateSpaceNode<UndirectedGraph> secondCompared) {
		return firstCompared.evaluate() < secondCompared.evaluate();
	}


}
