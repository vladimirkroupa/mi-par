package cz.cvut.fit.par.kgm.statespace.stack;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;

import java.util.NoSuchElementException;

import org.junit.Before;
import org.junit.Test;

import cz.cvut.fit.par.kgm.common.DequeueStackAdapter;

public class DequeueStackAdapterTest {

	private DequeueStackAdapter<String> testObject;
	
	@Before
	public void init() {
		testObject = new DequeueStackAdapter<String>();
	}
	
	@Test
	public void pushAndPop() {
		testObject.push("Hullo!");
		String actual = testObject.pop();
		assertEquals("Hullo!", actual);
	}
	
	@Test
	public void isEmpty() {
		assertTrue(testObject.isEmpty());
		testObject.push("Kapitan Pejsek");
		assertFalse(testObject.isEmpty());
		testObject.pop();
		assertTrue(testObject.isEmpty());
	}
	
	@Test(expected = NoSuchElementException.class)
	public void testEmptyPop() {
		testObject.pop();
	}
	
	@Test
	public void holdSeveral() {
		testObject.push("Kapitan Pejsek");
		testObject.push("Hullo!");
		testObject.push("Praseodym");
		assertEquals("Praseodym", testObject.pop());
		assertEquals("Hullo!", testObject.pop());
		assertEquals("Kapitan Pejsek", testObject.pop());
		assertTrue(testObject.isEmpty());
	}
	
}
