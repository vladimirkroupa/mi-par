package cz.cvut.fit.par.kgm.statespace.solver;

import static org.fest.assertions.api.Assertions.assertThat;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import com.google.common.collect.Lists;

import cz.cvut.fit.par.kgm.statespace.problem.graph.SpanningTreeProblemNode;
import cz.cvut.fit.par.kgm.statespace.problem.graph.SpanningTreeProblemReferee;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.ImmutableGraph;
import cz.cvut.fit.par.kgm.statespace.problem.graph.domain.UndirectedGraph;
import cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee;
import cz.cvut.fit.par.kgm.statespace.solholder.SimpleSolutionHolder;
import cz.cvut.fit.par.kgm.statespace.solholder.SolutionHolder;
import cz.cvut.fit.par.kgm.statespace.stack.StateStack;

public class DFSSolverTest {

private DFSSolver<UndirectedGraph> testObject;
	
	private static final UndirectedGraph INPUT_GRAPH;
	
	static {
		String problem = 	
				"4\n" +
				"0101\n" +
				"1011\n" +
				"0100\n" +
				"1100"; 	
		INPUT_GRAPH = new ImmutableGraph(new ImmutableGraph.Builder(problem));
	}		
	

	@Before
	public void init() {
		final SolutionReferee<UndirectedGraph> referee = new SpanningTreeProblemReferee(INPUT_GRAPH);
		final SolutionHolder<UndirectedGraph> solutionHolder = new SimpleSolutionHolder<UndirectedGraph>();
		final StateStack<UndirectedGraph> stack = new StateStack<UndirectedGraph>();
		
		testObject = new DFSSolver<UndirectedGraph>(referee, solutionHolder, stack);
		testObject.setInitialState(new SpanningTreeProblemNode(INPUT_GRAPH));
	}
	
	@Test
	public void findBestSolution() {
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
		UndirectedGraph exp2Graph = new ImmutableGraph(new ImmutableGraph.Builder(exp2));
		List<UndirectedGraph> possible = Lists.newArrayList(exp1Graph, exp2Graph);
		
		UndirectedGraph actual = testObject.findBestSolution().get().state();
		assertThat(possible).contains(actual);
	}
	
}
