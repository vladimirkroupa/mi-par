package cz.cvut.fit.par.kgm.statespace.problem.graph;

import static junit.framework.Assert.assertEquals;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.ImmutableGraph;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;
import static org.fest.assertions.api.Assertions.assertThat;

//TODO: aktualizovat
public class SpanningTreeNodeTest {

	private StateSpaceNode<UndirectedGraph> testObject;
	
	private static final String graphStr = 
			"4\n" +
			"0101\n" +
			"1011\n" +
			"0100\n" +
			"1100"; 

	@Before
	public void init() {
		ImmutableGraph.Builder builder = new ImmutableGraph.Builder(graphStr);
		UndirectedGraph graph = new ImmutableGraph(builder);
		testObject = new SpanningTreeProblemNode(graph);
	}
	
	@Test
	public void evaluate() {
		int expected = 0;
		int actual = testObject.evaluate();
		assertEquals(expected, actual);		
	}

	@Test
	public void expandStates() {
		String exp1 = 	
				"4\n" +
				"0100\n" +
				"1000\n" +
				"0000\n" +
				"0000";
		String exp2 = 	
				"4\n" +
				"0001\n" +
				"0000\n" +
				"0000\n" +
				"1000";	
		String exp3 = 	
				"4\n" +
				"0000\n" +
				"0001\n" +
				"0000\n" +
				"0100";
		String exp4 = 	
				"4\n" +
				"0000\n" +
				"0010\n" +
				"0100\n" +
				"0000";			
		
		UndirectedGraph exp1Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp1));
		StateSpaceNode<UndirectedGraph> expected1 = new SpanningTreeProblemNode(exp1Graph, exp1Graph);  		
		UndirectedGraph exp2Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp2));
		StateSpaceNode<UndirectedGraph> expected2 = new SpanningTreeProblemNode(exp2Graph, exp2Graph);
		UndirectedGraph exp3Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp3));
		StateSpaceNode<UndirectedGraph> expected3 = new SpanningTreeProblemNode(exp3Graph, exp3Graph);  		
		UndirectedGraph exp4Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp4));
		StateSpaceNode<UndirectedGraph> expected4 = new SpanningTreeProblemNode(exp4Graph, exp4Graph);		
		
		@SuppressWarnings("unchecked")
		List<StateSpaceNode<UndirectedGraph>> expected = Lists.newArrayList(expected1, expected2, expected3, expected4);		
		List<StateSpaceNode<UndirectedGraph>> actual = testObject.expandStates();
		assertThat(expected).containsAll(actual);
	}
	
}
