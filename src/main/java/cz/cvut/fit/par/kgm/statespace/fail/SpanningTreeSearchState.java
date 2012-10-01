//package cz.cvut.fit.par.kgm.statespace.fail;
//
//import java.util.ArrayDeque;
//import java.util.List;
//import java.util.Queue;
//
//import com.google.common.collect.Lists;
//
//import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Color;
//import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Edge;
//import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.ImmutableGraph;
//import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;
//
//public class SpanningTreeSearchState {
//
//	private final UndirectedGraph original;
//	private final Color[] vertexColors;
//	private final UndirectedGraph tree;
//	private final Queue<Integer> gray;
//	
//	public SpanningTreeSearchState(UndirectedGraph original, int startingVertex) {
//		this.original = original;
//		int vertices = original.vertexCount();
//		this.tree = new ImmutableGraph(vertices);
//		this.gray = new ArrayDeque<Integer>(vertices);
//		this.vertexColors = new Color[vertices];
//		
//		for (int i = 0; i < vertices; i++) {
//			vertexColors[i] = Color.WHITE;
//		}
//		paintGray(startingVertex);
//		gray.add(startingVertex);
//	}
//	
//	private void paintGray(int vertex) {
//		vertexColors[vertex] = Color.GRAY;
//	}
//	
//	private void paintBlack(int vertex) {
//		vertexColors[vertex] = Color.BLACK;
//	}
//	
//	public List<SpanningTreeSearchState> nextStates() {
//		List<SpanningTreeSearchState> next = Lists.newArrayList();
//		Integer current = gray.poll();
//		for (Integer adj : original.adjacent(current)) {
//			if (vertexColors[adj] == Color.WHITE) {
//				SpanningTreeSearchState one = new SpanningTreeSearchState(this);
//				one.paintGray(adj);
//				one.gray.add(adj);
//				one.tree.connect(new Edge(current, adj));
//			}
//		}
//		paintBlack(current);
//	}
//	
//	private boolean hasFinished() {
//		for (Color color : vertexColors) {
//			if (color != Color.BLACK) {
//				return false;
//			}
//		}
//		return true;
//	}
//	
//}
