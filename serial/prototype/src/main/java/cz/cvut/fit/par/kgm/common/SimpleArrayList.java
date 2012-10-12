package cz.cvut.fit.par.kgm.common;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SimpleArrayList<T> implements SimpleList<T> {

	private final Object[] array;
	private int nextPos;
	
	public SimpleArrayList(int capacity) {
		array = new Object[capacity];
		nextPos = 0;
	}

	public SimpleArrayList(SimpleArrayList<T> original) {
		this.array = Arrays.copyOf(original.array, original.array.length);
		this.nextPos = original.nextPos;
	}
	
	@Override
	public int size() {
		return nextPos;
	}

	@SuppressWarnings("unchecked")
	@Override
	public T get(int index) {
		if (index < 0 || index > array.length - 1) {
			throw new ArrayIndexOutOfBoundsException();
		}
		return (T)array[index];
	}

	@Override
	public void add(T element) {
		array[nextPos++] = element;
	}

	@SuppressWarnings("unchecked")
	@Override
	public T removeLast() {
		Object result = array[--nextPos];
		array[nextPos]= null;
		return (T)result;
	}

	@SuppressWarnings("unchecked")
	@Override
	public List<T> asList() {
		List<T> list = new ArrayList<T>();
		for (int i = 0; i < size(); i++) {
			list.add((T)array[i]);
		}
		return list;
	}

}
