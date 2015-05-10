#include "stdafx.h"
#include "SynSemantic.hpp"
#include "LRICluster.hpp"

//#define TRANSCRIBE_OFFSET 100
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
	setSemanticItemList();
	setTerminator();
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
bool SynSemantic::LRAnalyser( Token x ){/////////////
	//choose coresponding analysis table and stake
	//...
	string actionFlag;
	do{
		actionFlag = LRAnalyserAUX( LRStake, stakeIndex, actionTable, gotoTable, x );
		if ( actionFlag == "S" ){
			break;
		}
		else if ( actionFlag == "R" ) {//needs to continue until returns R or ACC
			continue;
		}
		else if ( actionFlag == "ACC" ){
			cout << "LR(1) analysis finished." << endl;
			return true;
		}
		else if ( actionFlag == "ERROR" ){
			break;
		}
	}while( actionFlag == "R" );
	return false;
};

string SynSemantic::LRAnalyserAUX( vector<LRStakeEntry>& currentStake, long& sIndex,
								  vector<vector<AnalysisTableItem>>& actionT , 
								vector<vector<AnalysisTableItem>>& gotoT, Token x){
	bool reduceFlag = false;

	long row =0, column =0;
	row = currentStake[sIndex].state;
	column = transcribeTableIndex( x );
	if ( column == -1 ){//error check point
		cerr 
			<<" illegal token( "<< x.classMarco<< " )detected in LRAnalyserAUX. "
			<<" illegal token ignored, analysis proceding." <<	
		endl;
		return "ERROR";
	}

	AnalysisTableItem tempTableItem;
	tempTableItem = actionT[row][column];
	if ( tempTableItem.actionType == "S" ) {
		reduceFlag = false;
	}
	else if ( tempTableItem.actionType == "R" ){
		reduceFlag = true;
	}
	else if ( tempTableItem.actionType == "ACC" ){
		return "ACC";
	}
	else{//error in syntax occured
		//counter-mmeasure: ignore current input token
		//record this error
		errorRecord.push_back(readerIndex);
		return "ERROR";
	}

	LRStakeEntry tempEntry;
	vector<LRStakeEntry>* currentStake = &LRStake;
	if ( !reduceFlag ) { //in stake
		tempEntry.classMacro = x.classMarco;
		//tempEntry.state = tempTableItem.semanticActionID;//?????????????????????
		tempEntry.state = tempTableItem.stateNumber;
		currentStake.push_back(tempEntry);
		sIndex++;
	}
	else if ( reduceFlag ) {//reduce
		//stage one: pop stake
		long reduceLength = augmentedGrammar[tempEntry.state].rightSide.size();
		for (int i=0; i< reduceLength; i++){
			currentStake.pop_back();//potential hazard here: stake goes empty before loop stops
			sIndex--;
		}
		//stage two: record reducer and execute semantic function
		reducedLRItem.push_back(augmentedGrammar[tempTableItem.semanticActionID]);
		semanticActionDispatcher(tempTableItem.semanticActionID);
		//stage three: push stake the left sid of the reducer
		tempEntry.classMacro = augmentedGrammar[tempTableItem.semanticActionID].leftSide.classMarco;
		row = currentStake[sIndex].state;
		column = transcribeTableIndex(augmentedGrammar[tempTableItem.semanticActionID].leftSide);
		tempEntry.state = gotoT[row][column].stateNumber;
		currentStake.push_back(tempEntry);
		sIndex++;

		return "R";
	}
	return "S";
};

vector<Token> SynSemantic::first( vector<Token> x ){
	vector<Token> firstSet;
	vector<Token> temp;
	firstSet.clear();
	if ( x.size() == 0 ){
		return ;
	}
	clearFirstTrail();//necessary before calling firstAUX
	firstSet = firstAUX( x[0] );
	bool voidFlag = containVOID( firstSet );
	if ( !voidFlag ){//find no VOID
		return firstSet;
	}
	else if( voidFlag && x.size() > 1) {
		//delete VOID in firstSet
		firstSet = eraseVOID( firstSet );
		clearFirstTrail();//necessary before calling firstAUX
		temp = eraseVOID( firstAUX( x[1] ) );
		firstSet = joinSet( firstSet, temp );
		int i;
		for (  i=1; i<x.size()-1; i++ ){
			temp = firstAUX( x[i] );
			if ( containVOID( temp ) ){
				clearFirstTrail();//necessary before calling firstAUX
				temp = eraseVOID( firstAUX( x[i+1] ) );
				firstSet = joinSet( firstSet, temp );
			}
		}
		clearFirstTrail();//necessary before calling firstAUX
		if ( i == x.size()-1 && containVOID( firstAUX( x[i] ) ) ){
			Token voidToken;
			voidToken.classMarco = "VOID";
			firstSet.push_back( voidToken );
		}

	}
	else;
		
	return firstSet;
};

