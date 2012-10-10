package cz.cvut.fit.par.kgm.simple;

import java.util.ArrayList;
import java.util.List;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;
import cz.cvut.fit.par.kgm.common.SimpleStack;

public class DFSSolver {

	static final int MIN_PRICE_POSSIBLE = 2;
	
	private final SimpleStack<SpanningTree> open;
	
	private final UndirectedGraph graph;
	
	private SpanningTree best = null;
	private int bestPrice = Integer.MAX_VALUE;
	
	public DFSSolver(UndirectedGraph graph) {
		this.graph = graph;
		open = new DequeueStackAdapter<>();
	}

	public SpanningTree findBestSolution() {
		for (SpanningTree state : initialStates()) {
			open.push(state);			
		}

		while (! open.isEmpty()) {
			SpanningTree current = open.pop();

			System.out.println("current tree:");
			System.out.println(current);
			System.out.println("---------------");
			
			if (isSolution(current)) {
				if (isBestPossible(current)) {
					return current;
				}
				if (! solutionExists() || isBestSoFar(current)) {
					storeBest(current);
				}
			} else {
				List<SpanningTree> expandedStates = expand(current);
				for (SpanningTree newState : expandedStates) {
					if (! solutionExists() || isBestSoFar(newState)) {
						open.push(newState);
					}
				}
			}
		}
		if (solutionExists()) {
			return best;
		}
		return null;
	}
	
	List<SpanningTree> initialStates() {
		// TODO
		List<SpanningTree> result = new ArrayList<>();
		SpanningTree tree = new SpanningTree();
		tree.addEdge(graph.edges().get(0));
		result.add(tree);
		return result;
	}
	
	List<SpanningTree> expand(SpanningTree state) {
		List<SpanningTree> result = new ArrayList<>();
		List<Edge> possibleEdges = edgeCandidates(state, graph);
		for (Edge edge : possibleEdges) {
			SpanningTree newTree = new SpanningTree(state);
			newTree.addEdge(edge);
			result.add(newTree);
		}
		return result;
	}
	
	List<Edge> edgeCandidates(SpanningTree tree, UndirectedGraph graph) {
		List<Edge> candidates = new ArrayList<>();
		for (Edge edge : graph.edges()) {
			if (tree.containsOnlyOneVertexOf(edge) && ! candidates.contains(edge)) {
				candidates.add(edge);
			}
		}
		return candidates;
	}

	// Spanning tree
	
	int evaluate(SpanningTree tree) {
		return maxDegreeOf(tree);
	}
	
	int maxDegreeOf(SpanningTree tree) {
		int[] vertexDegrees = new int[graph.vertexCount()];
		for (Edge edge : tree.edges()) {
			vertexDegrees[edge.vertex1]++;
			vertexDegrees[edge.vertex2]++;
		}
		
		int maxDegree = 0;
		for (int degree : vertexDegrees) {
			if (degree > maxDegree) {
				maxDegree = degree;
			}
		}
		return maxDegree;
	}
	
	// Solution manipulation
	
	boolean isSolution(SpanningTree candidate) {
		return candidate.isSpanningTreeOf(graph);
	}
	
	boolean isBestPossible(SpanningTree candidate) {
		return evaluate(candidate) == MIN_PRICE_POSSIBLE;
	}
	
	boolean isBestSoFar(SpanningTree candidate) {
		return evaluate(candidate) < bestPrice;
	}
	
	void storeBest(SpanningTree newBest) {
		bestPrice = evaluate(newBest);
		best = newBest;
	}
	
	boolean solutionExists() {
		return best != null;
	}
	
}
