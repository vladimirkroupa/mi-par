package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.List;
import java.util.Objects;

import com.google.common.collect.Lists;

public class PlaybackGraph extends AbstractGraph {

	private final Graph startingGraph;
	private List<Edge> toApply;

	public PlaybackGraph(Graph baseGraph) {
		this.startingGraph = baseGraph;
		toApply = Lists.newArrayList();
	}

	@Override
	public int vertexCount() {
		return startingGraph.vertexCount();
	}

	@Override
	public int degreeOf(int vertexIndex) {
		int degreeFromOrig = startingGraph.degreeOf(vertexIndex);
		int degreeFromNew = 0;
		for (Edge edge : toApply) {
			if (edge.connects(vertexIndex)) {
				degreeFromNew++;
			}
		}
		return degreeFromOrig + degreeFromNew;
	}
	
	@Override
	public boolean areConnected(int vertexIndex1, int vertexIndex2) {
		Edge hypotheticalEdge = new Edge(vertexIndex1, vertexIndex2);
		for (Edge edge : toApply) {
			if (edge.equals(hypotheticalEdge)) {
				return true;
			}
		}
		return startingGraph.areConnected(vertexIndex1, vertexIndex2);
	}

	@Override
	public Graph connect(int vertexIndex1, int vertexIndex2) {
		toApply.add(new Edge(vertexIndex1, vertexIndex2));
		return this;
	}

	/**
	 * Represents connection of two vertices;
	 *
	 */
	static class Edge {
		
		public final int vertex1;
		public final int vertex2;

		public Edge(int vertex1, int vertex2) {
			this.vertex1 = vertex1;
			this.vertex2 = vertex2;
		}
	
		public boolean connects(int vertexIndex) {
			return vertex1 == vertexIndex || vertex2 == vertexIndex;
		}

		@Override
		public int hashCode() {
			return Objects.hash(vertex1, vertex2);
		}

		@Override
		public boolean equals(Object obj) {
			if (this == obj) return true;
			if (obj == null) return false;
			if (getClass() != obj.getClass()) return false;
			Edge other = (Edge) obj;
			if (vertex1 == other.vertex1 && vertex2 == other.vertex2) return true;
			if (vertex1 == other.vertex2 && vertex2 == other.vertex1) return true;
			return false;
		}
		
		
	}

	
}
