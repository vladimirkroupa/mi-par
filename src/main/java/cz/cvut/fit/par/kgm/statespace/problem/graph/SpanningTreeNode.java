package cz.cvut.fit.par.kgm.statespace.problem.graph;

import java.util.List;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.AbstractStateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Edge;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public class SpanningTreeNode extends AbstractStateSpaceNode<UndirectedGraph> {

	private final UndirectedGraph graph;
	
	public SpanningTreeNode(UndirectedGraph graph) {
		this.graph = graph;
	}

	@Override
	public int evaluate() {
		return graph.maxDegree();
	}

	@Override
	public List<StateSpaceNode<UndirectedGraph>> expandStates() {
		List<Edge> possibleEdges = graph.possibleEdges();
		List<StateSpaceNode<UndirectedGraph>> expanded = Lists.newArrayListWithExpectedSize(possibleEdges.size());
		
		for (Edge edge : possibleEdges) {
			UndirectedGraph newGraph = graph.connect(edge);
			expanded.add(new SpanningTreeNode(newGraph));
		}
		return expanded;
	}

	@Override
	public UndirectedGraph state() {
		return graph;
	}

	@Override
	public String toString() {
		return "Node with graph: " + graph.toString();
	}

}
