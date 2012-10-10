package cz.cvut.fit.par.kgm.simple;

import static junit.framework.Assert.assertTrue;
import static org.fest.assertions.api.Assertions.assertThat;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

public class DFSSolverTest {

	private DFSSolver testObject;
	
	private static final UndirectedGraph INPUT_GRAPH;
	
	static {
		INPUT_GRAPH = new UndirectedGraph(
				new GraphBuilder(4)
				.withEdge(0, 1).withEdge(0, 3).withEdge(1, 2).withEdge(1, 3)
				);
	}		
	
	@Before
	public void init() {
		testObject = new DFSSolver(INPUT_GRAPH);
	}
	
	@Test
	public void findBestSolution() {
		SpanningTree exp1 = new SpanningTree();
		exp1.addEdge(new Edge(0, 1));
		exp1.addEdge(new Edge(0, 3));
		exp1.addEdge(new Edge(1, 2));
		assertTrue(testObject.isSolution(exp1));
		
		SpanningTree exp2 = new SpanningTree();
		exp2.addEdge(new Edge(0, 1));
		exp2.addEdge(new Edge(1, 3));
		exp2.addEdge(new Edge(1, 2));
		assertTrue(testObject.isSolution(exp2));
		
		SpanningTree exp3 = new SpanningTree();
		exp3.addEdge(new Edge(0, 3));
		exp3.addEdge(new Edge(1, 2));
		exp3.addEdge(new Edge(1, 3));
		assertTrue(testObject.isSolution(exp3));
		
		List<SpanningTree> possible = new ArrayList<>();
		possible.addAll(Arrays.asList(exp1, exp2, exp3));
		
		SpanningTree actual = testObject.findBestSolution();
		assertThat(possible).contains(actual);
	}
	
}
