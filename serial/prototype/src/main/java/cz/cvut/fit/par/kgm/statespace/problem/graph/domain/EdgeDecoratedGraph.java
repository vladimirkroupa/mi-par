package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;


public class EdgeDecoratedGraph extends AbstractGraph {

	private final UndirectedGraph startingGraph;
	private final Edge toApply;

	public EdgeDecoratedGraph(UndirectedGraph baseGraph, Edge toApply) {
		this.startingGraph = baseGraph;
		this.toApply = toApply;
	}

	@Override
	public int vertexCount() {
		return startingGraph.vertexCount();
	}

	@Override
	public int degreeOf(int vertexIndex) {
		int degreeFromOrig = startingGraph.degreeOf(vertexIndex);
		int degreeFromExtraEdge = 0;
		if (toApply != null) {
			degreeFromExtraEdge = toApply.connects(vertexIndex) ? 1 : 0;
		}
		return degreeFromOrig + degreeFromExtraEdge;
	}
	
	@Override
	public boolean areConnected(int vertexIndex1, int vertexIndex2) {
		Edge hypotheticalEdge = new Edge(vertexIndex1, vertexIndex2);
		if (hypotheticalEdge.equals(toApply)) {
			return true;
		}
		return startingGraph.areConnected(vertexIndex1, vertexIndex2);
	}

	@Override
	public UndirectedGraph connect(Edge edge) {
		return new EdgeDecoratedGraph(this, edge);
	}
	
}
