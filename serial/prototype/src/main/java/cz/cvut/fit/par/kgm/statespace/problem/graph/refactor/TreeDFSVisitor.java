package cz.cvut.fit.par.kgm.statespace.problem.graph.refactor;

import java.util.List;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;
import cz.cvut.fit.par.kgm.common.SimpleStack;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.Color;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

public abstract class TreeDFSVisitor {

	private Color[] vertexColors;
	
	public void init(UndirectedGraph tree) {
		this.vertexColors = new Color[tree.vertexCount()];
		for (int i = 0; i < vertexColors.length; i++) {
			vertexColors[i] = Color.WHITE;
		}
	}

	public void goDFS(UndirectedGraph tree, int startingVertex) {
		init(tree);
		SimpleStack<Integer> gray = new DequeueStackAdapter<Integer>();
		gray.push(startingVertex);
		
		while (! gray.isEmpty()) {
			int vertex = gray.pop();
			boolean stop = visit(vertex, tree);
			if (stop) {
				return;
			}
			for (Integer child : children(tree, vertex) ) {
				if (vertexColors[child] == Color.WHITE) {
					vertexColors[child] = Color.GRAY;
					gray.push(child);
				}
			}
			vertexColors[vertex] = Color.BLACK;
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
