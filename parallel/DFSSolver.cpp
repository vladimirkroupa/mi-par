/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"

using namespace std;

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	this->graph = graph;
	
	edgeStack = new vector<Edge>();
	bestPrice = 0;
	best = NULL;
	
	spanningTree = new SpanningTree(graph->vertexCount());
}

DFSSolver::~DFSSolver() {
	delete edgeStack;	
	delete spanningTree;
	delete best;
}

pair<vector<Edge> *, int> * DFSSolver::findBestSolution() {
	// initial state - push all edges adjacent to vertex 0 to stack
	vector<Edge> * initial = firstEdgeCandidates();
	for (int i = 0; i < initial->size(); i++) {
		edgeStack->push_back((*initial)[i]);
	}
	delete initial;
	
	while (! edgeStack->empty()) {
		if (DEBUG) printStack();
		// remove top edge from stack
		Edge current = edgeStack->back();
		edgeStack->pop_back();

		if (current.isBacktrackMarker()) {
			if (DEBUG) cout << "backtracking" << endl;
			// if current edge is backtrack marker, remove last edge from spanning tree and decrement vertex degrees
			spanningTree->removeLastEdge();
		} else {
			// add current edge to spanning tree, increment vertex degrees
			spanningTree->addEdge(current);
			if (DEBUG) printVertexDegrees();
			int price = spanningTree->evaluate();
			if (DEBUG) printSpanningTree(price);
			if (isSolution()) {
				if (! DEBUG) printSpanningTree(price);
				if (isBestPossible(price)) {
					// if the current solution is the best possible, return
					updateBest(price);
					return prepareSolution(best, bestPrice);
				} else if (isBestSoFar(price)) {
					// if not best possible, but better that any solution so far, update best
					updateBest(price);
				}
				// since we've found the solution, we're at the bottom of the DFS tree -> backtracking
				spanningTree->removeLastEdge();
			} else {
				// add backtrack marker to stack so we will know when we are moving up the DFS tree
				pushBacktrackMarker();
				// find new edges to add to spanning tree
				vector<Edge> * candidates = graph->edgeCandidates(spanningTree);
				if (DEBUG) printCandidates(candidates);
				for (int i = 0; i < candidates->size(); i++) {
					Edge & edge = (*candidates)[i];
					if (possibleWinner(edge)) {
						// if the current candidate edge can lead to better solution than the best solution so far,
						// add it to the stack
						edgeStack->push_back(edge);
					} else {
						if (DEBUG) cout <<  "leaving out edge " << edge << endl;
					}
				}
				delete candidates;
			}
		}
		if (DEBUG) cout <<  "---------------------------" << endl;
	}
	if (solutionExists()) {
		// at this point, we've completed the DFS tree traversal
		return prepareSolution(best, bestPrice);
	}
	return NULL;
}

void DFSSolver::pushBacktrackMarker() {
	edgeStack->push_back(Edge(-1, -1));
}

vector<Edge> * DFSSolver::firstEdgeCandidates() {
	int vertex = 0;
	return graph->edgesAdjacentTo(vertex);
}

bool DFSSolver::possibleWinner(Edge current) {
	int price = spanningTree->evaluate(current);
	return isBestSoFar(price);
}

bool DFSSolver::isSolution() {
	int vertices = graph->vertexCount();
	int edges = spanningTree->edgeCount();
	return (edges ==  vertices - 1);
}

bool DFSSolver::isBestPossible(int price) {
	return price == MIN_PRICE_POSSIBLE;
}

bool DFSSolver::isBestSoFar(int price) {
	if (best == NULL) {
		return true;
	}
	return price < bestPrice;
}

void DFSSolver::updateBest(int price) {
	bestPrice = price;
	delete best;
	best = new SpanningTree(*spanningTree); // make a copy
}

bool DFSSolver::solutionExists() {
	return best != NULL;
}

void DFSSolver::printSpanningTree(int price) {
	cout << *spanningTree << endl;
}

void DFSSolver::printStack() {
	//cout << "TODO: print stack" << endl;
}

void DFSSolver::printCandidates(vector<Edge> * candidates) {
	cout << candidates->size() << " possibilities: ";
	for (int i = 0; i < candidates->size(); i++) {
		Edge & edge = (*candidates)[i];
		if (i != 0) {
			cout << ", ";
		}
		cout << edge;
	}
	cout << endl;
}

void DFSSolver::printVertexDegrees() {
	spanningTree->printVertexDegrees();
}

pair<vector<Edge> *, int> * DFSSolver::prepareSolution(SpanningTree* solution, int solutionPrice) {
	vector<Edge> * copy = new vector<Edge>(*(solution->getEdges()));
	pair<vector<Edge> *, int> * result = new pair<vector<Edge> *, int>(copy, solutionPrice);
	return result;
}

void DFSSolver::splitWork() {
	// find edge to move
	int elemToMove = -1;
	for(int i = 0; i < edgeStack->size(); i++) {
		Edge edge = edgeStack[i];
		if (! edge.isBacktrackMarker()) {
			index = i;
		}
	}
	if (elemToMove == -1) {
		return NULL;
	}
	
	
	
}
//    /* std::cout << *it; ... */
//}
//        def fromBottomElement(self):
//            for index, edge in enumerate(self.edge_stack):
//                if not edge.isBacktrackMarker():
//                    return index
//            return None
//
//        def countBacktracks(self, to_index):
//            count = 0
//            for edge in self.edge_stack[to_index + 1:]:
//                if edge.isBacktrackMarker():
//                    count += 1
//            return count
//
//        elem_to_move = fromBottomElement(self)
//        if elem_to_move is None:
//            return None, None
//        new_stack = self.edge_stack[:elem_to_move + 1]
//        del self.edge_stack[elem_to_move]
//
//        backtracks_to_do = countBacktracks(self, elem_to_move)
//        new_spanning_tree = copy.deepcopy(self.spanning_tree)
//        for i in range(backtracks_to_do):
//            new_spanning_tree.removeLastEdge()
//
//        return new_stack, new_spanning_tree
