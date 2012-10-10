package cz.cvut.fit.par.kgm.simple;


public class Edge {

	public final int vertex1;
	public final int vertex2;
	
	public Edge(int vertex1, int vertex2) {
		this.vertex1 = vertex1;
		this.vertex2 = vertex2;
	}

	public boolean contains(int vertex) {
		return vertex1 == vertex || vertex2 == vertex;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + vertex1;
		result = prime * result + vertex2;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Edge other = (Edge) obj;
		if (vertex1 != other.vertex1)
			return false;
		if (vertex2 != other.vertex2)
			return false;
		return true;
	}

	@Override
	public String toString() {
		return String.format("(%d, %d)", vertex1, vertex2);
	}
	
	
}
