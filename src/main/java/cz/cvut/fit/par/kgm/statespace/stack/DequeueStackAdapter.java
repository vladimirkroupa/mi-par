package cz.cvut.fit.par.kgm.statespace.stack;

import java.util.ArrayDeque;
import java.util.Deque;

import cz.cvut.fit.par.kgm.common.SimpleStack;

/**
 * Adapts Dequeue to simpler StateStack interface.
 *
 */
public class DequeueStackAdapter<T> implements SimpleStack<T> {

	private final Deque<T> dequeue;
	
	public DequeueStackAdapter() {
		this.dequeue = new ArrayDeque<>();
	}

	public DequeueStackAdapter(Deque<T> dequeue) {
		this.dequeue = dequeue;
	}

	@Override
	public void push(T item) {
		dequeue.addFirst(item);
	}
	
	@Override
	public T peek() {
		return dequeue.peekFirst();
	}

	@Override
	public T pop() {
		return dequeue.pop();
	}

	@Override
	public boolean isEmpty() {
		return dequeue.isEmpty();
	}

}
