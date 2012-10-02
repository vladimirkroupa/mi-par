package cz.cvut.fit.par.kgm.common;

public interface SimpleStack<T> {

	void push(T state);

	T peek();
		
	T pop(); 

	boolean isEmpty();
		
}