vector<Token> SynSemantic::firstAUX( Token x ){////waiting for sequential inspection
	vector<Token> firstSetTemp;
	firstSetTemp.clear();
	bool extensionFlag;
	if ( isTerminator(x) ){
		firstSetTemp.push_back(x);
		return firstSetTemp;
	}
	//check if in this cycle the input has been visited
	long tempIndex = indexInSIL(x);
	if (isVisitedInFT(tempIndex)){
		return firstSet[tempIndex];
	}
	else {//A non-terminator
		//find coresponding producers of given semantic item in augmentedGrammar
		firstSetTrail[tempIndex] = 1;
		vector<Token> temp, buffer;
		LRItem temp_1;
		int j = 0;
		for (int i=0; i < augmentedGrammar.size(); i++) {
			temp_1 = augmentedGrammar[i];
			if (  temp_1.leftSide.classMarco == x.classMarco ){
				buffer.clear();
				for (j = 0; j < temp_1.rightSide.size(); j++){
					if (temp_1.rightSide[j].classMarco != "VOID"){
						temp = firstAUX( temp_1.rightSide[j] );
						buffer = joinSet(buffer, temp);
						if (containVOID(temp)){
							continue;
						}
						break;
					}
				}
				if (j != temp_1.rightSide.size()){//first set of token x do not contain void
					buffer = eraseVOID(buffer);
				}
				firstSetTemp = joinSet(firstSetTemp, buffer);
			}
		}
		firstSet[tempIndex] = joinSet(firstSet[tempIndex], firstSetTemp);
		return firstSet[tempIndex];
	}
};

vector<LRItem> SynSemantic::getClosure( vector<LRItem> x){//waiting for sequential inspection
	int j = 0; 
	Token targetT;
	Token tempT;
	LRItem targetLRI;
	LRItem tempLRI;
	vector<LRItem> queue;
	vector<Token> tempFS;
	bool matchFlag;
	queue.clear();
	for (int i = 0; i < x.size();i++){
		if ( x[i].dotPosition <= x[i].rightSide.size()-1 ){
			//i.e. those producers that is of this form "A->B.CD, x", in which CD is not void
			//i.e. the dot is not in the last
			queue.push_back( x[i] );
		}
	}
	while ( !queue.empty() ){
		targetLRI = queue[0];
		targetT = targetLRI.rightSide[targetLRI.dotPosition];
		//get lookahead chars by First(CDx)
		tempFS.clear();
		for (int m = targetLRI.dotPosition; m < targetLRI.rightSide.size(); m++){
			tempFS.push_back(targetLRI.rightSide[m]);
		}
		tempFS.push_back(targetLRI.lookAhead);
		tempFS = first(tempFS);

		for (j = 0; j < augmentedGrammar.size(); j++){
			if (augmentedGrammar[j].leftSide.classMarco == targetT.classMarco){
				for (int k = 0; k < tempFS.size(); k++){
					tempT.classMarco = tempFS[k].classMarco;
					//search matching in x
					matchFlag = false;
					tempLRI = augmentedGrammar[j];//may cause problem due to the lack of operatot = for LRItem
					tempLRI.dotPosition = 0;
					tempLRI.lookAhead = tempT;
					for (int p = 0; p < x.size(); p++){
						if ( x[p] == tempLRI ){
							matchFlag = true;
							break;
						}
					}
					
					if ( matchFlag = false ){
						//construct new LRItem and push to x 
						x.push_back(tempLRI);
						//push to queue if qualified
						if (tempLRI.dotPosition <= tempLRI.rightSide.size() - 1){
							queue.push_back(tempLRI);
						}
					}
				}
			}
		}
		//pop the front of queue
		queue.erase( queue.begin() );//?
	}
	return x;
};

vector<LRItem> SynSemantic::gotoTransition( Token x, vector<LRItem> y){//sequential inspection
	vector<LRItem> result;
	result.clear();

	for (int i = 0; i < y.size(); i++){
		if (y[i].dotPosition < y[i].rightSide.size() && 
			y[i].rightSide[y[i].dotPosition].classMarco == x.classMarco){

			y[i].dotPosition += 1;
			result.push_back( y[i] );
		}
	}
	return getClosure(result);
};

