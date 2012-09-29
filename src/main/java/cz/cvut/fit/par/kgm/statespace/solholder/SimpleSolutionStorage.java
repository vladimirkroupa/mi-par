package cz.cvut.fit.par.kgm.statespace.solholder;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

public class SimpleSolutionStorage<T> implements SolutionHolder<T> {

	private StateSpaceNode<T> currentBestSolution;

	@Override
	public StateSpaceNode<T> bestSoFar() {
		return currentBestSolution;
	}

	@Override
	public void replaceBest(StateSpaceNode<T> newBest) {
		currentBestSolution = newBest;
	}

	@Override
	public boolean exists() {
		return currentBestSolution != null;
	}

}
