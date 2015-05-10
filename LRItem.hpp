#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.27.2015
 * description: 
 *	1. structure to hold the LR(1) item along with it's lookahead symbol
 *	2. containing:	the left and right of the producer: char vector<Token>
 *					dotposition : int
 *					lopkahead : Token
 *	3. currently there appears not to be the need of distinguishing empty producer
 *	4. overload the == and != operator
 *	5. only attribute 'classMacro ' in Token is used here.
 */

class LRItem {
public:
	int dotPosition;
	Token leftSide;
	vector<Token> rightSide;
	Token lookAhead;
	long semanticActionID;

	friend bool operator== ( const LRItem & , const LRItem & );
	friend bool operator!= ( const LRItem & , const LRItem &);

	LRItem():dotPosition(-1){};
	LRItem(int, Token, vector<Token>, Token, long);
	LRItem( const LRItem&  );
	~LRItem();
private:

};


