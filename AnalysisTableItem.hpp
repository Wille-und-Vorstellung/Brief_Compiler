#include "stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.28.2015
 * description: 
 *	1. the entry in LR(1) ananlysis table
 *	2. format: actionType: string; semanticActionID: long;
 *	3. actionTyoe = {S, R, ACC}, stands for in-stake , reduce 
 *	   and accept(i.e. sentence recognition complete) respectively
 *	4. semanticActionID, which is valid only when actionType == R, 
 *	   coresponds to the semanticActionID in class LRItem
 *	5. 
 */

class AnalysisTableItem{
public:
	string actionType;
	long semanticActionID;

	AnalysisTableItem():semanticActionID(-1){};
	AnalysisTableItem( const AnalysisTableItem & );
	~AnalysisTableItem(){};
private:

};

//implements
AnalysisTableItem::AnalysisTableItem( const AnalysisTableItem& a ){
	actionType = a.actionType;
	semanticActionID = a.semanticActionID;
};
