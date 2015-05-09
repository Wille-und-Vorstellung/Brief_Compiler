#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>

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

	//LRItem getLRI();
	bool containLRI( LRItem& );
	void pushBack( LRItem );

	LRICluster():ID(0){
		LRIs.clear();
	};
	~LRICluster(){};
private:

};

bool LRICluster::containLRI( LRItem& a ){
	for (int i = 0; i < LRIs.size(); i++){
		if (LRIs[i] == a ){
			return true;
		}
	}
	return false;
}

void LRICluster::pushBack( LRItem a ){
	LRIs.push_back(a);
}