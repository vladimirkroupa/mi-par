package cz.cvut.fit.par.kgm.common;

import java.util.ArrayDeque;
import java.util.Deque;


/**
 * Adapts Dequeue to simpler StateStack interface.
 *
 */
public class DequeueStackAdapter<T> implements SimpleStack<T> {

	private final Deque<T> dequeue;
	
	public DequeueStackAdapter() {
		this.dequeue = new ArrayDeque<>();
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

	@Override
	public String toString() {
		StringBuilder accum = new StringBuilder();
		for (T elem : dequeue) {
			accum.append(String.format("| %d |", elem));
			accum.append(System.lineSeparator());
		}
		return accum.toString();
	}

}