void SynSemantic::constructCanonicalCollection(){////sequential inspection
	initializeLRTC();
	vector<LRICluster> queue;
	LRICluster tempC;
	Token tempT;
	vector<LRItem> tempLRIV;
	vector<Token> VT;
	LRICluster newC;
	long counter = 0;

	VT = joinSet( terminatorSet, semanticItemList );
	queue.clear();
	queue.push_back( canonicalCollection[0] );//push S'->.S, # into queue
	while ( !queue.empty() ){
		tempC = queue[0];
		for (int i = 0; i < tempC.size(); i++){
			for (int j = 0; j < VT.size(); j++){
				tempT = VT[j];
				tempLRIV = gotoTransition( tempT, tempC.LRIs );
				if ( tempLRIV.size() != 0  && !isInCCollection( tempLRIV ) ){
					//construct a new LRICluster and push into canonicalCollection
					newC.ID = ++counter;
					newC.LRIs = tempLRIV;
					canonicalCollection.push_back(newC);
					canonicalCollection[i].intrigers.push_back(tempT);
					canonicalCollection[i].dest.push_back(newC.ID);
					//push into queue also
					queue.push_back(newC);
				}
			}
		}
		//pop the queue front
		queue.erase(queue.begin());
	}

};

void SynSemantic::constructAnalysisTable(){////////////////////////////////////////int transcribeTableIndex( Token );
	AnalysisTableItem tempATI;
	vector<AnalysisTableItem> tempColumn;
	Token targetTerminator, targetSI;
	LRICluster currentCC;
	long column, row;
	//allocate the actiontable and gototable
	//canonicalCollection.size() X terminatorSet.size() for actionTable
	actionTable.clear();
	tempColumn.clear();
	for (int j = 0; j < terminatorSet.size(); j++){
		tempColumn.push_back(tempATI);
	}
	
	for (int i = 0; i < canonicalCollection.size(); i++){
		actionTable.push_back(tempColumn);
		
	}
	//canonicalCollection.size() X semanticItemList.size() for gotoTable
	gotoTable.clear();
	tempColumn.clear();
	for (int k = 0; k < semanticItemList.size(); k++){
		tempColumn.push_back(tempATI);
	}
	for (int i = 0; i < canonicalCollection.size(); i++){
		gotoTable.push_back(tempColumn);

	}

	//construct the tables
	for (int i = 0; i < canonicalCollection.size(); i++){
		currentCC = canonicalCollection[i];
		//fill actionTable(trans with terminator)
		for (int j = 0; j < terminatorSet.size(); j++){
			targetTerminator = terminatorSet[j];
			for (int p = 0; p < currentCC.size(); p++){
				if (currentCC.intrigers[p].classMarco == targetTerminator.classMarco){//got one trans
					row = i;
					column = transcribeTableIndex(targetTerminator);
					actionTable[row][column].actionType = "S";
					actionTable[row][column].stateNumber = currentCC.dest[p];
				}
			}
		}
		//fill actionTable(deal with reducables and ACC)
		for (int q = 0; q < currentCC.size();q++){

			if (currentCC.get(q).dotPosition == currentCC.get(q).rightSide.size()){//find one reducable
				row = i;
				column = transcribeTableIndex( currentCC.get(q).lookAhead );
				/*no need for exceptions on ACC I guess...
				if (currentCC.get(q).leftSide.classMarco == "S'" && 
					currentCC.get(q).rightSide.size() == 1 &&
					currentCC.get(q).rightSide[0].classMarco == "S" &&
					currentCC.get(q).dotPosition == 1 &&
					currentCC.get(q).lookAhead.classMarco == "#"){//the ACC
				}
				else */
					actionTable[row][column].actionType = "R";
					actionTable[row][column].semanticActionID = locateReducer(currentCC.get(q));
				
			}
		}
		//fill gotoTable 
		for (int k = 0; k < semanticItemList.size(); k++){
			targetSI = semanticItemList[k];
			for (int p = 0; p < currentCC.size(); p++){
				if (currentCC.intrigers[p].classMarco == targetSI.classMarco){//got one trans
					row = i;
					column = transcribeTableIndex(targetSI);;
					gotoTable[row][column].actionType = "S";
					gotoTable[row][column].stateNumber = currentCC.dest[p];
				}
			}
		}

	}

};

//-----------------------the costars
long SynSemantic::locateReducer(LRItem a){
	LRItem temp;
	int j = 0;
	for (int i = 0; i < augmentedGrammar.size(); i++){
		if (temp == augmentedGrammar[i] &&
			temp.leftSide.classMarco == a.leftSide.classMarco &&
			temp.rightSide.size() == a.rightSide.size() &&
			temp.lookAhead.classMarco == a.lookAhead.classMarco){
			for ( j = 0; j < temp.rightSide.size(); j++){
				if (temp.rightSide[j].classMarco != a.rightSide[j].classMarco){
					break;
				}
			}

			if (j == temp.rightSide.size()){
				return i;
			}
		}
	}
	cerr << "NOT FOUND in locateReducer :" << a.leftSide.classMarco << endl;
	return 0;
}

