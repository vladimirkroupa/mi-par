package cz.cvut.fit.par.kgm.statespace;

import com.google.common.base.Objects;

public abstract class AbstractStateSpaceNode<T> implements StateSpaceNode<T> {

	@Override
	public int hashCode() {
		return Objects.hashCode(state());
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) return true;
		if (! (obj instanceof StateSpaceNode)) return false;
		@SuppressWarnings("unchecked")
		StateSpaceNode<T> other = (StateSpaceNode<T>) obj;
		return this.state().equals(other.state());
	}
	
}
