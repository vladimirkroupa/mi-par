package cz.cvut.fit.par.kgm.statespace.stack;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;
import cz.cvut.fit.par.kgm.common.SimpleStack;
import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

/**
 * Stack operating on state space nodes.
 *
 * @param <T> node content type
 */
public class StateStack<T> implements SimpleStack<StateSpaceNode<T>> {

	private SimpleStack<StateSpaceNode<T>> delegate; 
	
	public StateStack() {
		delegate = new DequeueStackAdapter<StateSpaceNode<T>>();
	}

	@Override
	public void push(StateSpaceNode<T> state) {
		delegate.push(state);
	}

	@Override
	public StateSpaceNode<T> peek() {
		return delegate.peek();
	}

	@Override
	public StateSpaceNode<T> pop() {
		return delegate.pop();
	}

	@Override
	public boolean isEmpty() {
		return delegate.isEmpty();
	}
	
}
