package cz.cvut.fit.par.kgm.statespace.problem.graph;

import java.util.List;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.AbstractStateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Edge;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.ImmutableGraph;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public class SpanningTreeProblemNode extends AbstractStateSpaceNode<UndirectedGraph> {

	private final UndirectedGraph originalGraph;
	private final UndirectedGraph tree;
	
	// TODO: necitelne konstuktory
	public SpanningTreeProblemNode(UndirectedGraph originalGraph) {
		this.tree = new ImmutableGraph(originalGraph.vertexCount());
		this.originalGraph = originalGraph;
	}
	
	// TODO: necitelne konstuktory
	public SpanningTreeProblemNode(UndirectedGraph tree, UndirectedGraph originalGraph) {
		this.tree = tree;
		this.originalGraph = originalGraph;
	}

	@Override
	public int evaluate() {
		return tree.maxDegree();
	}

	@Override
	public List<StateSpaceNode<UndirectedGraph>> expandStates() {
		List<Edge> possibleEdges = tree.possibleEdges();
		List<StateSpaceNode<UndirectedGraph>> expanded = Lists.newArrayListWithExpectedSize(possibleEdges.size());
		
		for (Edge edge : possibleEdges) {
			if (originalGraph.contains(edge)) {
				UndirectedGraph newGraph = tree.connect(edge);
				expanded.add(new SpanningTreeProblemNode(newGraph, originalGraph));
			}
		}
		return expanded;
	}

	@Override
	public UndirectedGraph state() {
		return tree;
	}

	@Override
	public String toString() {
		return "Node with graph: " + System.lineSeparator() + tree.toString();
	}

}
