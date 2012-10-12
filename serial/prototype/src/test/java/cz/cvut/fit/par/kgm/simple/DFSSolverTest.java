package cz.cvut.fit.par.kgm.simple;

import static org.fest.assertions.api.Assertions.assertThat;

import java.util.ArrayList;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

public class DFSSolverTest {

	private DFSSolver testObject;
	
	private static final UndirectedGraph INPUT_GRAPH;
	
	static {
		INPUT_GRAPH = new UndirectedGraph(
				new GraphBuilder(7)
				.withEdge(0, 1).withEdge(1, 2).withEdge(1, 3).withEdge(0, 4).withEdge(1, 4)
				.withEdge(4, 5).withEdge(4, 6)
				);
	}		
	
	@Before
	public void init() {
		testObject = new DFSSolver(INPUT_GRAPH);
	}
	
	@Test
	public void findBestSolution() {
		List<Edge> expected = new ArrayList<Edge>();
		expected.add(new Edge(1, 2));
		expected.add(new Edge(1, 3));
		expected.add(new Edge(4, 5));
		expected.add(new Edge(4, 6));
		
		expected.add(new Edge(0, 1));
		expected.add(new Edge(0, 4));
		
		
		List<Edge> actual = testObject.findBestSolution().asList();
		assertThat(actual).isEqualTo(expected);
	}
	
}
