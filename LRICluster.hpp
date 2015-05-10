#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "LRItem.hpp"

using std::vector;
using std::string;

/*
* author: Ruogu Gao
* date: 5.9.2015
* description:
*	1. structure to hold the LR(1) item cluster in the LR(1) DFA
*	2. containing:	cluster id
*					vector of LRItem
*/

class LRICluster {
public:
	long ID;
	vector<LRItem> LRIs;
	vector<Token> intrigers;
	vector<long> dest;

	//LRItem getLRI();
	bool containLRI( LRItem& );
	void pushBack( LRItem );
	long size();
	LRItem get(long);
	bool getTransfer( Token , long* );

	LRICluster():ID(0){
		LRIs.clear();
		intrigers.clear();
		dest.clear();
	};
	LRICluster( const LRICluster& );
	~LRICluster(){};
private:

};

