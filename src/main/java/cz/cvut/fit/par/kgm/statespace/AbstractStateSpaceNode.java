package cz.cvut.fit.par.kgm.statespace;

import java.util.Objects;

public abstract class AbstractStateSpaceNode<T> implements StateSpaceNode<T> {

	@Override
	public int hashCode() {
		return Objects.hash(state());
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) return true;
		if (obj == null) return false;
		if (getClass() != obj.getClass()) return false;
		@SuppressWarnings("unchecked")
		AbstractStateSpaceNode<T> other = (AbstractStateSpaceNode<T>) obj;
		return this.state().equals(other.state());
	}
	
}
