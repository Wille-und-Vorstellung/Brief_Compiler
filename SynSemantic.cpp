#include "stdafx.h"
#include "SynSemantic.hpp"

SynSemantic::SynSemantic():blockReaderIndex(0){
	augmentedGrammar.clear();
	actionTable.clear();
	gotoTable.clear();
	canonicalCollection.clear();
	tokenSequence.clear();
	objectCode.clear();
	reducedLRItem.clear();
};

const vector<Tuple4> SynSemantic::getResult(){
	return objectCode;
};

const Tuple4 SynSemantic::gets( long i ){
	if ( i >= 0 && i < objectCode.size() ){
		return objectCode[i];
	}
	cerr << "error: index out of range accessing objectCode." << endl;
	return objectCode[0];
};

Token SynSemantic::accessTokenSequence( long i ){
	if ( i >= 0  && i < tokenSequence.size() ){
		return tokenSequence[i];
	}
	cerr << "error: index out of range accessing tokenSequence." << endl;
	return ;
};

void SynSemantic::activate( vector<Token> lexerResult ){
	tokenSequence = lexerResult;
	//preprocess: prepare the LR(1) canonical collection and ananlysis table
	constructAugmentedGrammar();
	constructCanonicalCollection();
	constructAnalysisTable();
	//read the token string
	Token temp;
	for (int i=0; i<tokenSequence.size(); i++ ){
		temp = accessTokenSequence(i);
		LRAnalyser( temp );
	};

	getchar();
	showProducer();
	getchar();
	showAddressCode();
	getchar();

	cout 
		<< 
		"I am quite certain that at some time point in future when you staring at this " <<
		"odd annotation, the syntax & semantic module has been testified and good to go, " <<
		"then you will have understand the true value of the persitence and laborious work " <<
		"after writting these words." 
		<< 
	endl;
	cout << "syntax and semantic analysis complete." << endl;
	return;
};
//------------------------our protagonists 
void SynSemantic::LRAnalyser( Token ){/////////////////////////////////////////////
	
};

void SynSemantic::constructAnalysisTable(){////////////////////////////////////////

};

void SynSemantic::constructCanonicalCollection(){//////////////////////////////////
};

vector<Token> SynSemantic::first( vector<Token> ){/////////////////////////////////
};

vector<LRItem> SynSemantic::getClosure( vector<LRItem> ){//////////////////////////
};

vector<LRItem> SynSemantic::gotoTransition( Token , vector<LRItem> ){//////////////
};


//-----------------------basic configurations (manual specified or read from files)
int SynSemantic::transcribeTableIndex( Token ){////////////////////////////////////
};

void SynSemantic::constructAugmentedGrammar(){/////////////////////////////////////
};

void SynSemantic::semanticActionDispatcher( long actionID ){///////////////////////
	//range check

	//dispatch semantic action

};

//-----------------------for debug usage only
void SynSemantic::showProducer(){
	cout << " all reduced producers shown below " << endl;
	for (int i=0; i< reducedLRItem.size(); i++){
		cout << reducedLRItem[i].leftSide.classMarco <<" -> ";
		for (int j=0; j<reducedLRItem[i].rightSide.size(); j++){
			cout << reducedLRItem[i].rightSide[j].classMarco << " ";
		}
		cout << endl;
	}
};

void SynSemantic::showAddressCode(){
	cout << " generated object code shown below " << endl;
	for (int i=0; i<objectCode.size(); i++){
		cout << 
			" op: " << objectCode[i].op << 
			" arg1: " << objectCode[i].arg1 <<
			" arg2: " << objectCode[i].arg2 <<
			" result: " << objectCode[i].result 
		<< endl;
	};
};

void SynSemantic::addReducedItem( LRItem a ){
	reducedLRItem.push_back( a );
	return;
};
