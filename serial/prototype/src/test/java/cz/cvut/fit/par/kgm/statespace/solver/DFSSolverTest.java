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
		INPUT_GRAPH = new ImmutableGraph(
				new ImmutableGraph.Builder(4)
				.withEdge(0, 1).withEdge(0, 3).withEdge(1, 2).withEdge(1, 3)
				);
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
		UndirectedGraph exp1Graph = new ImmutableGraph(
				new ImmutableGraph.Builder(4)
				.withEdge(0, 1).withEdge(0, 3).withEdge(1, 2)
				);
		UndirectedGraph exp2Graph = new ImmutableGraph(
				new ImmutableGraph.Builder(4)
				.withEdge(0, 3).withEdge(1, 2).withEdge(1, 3)
				);
		UndirectedGraph exp3Graph = new ImmutableGraph(
				new ImmutableGraph.Builder(4)
				.withEdge(1, 0).withEdge(1, 3).withEdge(1, 2)
				);
		List<UndirectedGraph> possible = Lists.newArrayList(exp1Graph, exp2Graph, exp3Graph);
		
		UndirectedGraph actual = testObject.findBestSolution().get().state();
		assertThat(possible).contains(actual);
	}
	
}
