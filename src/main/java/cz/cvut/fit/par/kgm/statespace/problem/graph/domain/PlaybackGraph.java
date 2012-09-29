package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.List;

import com.google.common.collect.Lists;

public class PlaybackGraph extends AbstractGraph {

	private final UndirectedGraph startingGraph;
	private List<Edge> toApply;

	public PlaybackGraph(UndirectedGraph baseGraph) {
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
	public UndirectedGraph connect(Edge edge) {
		toApply.add(edge);
		return this;
	}
	
}
