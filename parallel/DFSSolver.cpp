/* 
 * File:   DFSSolver.cpp
 */

#include "DFSSolver.h"
#include "Packer.h"
#include "Logger.h"
#include <limits>
#include <sstream>

#define WORK_STEPS 100

using namespace std;

DFSSolver::DFSSolver(UndirectedGraph * graph) {
	this->graph = graph;
	spanningTree = new SpanningTree(graph->vertexCount());
	edgeStack = new vector<Edge>();
	bestPrice = numeric_limits<int>::max();
	best = NULL;

	color = BLACK;
	whiteTokenSent = false;
	workCounter = 0;
	finished = false;

	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &commSize);
	MPI_Comm_rank(comm, &rank);

	if (MPI_DEBUG) {
		stringstream str;
		str << "My graph is: " << endl;
		str << *graph;
		Logger::log(&str);
	}
}

DFSSolver::~DFSSolver() {
	delete edgeStack;	
	delete spanningTree;
	delete best;
}

pair<vector<Edge> *, int> * DFSSolver::findBestSolution() {
	// initial state - push all edges adjacent to vertex 0 to stack
	if (rank == 0) {
		vector<Edge> * initial = firstEdgeCandidates();
		for (unsigned i = 0; i < initial->size(); i++) {
			edgeStack->push_back((*initial)[i]);
		}
		delete initial;
	}
	
	while (! shouldTerminate()) {

		if (edgeStack->size() == 0) {
            continue;
		}

		if (DEBUG) printStack(edgeStack);
		// remove top edge from stack
		Edge current = edgeStack->back();
		edgeStack->pop_back();
		if (current.isBacktrackMarker()) {
			if (DEBUG) Logger::logLn("backtracking");
			// if current edge is backtrack marker, remove last edge from spanning tree and decrement vertex degrees
			spanningTree->removeLastEdge();
		} else {
			// add current edge to spanning tree, increment vertex degrees
			spanningTree->addEdge(current);
			if (DEBUG) printVertexDegrees();
			if (DEBUG) printSpanningTree(spanningTree);
			if (isSolution()) {
				int price = spanningTree->evaluate();
				if (isBestPossible(price)) {
					// if the current solution is the best possible, return
					updateBest(price);
					if (MPI_DEBUG) { stringstream str; str << rank << " found the best possible solution." << endl; Logger::log(&str); }
					askToTerminate();
				} else if (isBestSoFar(price)) {
					// if not best possible, but better that any solution so far, update best
					updateBest(price);
					if (MPI_DEBUG) { stringstream str; str << rank << " found new best solution with price " << price << endl; Logger::log(&str); }
					if (MPI_DEBUG) printSpanningTree(spanningTree);
				}
				// since we've found the solution, we're at the bottom of the DFS tree -> backtracking
				spanningTree->removeLastEdge();
			} else {
				// add backtrack marker to stack so we will know when we are moving up the DFS tree
				pushBacktrackMarker();
				// find new edges to add to spanning tree
				vector<Edge> * candidates = graph->edgeCandidates(spanningTree);
				if (DEBUG) printCandidates(candidates);
				for (unsigned i = 0; i < candidates->size(); i++) {
					Edge & edge = (*candidates)[i];
					if (possibleWinner(edge)) {
						// if the current candidate edge can lead to better solution than the best solution so far,
						// add it to the stack
						edgeStack->push_back(edge);
					} else {
						if (DEBUG) { stringstream str; str << "leaving out edge " << edge << endl; Logger::log(&str); }
					}
				}
				delete candidates;
			}
		}
		if (DEBUG) { stringstream str; str <<  "---------------------------" << endl; Logger::log(&str); }
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
	stringstream str; str << *tree << endl; Logger::log(&str);
}

void DFSSolver::printStack(vector<Edge> * stack) {
	stringstream str;
	str << endl << "|--------|" << endl;
	for(unsigned i = 0; i < stack->size(); i++) {
		Edge & edge = (*stack)[i];
		if (edge.isBacktrackMarker()) {
			str << "|   **   |" << endl;
		} else {
			str << "|";
			str.width(2);
			str << edge;
			str.width(2);
			str << "|" << endl;
		}
	}
	str << "^        ^" << endl;
	Logger::log(&str);
}

void DFSSolver::printCandidates(vector<Edge> * candidates) {
	stringstream str;
	str << candidates->size() << " possibilities: ";
	for (unsigned i = 0; i < candidates->size(); i++) {
		Edge & edge = (*candidates)[i];
		if (i != 0) {
			str << ", ";
		}
		str << edge;
	}
	str << endl;
	Logger::log(&str);
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
	for(unsigned i = 0; i < edgeStack->size(); i++) {
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
	for(unsigned i = elemToMove + 1; i < edgeStack->size(); i++) {
		Edge edge = (*edgeStack)[i];
		if (edge.isBacktrackMarker()) {
			backtracks++;
		}
	}

	if (MPI_DEBUG) Logger::logLn("existing stack:");
	if (MPI_DEBUG) printStack(edgeStack);
	
	if (MPI_DEBUG) { stringstream str; str << "elemToMove = " << elemToMove << endl; Logger::log(&str); }

	// copy elems from 0 to elemToMove included
	vector<Edge> * newStack = new vector<Edge>();
	for(int i = 0; i < elemToMove + 1; i++) {
		Edge & e = (*edgeStack)[i];
		newStack->insert(newStack->end(), e);
		edgeStack->erase(edgeStack->begin() + i);
	}
	if (MPI_DEBUG) Logger::logLn("stack to send:");
	if (MPI_DEBUG) printStack(newStack);

	if (MPI_DEBUG) Logger::logLn("splitted stack:");
	if (MPI_DEBUG) printStack(edgeStack);

	if (MPI_DEBUG) Logger::logLn("existing spanning tree:");
	if (MPI_DEBUG) printSpanningTree(spanningTree);

	SpanningTree * newTree = new SpanningTree(*spanningTree);
	if (MPI_DEBUG) { stringstream str; str << "backtracks to do: " << backtracks << endl; Logger::log(&str); }
	for(int i = 0; i < backtracks; i++) {
		newTree->removeLastEdge();
	}
	
	if (MPI_DEBUG) Logger::logLn("splitted spanning tree:");
	if (MPI_DEBUG) printSpanningTree(newTree);

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
		if (MPI_DEBUG) { stringstream str; str << "* " << rank << " exiting..." << endl; Logger::log(&str); }
		return true;
	}

	if (hasWorkToShare()) {
		handleWorkRequests();
		return false;
	} else {
		// not enough work to share
		if (MPI_DEBUG) { stringstream str; str << rank << " has no work to share." << endl; Logger::log(&str); }
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
				return true;
			}

			if (!workRequestSent) {
				sendWorkRequest();
				workRequestSent = true;
			} else {
				int workFrom;
				bool workAvailable = checkWorkResponse(&workRequestSent, &workFrom);
				if (workAvailable) {
					receiveWork(workFrom);
					return false;
				}
			}
		}
	}

}

