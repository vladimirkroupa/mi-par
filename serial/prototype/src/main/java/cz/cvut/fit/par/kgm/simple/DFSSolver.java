package cz.cvut.fit.par.kgm.simple;

import java.util.List;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;
import cz.cvut.fit.par.kgm.common.SimpleArrayList;
import cz.cvut.fit.par.kgm.common.SimpleList;
import cz.cvut.fit.par.kgm.common.SimpleStack;

public class DFSSolver {

	static final int MIN_PRICE_POSSIBLE = 2;
	
	private final SimpleStack<Edge> stack;
	private final UndirectedGraph graph;
	
	private int[] vertexDegrees;
	private SimpleList<Edge> spanningTree;
	
	private SimpleList<Edge> best;
	private int bestPrice;
	
	public DFSSolver(UndirectedGraph graph) {
		this.graph = graph;
		
		stack = new DequeueStackAdapter<Edge>();
		bestPrice = Integer.MAX_VALUE;
		best = null;
		
		int vertices = graph.vertexCount();
		spanningTree = new SimpleArrayList<Edge>(vertices);
		vertexDegrees = new int[vertices];
	}

	public SimpleList<Edge> findBestSolution() {
		for (Edge edge : firstEdgeCandidates(graph)) {
			stack.push(edge);	
		}
		
		while (! stack.isEmpty()) {
			printStack(stack);
			Edge current = stack.pop();

			if (isBacktrackMarker(current)) {
				System.out.println("backtracking...");
				System.out.println();
				removeLastEdge(spanningTree);
			} else {
				addEdge(spanningTree, current);
				printVertexDegrees(vertexDegrees);
				int price = evaluate(vertexDegrees);
				printSpanningTree(spanningTree, price);
				if (isSolution(vertexDegrees)) {
					if (isBestPossible(price)) {
						return spanningTree;
					} else if (isBestSoFar(price)) {
						updateBest(spanningTree, price);
					}
					removeLastEdge(spanningTree);
				} else {
					pushBacktrackMarker(stack);
					List<Edge> candidates = graph.edgeCandidates(spanningTree, vertexDegrees);
					printCandidates(candidates);
					for (Edge edge : candidates) {
						if (possibleWinner(vertexDegrees, edge)) {
							stack.push(edge);
						} else {
							System.out.println("leaving out edge " + edge);
						}
					}
				}
			}
			System.out.println("---------------------");
		}
		if (solutionExists()) {
			return best;
		}
		return null;
	}
	
	void removeLastEdge(SimpleList<Edge> spanningTree) {
		Edge removed = spanningTree.removeLast();
		vertexDegrees[removed.vertex1]--;
		vertexDegrees[removed.vertex2]--;
	}
	
	void addEdge(SimpleList<Edge> spanningTree, Edge edge) {
		spanningTree.add(edge);
		vertexDegrees[edge.vertex1]++;
		vertexDegrees[edge.vertex2]++;
	}
	
	boolean isBacktrackMarker(Edge edge) {
		return edge instanceof BacktrackMarker;
	}
	
	void pushBacktrackMarker(SimpleStack<Edge> stack) {
		stack.push(new BacktrackMarker());
	}
	
	List<Edge> firstEdgeCandidates(UndirectedGraph graph) {
		int vertex = 0;
		return graph.edgesAdjacentTo(vertex);
	}
	
	// Spanning tree
	
	int evaluate(int[] vertexDegrees) {
		int max = vertexDegrees[0];
		for (int i = 0; i < vertexDegrees.length; i++) {
			if (vertexDegrees[i] > max) {
				max = vertexDegrees[i];
			}
		}
		return max;
	}
	
	int evaluate(int[] vertexDegrees, Edge current) {
		int max = vertexDegrees[0];
		for (int i = 0; i < vertexDegrees.length; i++) {
			int vertexDeg = vertexDegrees[i]; 
			if (current.vertex1 == i || current.vertex2 == i) {
				vertexDeg++;
			}
			if (vertexDeg > max) {
				max = vertexDeg;
			}
		}
		return max;
	}
	
	boolean possibleWinner(int[] vertexDegrees, Edge current) {
		int price = evaluate(vertexDegrees, current);
		return isBestSoFar(price);
	}
	
	// Solution manipulation
	
	boolean isSolution(int[] vertexDegrees) {
		int degreeSum = 0;
		for (int i = 0; i < vertexDegrees.length; i++) {
			degreeSum += vertexDegrees[i];
		}
		if (degreeSum %2 != 0) {
			throw new IllegalStateException();
		}
		int edges = degreeSum / 2;
		int vertices = vertexDegrees.length;
		return (edges ==  vertices - 1);
	}
	
	boolean isBestPossible(int price) {
		return price == MIN_PRICE_POSSIBLE;
	}
	
	boolean isBestSoFar(int price) {
		if (best == null) {
			return true;
		}
		return price < bestPrice;
	}
	
	void updateBest(SimpleList<Edge> spanningTree, int price) {
		bestPrice = price;
		best = new SimpleArrayList<Edge>((SimpleArrayList<Edge>)spanningTree);
	}
	
	boolean solutionExists() {
		return best != null;
	}
	
	void printSpanningTree(SimpleList<Edge> spanningTree, int price) {
		if (isSolution(vertexDegrees)) {
			System.out.println("spanning tree:");
		}
		for (int i = 0; i < spanningTree.size(); i++) {
			if (i != 0) {
				System.out.print(", ");
			}
			Edge edge = spanningTree.get(i);
			System.out.print(edge);
		}
		System.out.println();
		System.out.printf("price: %d", price);
		System.out.println();
		System.out.println();
	}
	
	void printStack(SimpleStack<Edge> stack) {
		System.out.println(stack);
		System.out.println();
	}
	
	void printCandidates(List<Edge> candidates) {
		System.out.print(candidates.size() + " possibilities: ");
		for (Edge edge : candidates) {
			System.out.print(edge);
			System.out.print(" ");
		}
		System.out.println();
	}
	
	void printVertexDegrees(int[] degrees) {
		for (int degree : degrees) {
			System.out.print(degree);
			System.out.print(" | ");
		}
		System.out.println();
	}
	
}
