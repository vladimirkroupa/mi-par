package cz.cvut.fit.par.kgm.common;

import junit.framework.Assert;

import org.junit.Before;
import org.junit.Test;



/**
 * Adapts Dequeue to simpler StateStack interface.
 *
 */
public class DequeueStackAdapterTest {

	private DequeueStackAdapter<Integer> testObject;
	
	@Before
	public void init() {
		testObject = new DequeueStackAdapter<>();
		testObject.push(1);
		testObject.push(2);
		testObject.push(3);
	}
	
	@Test
	public void toStringTest() {
		String expected = "| 3 |";
		expected += System.lineSeparator();
		expected += "| 2 |";
		expected += System.lineSeparator();
		expected += "| 1 |";
		expected += System.lineSeparator();
		
		String actual = testObject.toString();
		Assert.assertEquals(expected, actual);
	}

}
