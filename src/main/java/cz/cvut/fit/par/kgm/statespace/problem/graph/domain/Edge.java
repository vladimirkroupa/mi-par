package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import com.google.common.base.Objects;

/**
 * Represents connection of two vertices;
 *
 */
public class Edge {
	
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
		return Objects.hashCode(vertex1, vertex2);
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

	@Override
	public String toString() {
		return String.format("(%d, %d)", vertex1, vertex2);
	}
	
}