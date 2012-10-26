package cz.cvut.fit.par.kgm.simple;

import static org.fest.assertions.api.Assertions.assertThat;

import org.junit.Before;
import org.junit.Test;

public class TriangularMatrixTest {

	private Matrix sm;
	private Matrix tm;
	
	@Before
	public void init() {
		GraphBuilder gb = new GraphBuilder(7);
		gb.withEdge(0, 1);
		gb.withEdge(1, 2);
		gb.withEdge(1, 3);
		gb.withEdge(1, 4);
		gb.withEdge(0, 4);
		gb.withEdge(4, 5);
		gb.withEdge(4, 6);
		sm = gb.build();
		
		tm = new TriangularMatrix(7);
		for (int i = 0; i < gb.adjacencyMatrix.length; i++) {
			for (int j = 0; j < gb.adjacencyMatrix[0].length; j++) {
				if (gb.adjacencyMatrix[i][j] == true) {
					tm.set(i, j, true);
				}
			}
		}
	}
	
	@Test
	public void sameElements() {
		for (int i = 0; i < sm.size(); i++) {
			for (int j = 0; j < sm.size(); j++) {
				boolean expected = sm.get(i, j);
				boolean actual = tm.get(i, j);
				System.out.println(i + ", " + j);
				assertThat(actual).isEqualTo(expected);
			}
		}
	}
	
}
