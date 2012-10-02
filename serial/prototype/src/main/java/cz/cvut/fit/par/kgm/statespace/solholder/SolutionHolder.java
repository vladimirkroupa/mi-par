package cz.cvut.fit.par.kgm.statespace.solholder;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

/**
 * Encapsulates operations on solution.
 *
 * @param <T>
 */
public interface SolutionHolder<T> {

	StateSpaceNode<T> bestSoFar();
	
	void replaceBest(StateSpaceNode<T> newBest);
	
	boolean exists();
	
}
