package cz.cvut.fit.par.kgm.statespace.solver;

import com.google.common.base.Optional;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee;
import cz.cvut.fit.par.kgm.statespace.solholder.SolutionHolder;
import cz.cvut.fit.par.kgm.statespace.stack.StateStack;

public class DFSSolver<T> {

	private final SolutionReferee<T> referee;
	private final SolutionHolder<T> solution;
	private final StateStack<T> open;

	public DFSSolver(SolutionReferee<T> referee, SolutionHolder<T> solution, StateStack<T> stack) {
		this.referee = referee;
		this.solution = solution;
		this.open = stack;
	}

	public Optional<StateSpaceNode<T>> findBestSolution2() {

		StateSpaceNode<T> initialState = null; // FUXME
		
		open.push(initialState);
		while (! open.isEmpty()) {
			StateSpaceNode<T> current = open.pop();
			if (referee.isSolution(current)) {
				if (referee.isBestPossible(current)) {
					return Optional.of(current);
				}
				if (referee.betterThan(current, solution.bestSoFar())) {
					solution.replaceBest(current);
				}
			} else {
				for (StateSpaceNode<T> expanded : current.expandStates()) {
					if (referee.betterThan(solution.bestSoFar(), expanded)) {
						open.push(expanded);
					}
				}
			}
		}
		if (solution.exists()) {
			return Optional.of(solution.bestSoFar());
		} else {
			return Optional.absent();
		}
	}
	
}
