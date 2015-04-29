#include "stdafx.h"
#include "SynSemantic.hpp"

SynSemantic::SynSemantic():readerIndex(0){
	augmentedGrammar.clear();
	actionTable.clear();
	gotoTable.clear();
	canonicalCollection.clear();
	tokenSequence.clear();
	objectCode.clear();
	reducedLRItem.clear();
	LRStake.clear();
	errorRecord.clear();
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

void SynSemantic::showErrorRecord(){
	if ( errorRecord.size() == 0 ){
		cout << "No syntax error detected." << endl;
		return;
	}
	cout << " Possible Syntax errores in those position:" <<endl;
	for (int i=0; i<errorRecord.size(); i++){
		cout << "(" << i <<")" << "at " << errorRecord[i] << endl;

	}
};

void SynSemantic::activate( vector<Token> lexerResult ){
	tokenSequence = lexerResult;
	//preprocess: prepare the LR(1) canonical collection and ananlysis table
	constructAugmentedGrammar();
	constructCanonicalCollection();
	constructAnalysisTable();
	//prepare the LR stake
	LRStake.clear();
	LRStakeEntry buttom;
	buttom.classMacro = "#";
	buttom.state = 0;
	LRStake.push_back( buttom );//initial push, the stake buttom
	stakeIndex = LRStake.size() - 1;
	//canonical collection ready, analysis table ready, good to read the token string
	Token temp;
	bool finishFlag = false;
	for (int i=0; i<tokenSequence.size(); i++ ){
		readerIndex = i;
		temp = accessTokenSequence(i);
		//if you truly wants to apply multi-table strategy, then here is the place to judge 
		//which table shall be selected. 
		//an option: reconstruct LRanalyser, add an indicator to choose different table and stake
		finishFlag = LRAnalyser( temp );
		if ( finishFlag )
			break;
	};
	//check-point
	if ( !finishFlag || (finishFlag && tokenSequence.size()-1 != readerIndex ) ){
		cerr << "error occured during LR(1) analysis, the result is corrupted." << endl;
	}
	showProducer();
	getchar();
	showAddressCode();
	getchar();

	cout 
		<< 
		"It's quite certain that there exists some time point in future when you staring at this " <<
		"odd annotation, the syntax & semantic module has been testified and good to go, " <<
		"then you will have understand the true value of the persitence and laborious work " <<
		"after writting these words." 
		<< 
	endl;
	cout << "syntax and semantic analysis complete." << endl;
	return;
};
//------------------------our protagonists 
bool SynSemantic::LRAnalyser( Token x ){/////////////////////////////////////////////
	bool reduceFlag = false;

	long row =0, column =0;
	row = LRStake[stakeIndex].state;
	column = transcribeTableIndex( x );
	if ( column == -1 ){//error check point
		cerr 
			<<" illegal token( "<< x.classMarco<< " )detected in LRAnalyser. "
			<<" illegal token ignored, analysis proceding." <<	
		endl;
		return false;
	}

	AnalysisTableItem tempTableItem;
	tempTableItem = actionTable[row][column];
	if ( tempTableItem.actionType == "S" ) {
		reduceFlag = false;
	}
	else if ( tempTableItem.actionType == "R" ){
		reduceFlag = true;
	}
	else if ( tempTableItem.actionType == "ACC" ){
		cout << "LR(1) analysis finished." << endl;
		return true;
	}
	else{//error in syntax occured
		//counter-mmeasure: ignore current input token
		//record this error
		errorRecord.push_back(readerIndex);
		return;
	}

	LRStakeEntry tempEntry;
	vector<LRStakeEntry>* currentStake = &LRStake;
	if ( !reduceFlag ) { //in stake
		tempEntry.classMacro = x.classMarco;
		tempEntry.state = tempTableItem.semanticActionID;
		currentStake->push_back(tempEntry);
	}
	else if ( reduceFlag ) {//reduce
		//stage one: pop stake
		long reduceLength = augmentedGrammar[tempEntry.state].rightSide.size();
		for (int i=0; i< reduceLength; i++){
			currentStake->pop_back();//potential hazard here
		}
		//stage two: push stake
		while (){

		}
	}
	return false;
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


	//if is illegal token, return -1, cerr<<"...."<<endl;
};

void SynSemantic::constructAugmentedGrammar(){/////////////////////////////////////
};

void SynSemantic::semanticActionDispatcher( long actionID ){///////////////////////
	//range check
	if ( actionID < 0 ){
		cerr << "invalid actionID detected in semanticActionDispatcher." << endl;
		return;
	}
	//dispatch semantic action
	switch ( actionID ){
	case 0:
		break;
	case 1:
		break;



	default:
			break;
	}
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
