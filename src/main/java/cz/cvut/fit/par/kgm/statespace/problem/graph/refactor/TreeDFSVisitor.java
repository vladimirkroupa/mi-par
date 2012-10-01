package cz.cvut.fit.par.kgm.statespace.problem.graph.refactor;

import java.util.List;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;
import cz.cvut.fit.par.kgm.common.SimpleStack;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public abstract class TreeDFSVisitor {

	public void goDFS(UndirectedGraph tree, int startingVertex) {
		if ( true ) {
			throw new UnsupportedOperationException("TODO: use node coloring!");
		}
		SimpleStack<Integer> open = new DequeueStackAdapter<Integer>();
		open.push(startingVertex);
		
		while (! open.isEmpty()) {
			int vertex = open.pop();
			boolean stop = visit(vertex, tree);
			if (stop) {
				return;
			}
			for (Integer child : children(tree, vertex) ) {
				open.push(child);
			}
		}
	}
	
	/**
	 * @param vertex
	 * @param tree
	 * @return true to abort visit, false to continue
	 */
	abstract boolean visit(int vertex, UndirectedGraph tree);

	private List<Integer> children(UndirectedGraph tree, int ofVertex) {
		List<Integer> children = Lists.newArrayList();
		
		for (int i = 0; i < tree.vertexCount(); i++) {
			if (tree.areConnected(ofVertex, i)) {
				children.add(i);
			}
		}
		return children;
	}
	
}