bool DFSSolver::hasWorkToShare() {
	int edgeCount = 0;
	for (unsigned i = 0; i < edgeStack->size(); i++) {
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
	if (received) {
		int source = status.MPI_SOURCE;
		if (MPI_DEBUG) { stringstream str; str << rank << " has WORK_REQ from " << source << endl; Logger::log(&str); }
		int message = 0;
		MPI_Recv(&message, 0, MPI_CHAR, source, WORK_REQ, comm, MPI_STATUS_IGNORE);
		sendWork(source);
	} else {
		// stringstream str; str << rank << " has no work requests." << endl; Logger::log(&str);
	}
}

void DFSSolver::sendWork(int to) {
	pair<vector<Edge> *, SpanningTree *> * work = splitWork();
	if (work == NULL) {
		return;
	}
	int position = 0;
	char * message = Packer::packWorkShare(work, &position);
	MPI_Send(message, position, MPI_PACKED, to, WORK_SHARE, comm);
	if (MPI_DEBUG) { stringstream str; str << rank << " sent work request to " << to << endl; Logger::log(&str); }
	checkColorChange(to);

	delete work->first;
	delete work->second;
	delete work;
	delete[] message;
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
		if (MPI_DEBUG) { stringstream str; str << rank << " received and REJECTED work request from " << source << endl; Logger::log(&str); }
		MPI_Iprobe(MPI_ANY_SOURCE, WORK_REQ, comm, &flag, &status);
	}
}

void DFSSolver::sendWorkRequest() {
	int message = 0;
	int position = 0;
	int destination = nextNode();
	MPI_Send(&message, position, MPI_INT, destination, WORK_REQ, comm);
	if (MPI_DEBUG) { stringstream str; str << rank << " sent work request to " << destination << endl; Logger::log(&str); }
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
		if (MPI_DEBUG) { stringstream str; str << "There is shared work for " << rank << " from " << *availableFrom << endl; Logger::log(&str); }
		return true;
	} else {
		// check for rejections
		int rejected = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, WORK_REJECT, comm, &rejected, &status);
		if (rejected) {
			// got rejection for work request
			int source = status.MPI_SOURCE;
			if (MPI_DEBUG) { stringstream str; str << rank << " received work request rejection from " << source << endl; Logger::log(&str); }
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
	if (MPI_DEBUG) { stringstream str; str << rank << " RECEIVED work from " << source << endl; Logger::log(&str); }
	pair<vector<Edge> *, SpanningTree *> * work = Packer::unpackWorkShare(workBuffer);
	vector<Edge> * newEdgeStack = work->first;
	SpanningTree * newTree = work->second;
	
	if (MPI_DEBUG) Logger::logLn("received tree: ");
	if (MPI_DEBUG) printSpanningTree(newTree);
	if (MPI_DEBUG) Logger::logLn("received stack: ");
	if (MPI_DEBUG) printStack(newEdgeStack);
	
	delete this->spanningTree;
	this->spanningTree = newTree;
	
	for (int i = newEdgeStack->size() - 1; i > 0; i--) {
		Edge e = (*newEdgeStack)[i];
		if (MPI_DEBUG) { stringstream str; str << e << ", "; Logger::log(&str); }
		this->edgeStack->push_back(e);
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
		if (MPI_DEBUG) { stringstream str; str << "* " << rank << " sent TERMINATION tag to " << i << endl; Logger::log(&str); }
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
		if (MPI_DEBUG) { stringstream str; str << "* " << rank << " has received termination request." << endl; Logger::log(&str); }
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
	if (hasToken) {
		receiveToken();
	} else {
		// { stringstream str; str << "* No token for " << rank << endl; Logger::log(&str); }
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
	if (MPI_DEBUG) { stringstream str; str << "* " << rank << " sent " << tokenToStr(token) << " to " << dest << endl; Logger::log(&str); }
	this->color = WHITE;
}

void DFSSolver::receiveToken() {
	int source = prevNode();
	int packed = 0;
	MPI_Recv(&packed, 1, MPI_INT, source, TOKEN, comm, MPI_STATUS_IGNORE);
	Token token = Packer::unpackToken(packed);
	if (MPI_DEBUG) { stringstream str; str <<  "* " << rank << " received " << tokenToStr(token) << " from " << source << endl; Logger::log(&str); }
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

