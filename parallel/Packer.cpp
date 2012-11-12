/* 
 * File:   Packer.cpp
 * Author: stoupa
 * 
 * Created on November 9, 2012, 11:32 PM
 */

#include "mpi.h"
#include <utility>
#include <vector>
#include <stack>
#include "Packer.h"
#include "SpanningTree.h"
#include "Logger.h"

using namespace std;

// stack_size | spanning_tree | marker | stack_edges | marker
// spanning tree: vertex_count | edge_count | vertex_degrees | marker | edges
// edge: edge_from | edge_to

// stack_size | vertex_count | edge_count | vertex_degrees ... | marker | edges ... | marker | stack_edges | marker

char * Packer::packWorkShare(pair<vector<Edge> *, SpanningTree *> * workShare, int * size) {
	char * buffer = new char[BUFFER_SIZE];
	int position = 0;
	
	vector<Edge> * stack = workShare->first;
	SpanningTree * tree = workShare->second;
	
	if (PACKER_DEBUG) Logger::logLn("packing:");

	// stack_size
	int stackSize = stack->size();
	if (PACKER_DEBUG) { stringstream str; str << "stack_size is: " << stackSize << endl; Logger::log(&str); }
	MPI_Pack(&stackSize, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

	// vertex_count
	int vertices = tree->vertexCount();
	if (PACKER_DEBUG) { stringstream str; str << "vertex_count is: " << vertices << endl; Logger::log(&str); }
	MPI_Pack(&vertices, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	
	// edge_count
	int edgeCnt = tree->edgeCount();
	if (PACKER_DEBUG) { stringstream str; str << "edge_count is: " << edgeCnt << endl; Logger::log(&str); }
	MPI_Pack(&edgeCnt, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	
	// vertex_degrees
	int * degrees = tree->getVertexDegrees();
	if (PACKER_DEBUG) {
		stringstream str;
		str << "degrees: ";
		for (int i = 0; i < vertices; i++) {
			str << degrees[i] << " ";
		}
		str << endl;
		Logger::log(&str);
	}
	MPI_Pack(degrees, vertices, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

	// marker
	char marker = '*';
	if (PACKER_DEBUG) Logger::logLn("*");
	MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	
	// edges
	vector<Edge> * edges = tree->getEdges();
	for(int i = 0; i < tree->edgeCount(); i++) {
		Edge edge = (*edges)[i];
		int edgeFrom = edge.vertex1;
		int edgeTo = edge.vertex2;
		MPI_Pack(&edgeFrom, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
		MPI_Pack(&edgeTo, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	}
	
	// marker
	MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	
	// stack_edges
	for(int i = 0; i < stackSize; i++) {
		Edge edge = (*stack)[i];
		int edgeFrom = edge.vertex1;
		int edgeTo = edge.vertex2;
		MPI_Pack(&edgeFrom, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
		MPI_Pack(&edgeTo, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	}
	
	// marker
	MPI_Pack(&marker, 1, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
	
	*size = position;

	return buffer;
}

pair<vector<Edge> *, SpanningTree *> * Packer::unpackWorkShare(char * buffer) {
	int position = 0;
	if (PACKER_DEBUG) Logger::logLn("unpacking:");

	// stack_size
	int stackSize = 0;	
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &stackSize, 1, MPI_INT, MPI_COMM_WORLD);
	if (PACKER_DEBUG) { stringstream str; str << "stack_size is: " << stackSize << endl; Logger::log(&str); }

	// vertex_count
	int vertices = 0;
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &vertices, 1, MPI_INT, MPI_COMM_WORLD);
	if (PACKER_DEBUG) { stringstream str; str << "vertex_count is: " << vertices << endl; Logger::log(&str); }

	// edge_count
	int edgeCount;
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &edgeCount, 1, MPI_INT, MPI_COMM_WORLD);
	if (PACKER_DEBUG) { stringstream str; str << "edge_count is: " << edgeCount << endl; Logger::log(&str); }

	// vertex_degrees ...
	int * degrees = new int[vertices];
	MPI_Unpack(buffer, BUFFER_SIZE, &position, degrees, vertices, MPI_INT, MPI_COMM_WORLD);
	if (PACKER_DEBUG) { stringstream str;
		// 	cout << "degrees: ";
		for (int i = 0; i < vertices; i++) {
			str << degrees[i] << " ";
		}
		str << endl;
		Logger::log(&str);
	}
	
	// marker
	char marker = 'X';
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &marker, 1, MPI_CHAR, MPI_COMM_WORLD);
	if (marker != '*') {
		throw;
	}
	if (PACKER_DEBUG) Logger::logLn("*");
	
	// edges ... 
	int elemCnt = edgeCount * 2;
	int * edges = new int[elemCnt];
	MPI_Unpack(buffer, BUFFER_SIZE, &position, edges, elemCnt, MPI_INT, MPI_COMM_WORLD);
	
	// marker 
	marker = 'X';
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &marker, 1, MPI_CHAR, MPI_COMM_WORLD);
	if (marker != '*') {
		throw;
	}
	
	//stack_edges
	elemCnt = stackSize * 2;
	int * stackEdges = new int[elemCnt];
	MPI_Unpack(buffer, BUFFER_SIZE, &position, stackEdges, elemCnt, MPI_INT, MPI_COMM_WORLD);
	vector<Edge> * stackEdgeVec = new vector<Edge>();
	for (int i = 0; i < elemCnt; i++) {
		Edge e = Edge(stackEdges[i], stackEdges[i + 1]);
		stackEdgeVec->insert(stackEdgeVec->end(), e);
		i++;
	}
	delete[] stackEdges;
	
	// marker
	marker = 'X';
	MPI_Unpack(buffer, BUFFER_SIZE, &position, &marker, 1, MPI_CHAR, MPI_COMM_WORLD);
	if (marker != '*') {
		throw;
	}
	
	SpanningTree * newTree = new SpanningTree(vertices, edgeCount, degrees, edges);
	delete[] edges;
			
	return new pair<vector<Edge> *, SpanningTree *>(stackEdgeVec, newTree);
}

int Packer::packToken(Token token) {
	return (token == WHITE) ? 0 : 1;
}

Token Packer::unpackToken(int packed) {
	return (packed == 0) ? WHITE : BLACK;
}
