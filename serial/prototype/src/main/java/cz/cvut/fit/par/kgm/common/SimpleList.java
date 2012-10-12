package cz.cvut.fit.par.kgm.common;

import java.util.List;

public interface SimpleList<T> {

	int size();
	
	T get(int index);
	
	void add(T element);
	
	T removeLast();
	
	List<T> asList();
	
}
