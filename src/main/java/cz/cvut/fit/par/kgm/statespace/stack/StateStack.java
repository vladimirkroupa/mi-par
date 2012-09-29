package cz.cvut.fit.par.kgm.statespace.stack;

import cz.cvut.fit.par.kgm.common.SimpleStack;
import cz.cvut.fit.par.kgm.statespace.StateSpaceNode;

/**
 * Stack operating on state space nodes.
 *
 * @param <T> node content type
 */
public interface StateStack<T> extends SimpleStack<StateSpaceNode<T>> {

}