bool SynSemantic::containVOID( vector<Token> x ){
	vector<Token>::iterator i;
	for ( i =  x.begin(); i != x.end(); i++){
		if ( i->classMarco == "VOID" ){
			return true;
		}
	}
	return false;
};

vector<Token> SynSemantic::joinSet( vector<Token>& a , vector<Token>& b ){
	vector<Token> joined;
	joined.clear();
	vector<Token>::iterator i;
	for (i = a.begin(); i != a.end(); i++){
		joined.push_back( (*i) );
	}
	for ( i= b.begin(); i != b.end(); i++ ){
		joined.push_back( (*i) );
	}
	return joined;
};

vector<Token> SynSemantic::eraseVOID( vector<Token> a ){
	vector<Token>::iterator temp;
	bool voidFlag = containVOID( a );
	if ( voidFlag ){
		return a;
	}
	for ( temp = a.begin(); temp != a.end();){
		if ( temp->classMarco == "VOID" ){
			temp = a.erase(temp);
		}
		else
			temp++;
	}
}

void SynSemantic::prepareFirstSetRelated(){
	for (int i = 0; i < SEMANTIC_ITEM_N; i++){
		firstSet[i].clear();
	}
	clearFirstTrail();
}

void SynSemantic::clearFirstTrail(){
	for (int i = 0; i < SEMANTIC_ITEM_N; i++) {
		firstSetTrail[i] = 0;
	}
	return;
}

long SynSemantic::indexInSIL(Token x){
	for (int i = 0; i < semanticItemList.size(); i++){
		if (semanticItemList[i].classMarco == x.classMarco){
			return i;
		}
	}
	//not found
	cerr << "WARNING: Token not found in indexInSIL" << endl;
	return 0;
};

bool SynSemantic::isVisitedInFT( long i){
	//boundary check
	if ( i<0 || i>SEMANTIC_ITEM_N-1 ){
		cerr << "ERROR: index out of range in isVisitedInFT" << endl;
		return true;
	}
	bool temp = ( firstSetTrail[i] == 1 ) ? true : false;
};

void SynSemantic::initializeLRTC(){
	LRItem temp;
	Token tempT;
	LRICluster tempC;
	canonicalCollection.clear();

	tempT.classMarco = "#";
	temp.dotPosition = 0;
	temp.lookAhead = tempT;
	tempT.classMarco = "S'";
	temp.leftSide = tempT;
	tempT.classMarco = "S";
	temp.rightSide.push_back( tempT );

	tempC.ID = 0;
	tempC.pushBack( temp );

	canonicalCollection.push_back(tempC);
};

Token SynSemantic::newToken( string macro ){
	Token temp;
	temp.classMarco = macro;
	return temp;
};


bool SynSemantic::isInCCollection(vector<LRItem> x){
	int j = 0;
	for (int i = 0; i < canonicalCollection.size(); i++){
		if (x.size() != canonicalCollection[i].size())
			continue;
		for ( j = 0; j < canonicalCollection[i].size(); j++){
			if (x[i] != canonicalCollection[i].get(j)){
				break;
			}
		}
		if (j == canonicalCollection[i].size()){
			return true;
		}

	}
	return false;
};

bool SynSemantic::isTerminator(Token a){
	for (int i = 0; i < terminatorSet.size(); i++){
		if (terminatorSet[i].classMarco == a.classMarco){
			return true;
		}
	}
	return false;
};

int SynSemantic::transcribeTableIndex(Token x){
	//terminator
	for (int i = 0; i < terminatorSet.size(); i++){
		if ( x.classMarco == terminatorSet[i].classMarco ){
			return i;
		}
	}
	//non-terminator(i.e. semantic item)
	for (int j = 0; j < semanticItemList.size(); j++){
		if ( x.classMarco == semanticItemList[j].classMarco ){
			return j;
		}
	}
	//if is illegal token, return 0, cerr<< "...." <<endl;
	cerr << "ERROR: illegal token detected in transcribeTableIndex " << x.classMarco << endl;
	return 0;
};

//-----------------------basic configurations (manual specified or read from files)
void SynSemantic::constructAugmentedGrammar(){/////////////////////////////////////
	augmentedGrammar.clear();

};

void SynSemantic::setSemanticItemList(){///////////////////////////////////////////
	//insert all semantic items into semanticItemList vector
	//unfortunately, it has to done manually....
	semanticItemList.clear();

};

void SynSemantic::setTerminator(){////////////////////////////////////////////////
	terminatorSet.clear();
}

void SynSemantic::semanticActionDispatcher(long actionID){///////////////////////
	//range check
	if (actionID < 0){
		cerr << "invalid actionID detected in semanticActionDispatcher." << endl;
		return;
	}
	//dispatch semantic action
	switch (actionID){
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
