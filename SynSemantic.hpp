#pragma once
#include"stdafx.h"
#include<iostream>
#include"Tuple4.hpp"
#include"LRItem.hpp"
#include"LRStakeEntry.hpp"
#include"AnalysisTableItem.hpp"
#include"LRStakeEntry.hpp"
#include"LRICluster.hpp"

using std::string;

#define SEMANTIC_ITEM_N 13

/*
 * author: Ruogu Gao
 * date: 4.19.2015
 * description: 
 *	1. syntax and semantic analyser
 *	2. receive the output vector of Lexer and output the object code in the form of 4-tuple
 *	3. using operator precedence syntaxer and buttom-up semantic 
 */

class SynSemantic{
public:

	void activate(vector<Token>, vector<Token>);
	const Tuple4 gets(long );
	//result expressing
	const vector<Tuple4> getObjectCode();
	const vector<LRItem> getReducedLRItem();
	const vector<long> getErrorRecord();

	SynSemantic():readerIndex(-1), stakeIndex(-1){
	};
	~SynSemantic(){
	};
private:
	//protagonists
	/*
	vector<Token> blockReader();
	long dispatcher( vector<Token> );
	void LRLocent();
	void TPAnalysis_Branch();
	void TPAnalysis_Loop();
	*/
	bool LRAnalyser( Token );
	string LRAnalyserAUX( vector<LRStakeEntry>&, long& ,
		vector<vector<AnalysisTableItem>>&, 
		vector<vector<AnalysisTableItem>>&, Token );//one single meta-analysis step
	void constructAugmentedGrammar();
	void constructAnalysisTable();
	void constructCanonicalCollection();
	
	Token accessTokenSequence(long );
	vector<Token> first( vector<Token> );
	vector<Token> firstAUX( Token );
	vector<LRItem> getClosure( vector<LRItem> );
	vector<LRItem> gotoTransition( Token , vector<LRItem> );
	
	//auxilary
	int transcribeTableIndex( Token );
	void semanticActionDispatcher(long );
	void showErrorRecord();
	bool containVOID( vector<Token> );//judges whether VOID is inthe input token vector
	vector<Token> joinSet( vector<Token>&, vector<Token>& );
	vector<Token> eraseVOID( vector<Token> );
	bool isTerminator( Token );
	void prepareFirstSetRelated();
	//need to initialize the firstSet vector and the firstSetTrailRecord vector
	void setSemanticItemList();
	void clearFirstTrail();
	long indexInSIL( Token );//index in semanticItemList, which is the ID of input semantic item
	bool isVisitedInFT(long);//if the coresponding semantic item of input ID has been visited
	void initializeLRTC();//push S'->.S,# to the first cluster
	Token newToken( string macro );
	void setTerminator( vector<Token> );
	bool isInCCollection(vector<LRItem>);
	long idInCCollection(vector<LRItem>);
	long locateReducer(LRItem);
	void manualSpecifyFirstSets();

	//debug
	void showProducer();
	void addReducedItem( LRItem );//called in LRanalyser when an LRItem is reduced.
	void showAddressCode();

	vector<LRItem> augmentedGrammar;//sequence number of reducers stored here.
	vector<vector<AnalysisTableItem>> actionTable, gotoTable;
	//vector<vector<Token>> canonicalCollection;
	vector<LRStakeEntry> LRStake;

	vector<Token> tokenSequence;
	//vector<string> accessRecord;//used in recursively constructing firstSet to avoid loop recursion
	long readerIndex;//points to the current input token
	long stakeIndex;//points to the top of LR stake

	vector<Tuple4> objectCode;
	vector<LRItem> reducedLRItem;
	vector<long> errorRecord;

	vector<Token> firstSet[SEMANTIC_ITEM_N];
	//well it turns out that we eventually need to record all the first sets whe
	//trying to recursively calculate them, the reason is to prevent recursive loop
	//from occuring which would not stop thus causing halting.
	//bascally, the SEMANTIC_ITEM_N equals the number of producers, which is also augmentGrammar.size()
	long firstSetTrail[SEMANTIC_ITEM_N];
	//like the firstSet above, this recording the calling trail during firstAux
	//calling(manually record the system stack), is to prevent loop from occuring,
	//when a new firstAux calls a visited first set X, returns the current X in
	//firstSet instead	 of recursively calling another firstAux again.
	/*
	well you know what, I starts to thinks this program to be a little bit clumsy......
	*/
	vector<Token> semanticItemList;
	vector<Token> terminatorSet;
	vector<LRICluster> canonicalCollection;

	vector<Token> firstB;//hold the manually specified first set of B
	vector<Token> firstE;
	vector<Token> firstLIST;
	vector<Token> firstLEFT;
	vector<Token> firstF;
	vector<Token> firstP;
	vector<Token> firstK;
};
