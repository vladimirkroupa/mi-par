package cz.cvut.fit.par.kgm.statespace.solver;

import com.google.common.base.Optional;
import com.google.common.base.Preconditions;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;
import cz.cvut.fit.par.kgm.statespace.referee.SolutionReferee;
import cz.cvut.fit.par.kgm.statespace.solholder.SolutionHolder;
import cz.cvut.fit.par.kgm.statespace.stack.StateStack;

public class DFSSolver<T> {

	private final SolutionReferee<T> referee;
	private final SolutionHolder<T> solutionHolder;
	private final StateStack<T> open;

	public DFSSolver(SolutionReferee<T> referee, SolutionHolder<T> solutionHolder, StateStack<T> stack) {
		this.referee = referee;
		this.solutionHolder = solutionHolder;
		this.open = stack;
	}

	public void setInitialState(StateSpaceNode<T> initialState) {
		open.push(initialState);
	}
	
	public Optional<StateSpaceNode<T>> findBestSolution() {
		Preconditions.checkArgument(! open.isEmpty());

		while (! open.isEmpty()) {
			StateSpaceNode<T> current = open.pop();
			System.out.println("current graph: ");
			System.out.println(current.state());
			System.out.println("---------------");
			if (referee.isSolution(current)) {
				if (referee.isBestPossible(current)) {
					return Optional.of(current);
				}
				if (! solutionHolder.exists() || referee.betterThan(current, solutionHolder.bestSoFar())) {
					solutionHolder.replaceBest(current);
				}
			} else {
				for (StateSpaceNode<T> expanded : current.expandStates()) {
					if (! solutionHolder.exists() || referee.betterThan(expanded, solutionHolder.bestSoFar())) {
						open.push(expanded);
					}
				}
			}
		}
		if (solutionHolder.exists()) {
			return Optional.of(solutionHolder.bestSoFar());
		} else {
			return Optional.absent();
		}
	}
	
}
