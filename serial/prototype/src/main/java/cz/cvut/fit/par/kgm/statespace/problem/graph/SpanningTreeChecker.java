package cz.cvut.fit.par.kgm.statespace.problem.graph;

import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public interface SpanningTreeChecker {

	boolean isSpanningTreeOf(UndirectedGraph tree, UndirectedGraph graph);
	
}
