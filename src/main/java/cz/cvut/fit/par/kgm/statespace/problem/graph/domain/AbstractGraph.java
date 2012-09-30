package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.List;

import com.google.common.collect.Lists;

public abstract class AbstractGraph implements UndirectedGraph {

	@Override
	public int maxDegree() {
		int max = 0;
		for (int vertexI = 0; vertexI < vertexCount(); vertexI++) {
			int degree = degreeOf(vertexI);
			if (degree > max) {
				max = degree;
			}
		}
		return max;
	}
	
	@Override
	public List<Edge> possibleEdges() {
		List<Edge> edges = Lists.newArrayList(); 
		
		for (int vertexFrom = 0; vertexFrom < vertexCount(); vertexFrom++) {
			for (int vertexTo = vertexFrom + 1; vertexTo < vertexCount(); vertexTo++) {
				if (! areConnected(vertexFrom, vertexTo)) {
					edges.add(new Edge(vertexFrom, vertexTo));
				}
			}
		}
		return edges;
	}


}