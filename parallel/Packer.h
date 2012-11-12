/* 
 * File:   Packer.h
 * Author: stoupa
 *
 * Created on November 9, 2012, 11:32 PM
 */

#ifndef PACKER_H
#define	PACKER_H

#include "Edge.h"
#include "SpanningTree.h"
#include "Token.h"

#define BUFFER_SIZE 1000

class Packer {
public:
	static char * packWorkShare(std::pair<std::vector<Edge> *, SpanningTree *> * workShare, int * size);
	static std::pair<std::vector<Edge> *, SpanningTree *> * unpackWorkShare(char * packed);
	static int packToken(Token token);
	static Token unpackToken(int packed);
private:
};

#endif	/* PACKER_H */

