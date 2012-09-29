package cz.cvut.fit.par.kgm.statespace.stack;

import java.util.ArrayDeque;
import java.util.Deque;

import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

/**
 * Adapts Dequeue to simpler StateStack interface.
 *
 */
public class DequeueStateStackAdapter<T> implements StateStack<T> {

	private final Deque<StateSpaceNode<T>> dequeue;
	
	public DequeueStateStackAdapter() {
		this.dequeue = new ArrayDeque<>();
	}

	public DequeueStateStackAdapter(Deque<StateSpaceNode<T>> dequeue) {
		this.dequeue = dequeue;
	}

	@Override
	public void push(StateSpaceNode<T> state) {
		dequeue.addFirst(state);
	}
	
	@Override
	public StateSpaceNode<T> peek() {
		return dequeue.peekFirst();
	}

	@Override
	public StateSpaceNode<T> pop() {
		return dequeue.pop();
	}

	@Override
	public boolean isEmpty() {
		return dequeue.isEmpty();
	}

}
