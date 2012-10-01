package cz.cvut.fit.par.kgm.statespace.problem.graph;

import static junit.framework.Assert.assertEquals;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.ImmutableGraph;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;

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
		testObject = new SpanningTreeNode(graph);
	}
	
	@Test
	public void evaluate() {
		int expected = 3;
		int actual = testObject.evaluate();
		assertEquals(expected, actual);
	}

	@Test
	public void expandStates() {
		String exp1 = 	
				"4\n" +
				"0111\n" +
				"1011\n" +
				"1100\n" +
				"1100";
		String exp2 = 	
				"4\n" +
				"0101\n" +
				"1011\n" +
				"0101\n" +
				"1110";		
		UndirectedGraph exp1Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp1));
		StateSpaceNode<UndirectedGraph> expected1 = new SpanningTreeNode(exp1Graph);  		
		UndirectedGraph exp2Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp2));
		StateSpaceNode<UndirectedGraph> expected2 = new SpanningTreeNode(exp2Graph);
		
		@SuppressWarnings("unchecked")
		List<StateSpaceNode<UndirectedGraph>> expected = Lists.newArrayList(expected1, expected2);		
		List<StateSpaceNode<UndirectedGraph>> actual = testObject.expandStates();
		assertEquals(expected, actual);
	}
	
}
