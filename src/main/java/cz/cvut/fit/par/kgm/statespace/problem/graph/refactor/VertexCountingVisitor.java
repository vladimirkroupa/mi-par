package cz.cvut.fit.par.kgm.statespace.problem.graph.refactor;

import java.util.Set;

import com.google.common.collect.Sets;

import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public class VertexCountingVisitor extends TreeDFSVisitor {

	private int vertexCount = 0;
	// TODO: to sem nepatri
	private boolean cycle = false;
	
	private Set<Integer> visited = Sets.newHashSet();
	
	@Override
	boolean visit(int vertex, UndirectedGraph tree) {
		if (visited.contains(vertex)) {
			cycle = true;
			return true;
		}
		vertexCount++;
		return false;
	}
	
	public int result() {
		return vertexCount;
	}
	
	public boolean cycleFound() {
		return cycle;
	}

}
