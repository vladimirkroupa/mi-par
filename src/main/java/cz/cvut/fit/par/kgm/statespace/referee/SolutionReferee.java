package cz.cvut.fit.par.kgm.statespace.referee;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

public interface SolutionReferee<T> {
	
	boolean isSolution(StateSpaceNode<T> candidate);
	
	boolean isBestPossible(StateSpaceNode<T> candidate);
	
	boolean betterThan(StateSpaceNode<T> firstCompared, StateSpaceNode<T> secondCompared);
	
}
