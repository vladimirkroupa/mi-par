package cz.cvut.fit.par.kgm.statespace;

import java.util.List;

public interface StateSpaceNode<T> {

	int evaluate();
	
	List<StateSpaceNode<T>> expandStates();
	
	T state();
	
}
