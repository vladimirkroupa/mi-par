package cz.cvut.fit.par.kgm.statespace.problem.graph.domain;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import com.google.common.collect.Lists;

public class ImmutableGraphTest {

	private UndirectedGraph testObject;
	
	private static final String graphStr = 
			"4\n" +
			"0101\n" +
			"1011\n" +
			"0100\n" +
			"1100"; 
	
	@Before
	public void init() {
		ImmutableGraph.Builder builder = new ImmutableGraph.Builder(graphStr);
		testObject = new ImmutableGraph(builder);
	}
	
	@Test
	public void vertexCount() {
		int expected = 4; 
		int actual = testObject.vertexCount();
		assertEquals(expected, actual);
	}
	
	@Test
	public void degreeOf() {
		int expected1 = 2;
		int expected2 = 3;
		int expected3 = 1;
		int expected4 = 2;
		int actual1 = testObject.degreeOf(0);
		int actual2 = testObject.degreeOf(1);
		int actual3 = testObject.degreeOf(2);
		int actual4 = testObject.degreeOf(3);
		assertEquals(expected1, actual1);
		assertEquals(expected2, actual2);
		assertEquals(expected3, actual3);
		assertEquals(expected4, actual4);
	}
	
	@Test
	public void maxDegree() {
		int expected = 3;
		int actual = testObject.maxDegree();
		assertEquals(expected, actual);
	}
	
	@Test
	public void areConnected() {
		assertFalse(testObject.areConnected(0, 0));
		assertTrue(testObject.areConnected(0, 1));
		assertFalse(testObject.areConnected(0, 2));
		assertTrue(testObject.areConnected(0, 3));
		
		assertTrue(testObject.areConnected(1, 0));
		assertFalse(testObject.areConnected(1, 1));
		assertTrue(testObject.areConnected(1, 2));
		assertTrue(testObject.areConnected(1, 3));
		
		assertFalse(testObject.areConnected(2, 0));
		assertTrue(testObject.areConnected(2, 1));
		assertFalse(testObject.areConnected(2, 2));
		assertFalse(testObject.areConnected(2, 3));
		
		assertTrue(testObject.areConnected(3, 0));
		assertTrue(testObject.areConnected(3, 1));
		assertFalse(testObject.areConnected(3, 2));
		assertFalse(testObject.areConnected(3, 3));
	}
	
	@Test
	public void possibleEdges() {
		List<Edge> expected = Lists.newArrayList(new Edge(0, 2), new Edge(2, 3));
		List<Edge> actual = testObject.possibleEdges();
		assertEquals(expected, actual);
	}
	
	@Test
	public void asString() {
		String expected = 
				"0101\n" +
				"1011\n" +
				"0100\n" +
				"1100"; 
		String actual = testObject.toString();
		assertEquals(expected, actual);
	}
	
}
