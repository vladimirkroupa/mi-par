/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"
#include "Packer.h"

#define WORK_STEPS 100

using namespace std;

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	this->graph = graph;
	
	edgeStack = new vector<Edge>();
	bestPrice = 0;
	best = NULL;
	
	spanningTree = new SpanningTree(graph->vertexCount());

	color = BLACK;
	whiteTokenSent = false;
	workCounter = 0;
	finished = false;

	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &commSize);
	MPI_Comm_rank(comm, &rank);
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
		if (DEBUG) printStack(edgeStack);
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
			if (DEBUG) printSpanningTree(spanningTree);
			if (isSolution()) {
				if (! DEBUG) printSpanningTree(spanningTree);
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

void DFSSolver::printSpanningTree(SpanningTree * tree) {
	cout << *tree << endl;
}

void DFSSolver::printStack(vector<Edge> * stack) {
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

pair<vector<Edge> *, SpanningTree *> * DFSSolver::splitWork() {
	// find edge to move to new stack
	int elemToMove = -1;
	for(int i = 0; i < edgeStack->size(); i++) {
		Edge edge = (*edgeStack)[i];
		if (! edge.isBacktrackMarker()) {
			elemToMove = i;
			break;
		}
	}
	if (elemToMove == -1) {
		return NULL;
	}

	// count backtracks to do
	int backtracks = 0;
	for(int i = elemToMove + 1; i < edgeStack->size(); i++) {
		Edge edge = (*edgeStack)[i];
		if (edge.isBacktrackMarker()) {
			backtracks++;
		}
	}
	
	// copy elems from 0 to elemToMove included
	vector<Edge> * newStack = new vector<Edge>();
	for(int i = 0; i < elemToMove + 1; i++) {
		(*newStack)[i] = (*edgeStack)[i];
		edgeStack->erase(edgeStack->begin() + i);
	}
	
	SpanningTree * newTree = new SpanningTree(*spanningTree);
	for(int i = 0; i < backtracks; i++) {
		newTree->removeLastEdge();
	}
	
	return new pair<vector<Edge> *, SpanningTree *>(newStack, newTree);
}	
	
bool DFSSolver::shouldTerminate() {
	if (workCounter > WORK_STEPS) {
		workCounter %= WORK_STEPS;
		return false;
	} else {
		workCounter++;
	}

	checkTerminationMsg();
	if (this->finished) {
		cout << "* " << rank << " exiting..." << endl;
		return true;
	}

	if (hasWorkToShare()) {
		handleWorkRequests();
		return false;
	} else {
		// not enough work to share
		cout << rank << " has no work to share." << endl;
		rejectAll();
	}
	if (edgeStack->size() > 0) {
		// but enough work to continue
		return false;
	} else {
		// own stack is empty
		if (commSize == 1) {
			return true;
		}

		handleTokens();

		bool workRequestSent = false;
		while(true) {
			rejectAll();

			handleTokens();
			if (this->finished) {
				break;
			}

			if (!workRequestSent) {
				sendWorkRequest();
				workRequestSent = true;
			} else {
				int workFrom = 0;
				bool workAvailable = checkWorkResponse(&workRequestSent, &workFrom);
				if (workAvailable) {
					receiveWork(workFrom);
					return false;
				}
			}
		}
		return false;
	}

}

bool DFSSolver::hasWorkToShare() {
	int edgeCount = 0;
	for (int i = 0; i < edgeStack->size(); i++) {
		Edge edge = (*edgeStack)[i];
		if (! edge.isBacktrackMarker()) {
			edgeCount++;
		}
	}
	return edgeCount >= 2;
}

void DFSSolver::handleWorkRequests() {
	MPI_Status status;
	// check for work request from anyone
	int received = 0;
	MPI_Iprobe(MPI_ANY_SOURCE, WORK_REQ, comm, &received, &status);
	if (! received) {
		cout << rank << " has no work requests." << endl;
		return;
	} else {
		int source = status.MPI_SOURCE;
		cout << rank << " has WORK_REQ from " << source << endl;
		int message = 0;
		MPI_Recv(&message, 0, MPI_CHAR, source, WORK_REQ, comm, MPI_STATUS_IGNORE);
		sendWork(source);
	}
}

void DFSSolver::sendWork(int to) {
	pair<vector<Edge> *, SpanningTree *> * work = splitWork();
	if (work == NULL) {
		return;
	}
	char * message = Packer::packWorkShare(work);
	int position = 0;
	MPI_Send(&message, position, MPI_PACKED, to, WORK_SHARE, comm);
	checkColorChange(to);
}

void DFSSolver::rejectAll() {
	MPI_Status status;
	int flag = 0;
			
	MPI_Iprobe(MPI_ANY_SOURCE, WORK_REQ, comm, &flag, &status);
	while (flag) {
		int source = status.MPI_SOURCE;
		int message = 0;
		MPI_Recv(&message, 0, MPI_INT, source, WORK_REQ, comm, MPI_STATUS_IGNORE);
		int position = 0;
		MPI_Send(&message, position, MPI_INT, source, WORK_REJECT, comm);
		cout << rank << " received and REJECTED work request from " << source << endl;
		MPI_Iprobe(MPI_ANY_SOURCE, WORK_REQ, comm, &flag, &status);
	}
}

void DFSSolver::sendWorkRequest() {
	int message = 0;
	int position = 0;
	int destination = nextNode();
	MPI_Send(&message, position, MPI_INT, destination, WORK_REQ, comm);
	cout << rank << " sent work request to " << destination << endl;
}

int DFSSolver::prevNode() {
	if (rank == 0) {
		return commSize - 1;
	} else {
		return rank - 1;
	}
}

int DFSSolver::nextNode() {
	return (rank + 1) % commSize;
}

bool DFSSolver::checkWorkResponse(bool * workRequestSent, int * availableFrom) {
	int workResp = 0;
	MPI_Status status;

	// check for shared work
	MPI_Iprobe(MPI_ANY_SOURCE, WORK_SHARE, comm, &workResp, &status);
	if (workResp) {
		*workRequestSent = true;
		*availableFrom = status.MPI_SOURCE;
		cout << "There is shared work for " << rank << " from " << *availableFrom << endl;
		return true;
	} else {
		// check for rejections
		int rejected = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, WORK_REJECT, comm, &rejected, &status);
		if (rejected) {
			// got rejection for work request
			int source = status.MPI_SOURCE;
			cout << rank << " received work request rejection from " << source << endl;
			int message = 0;
			MPI_Recv(&message, 0, MPI_INT, source, WORK_REJECT, comm, MPI_STATUS_IGNORE);
			*workRequestSent = false;
			return false;
		} else {
			// no response
			*workRequestSent = true;
			return false;
		}
	}
}

void DFSSolver::receiveWork(int source) {
	char * workBuffer = new char[BUFFER_SIZE];
	MPI_Recv(workBuffer, BUFFER_SIZE, MPI_PACKED, source, WORK_SHARE, comm, MPI_STATUS_IGNORE);
	cout << rank << " RECEIVED work from " << source << endl;
	pair<vector<Edge> *, SpanningTree *> * work = Packer::unpackWorkShare(workBuffer);
	vector<Edge> * newEdgeStack = work->first;
	SpanningTree * newTree = work->second;
	
	printSpanningTree(newTree);
	printStack(newEdgeStack);
	
	delete this->spanningTree;
	this->spanningTree = newTree;
	
	for (int i = newEdgeStack->size() - 1; i >= 0; i--) {
		this->edgeStack->push_back((*newEdgeStack)[i]);
	}
}

void DFSSolver::checkColorChange(int sentWorkTo) {
	bool change = false;
	if (rank == 0 && sentWorkTo == commSize - 1) {
		// should not happen
		change = true;
	} else if (rank > sentWorkTo) {
		change = true;
	}

	if (change) {
		color = BLACK;
	}
}

void DFSSolver::askToTerminate() {
	int message = 0;
	for (int i = 0; i < commSize; i++) {
		cout << "* " << rank << " sent TERMINATION tag to " << i << endl;
		MPI_Send(&message, 0, MPI_INT, i, TERMINATE, comm);
	}
}

void DFSSolver::checkTerminationMsg() {
	int shouldTerminate = 0;
	MPI_Iprobe(0, TERMINATE, comm, &shouldTerminate, MPI_STATUS_IGNORE);
	if (shouldTerminate) {
		int message = 0;
		int source = 0;
		MPI_Recv(&message, 0, MPI_INT, source, TERMINATE, comm, MPI_STATUS_IGNORE);
		cout << "* " << rank << " has received termination request." << endl;
		this->finished = true;
	}
}

string tokenToStr(Token token) {
	return (token == WHITE) ? "white token" : "black token";
}

void DFSSolver::handleTokens() {
	this->checkTerminationMsg();

	int hasToken = 0;
	int source = prevNode();
	MPI_Iprobe(source, TOKEN, comm, &hasToken, MPI_STATUS_IGNORE);
	Token token;
	if (hasToken) {
		receiveToken();
	} else {
		cout << "* No token for " << rank << endl;
		if (! whiteTokenSent && rank == 0) {
			initialTokenSend();
		}
	}
}

void DFSSolver::initialTokenSend() {
	Token token = WHITE;
	sendToken(token);
	whiteTokenSent = true;
}

void DFSSolver::sendToken(Token token) {
	int packed = Packer::packToken(token);
	int dest = nextNode();
	MPI_Send(&packed, 1, MPI_INT, dest, TOKEN, comm);
	cout << "* " << rank << " sent " << tokenToStr(token) << " to " << dest << endl;
	this->color = WHITE;
}

void DFSSolver::receiveToken() {
	int source = prevNode();
	int packed = 0;
	MPI_Recv(&packed, 1, MPI_INT, source, TOKEN, comm, MPI_STATUS_IGNORE);
	Token token = Packer::unpackToken(packed);
	cout <<  "* " << rank << " received " << tokenToStr(token) << " from " << source << endl;
	if (rank == 0) {
		if (token == WHITE) {
			this->askToTerminate();
		} else {
			// black token arrived, try again
			this->initialTokenSend();
		}
	} else {
		token = this->color;
		sendToken(token);
	}
}

