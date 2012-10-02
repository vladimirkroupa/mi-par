package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import java.util.List;

import org.apache.commons.lang3.BooleanUtils;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.problem.graph.CrudeSpanningTreeChecker;
import cz.cvut.fit.par.kgm.statespace.problem.graph.SpanningTreeChecker;

public abstract class AbstractGraph implements UndirectedGraph {

	@Override
	public List<Integer> adjacent(int toVertex) {
		List<Integer> adjacent = Lists.newArrayList();
		for (int candidate = 0; candidate < vertexCount(); candidate++) {
			if (areConnected(candidate, toVertex)) {
				adjacent.add(candidate);
			}
		}
		return adjacent;
	}

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
	public boolean contains(Edge edge) {
		return areConnected(edge.vertex1, edge.vertex2);
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

	@Override
	public boolean isSpanningTreeOf(UndirectedGraph graph) {
		SpanningTreeChecker checker = new CrudeSpanningTreeChecker();
		return checker.isSpanningTreeOf(this, graph);
	}

	@Override
	public int hashCode() {
		List<Boolean> unique = Lists.newArrayList();
		for (int rowI = 0; rowI < vertexCount(); rowI++) {
			for (int colI = rowI; colI < vertexCount(); colI++) {
				unique.add(areConnected(rowI, colI));
			}
		}
		return unique.hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) return true;
		if (! (obj instanceof UndirectedGraph) ) return false;
		UndirectedGraph other = (UndirectedGraph) obj;
		for (int rowI = 0; rowI < vertexCount(); rowI++) {
			for (int colI = rowI; colI < vertexCount(); colI++) {
				boolean thisConnected = this.areConnected(rowI, colI);
				boolean otherConnected = other.areConnected(rowI, colI);
				if (thisConnected != otherConnected) {
					return false;
				}
			}
		}
		return true;
	}

	@Override
	public String toString() {
		//TODO: optimalizovat
		StringBuilder sb = new StringBuilder();
		for (int rowI = 0; rowI < vertexCount(); rowI++) {
			for (int colI = 0; colI < vertexCount(); colI++) {
				sb.append(BooleanUtils.toInteger(areConnected(rowI, colI)));
			}
			if (rowI != vertexCount() - 1) {
				sb.append(System.lineSeparator());
			}
		}
		return sb.toString();
	}
	
	
}