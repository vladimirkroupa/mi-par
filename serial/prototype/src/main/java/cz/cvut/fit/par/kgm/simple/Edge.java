package cz.cvut.fit.par.kgm.simple;


public class Edge {

	public final int vertex1;
	public final int vertex2;
	
	public Edge(int vertex1, int vertex2) {
		this.vertex1 = vertex1;
		this.vertex2 = vertex2;
	}

	@Override
	public String toString() {
		return String.format("(%d, %d)", vertex1, vertex2);
	}
	
}
