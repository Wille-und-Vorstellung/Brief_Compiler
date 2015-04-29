#include"stdafx.h"
#include<iostream>
using std::string;

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

	void activate( vector<Token> );
	const vector<Tuple4> getResult();
	const Tuple4 gets(long );

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
	void constructAugmentedGrammar();
	void constructAnalysisTable();
	void constructCanonicalCollection();
	
	Token accessTokenSequence(long );
	vector<Token> first( vector<Token> );
	vector<LRItem> getClosure( vector<LRItem> );
	vector<LRItem> gotoTransition( Token , vector<LRItem> );
	
	//auxilary
	int transcribeTableIndex( Token );
	void semanticActionDispatcher(long );
	void showErrorRecord();

	//debug
	void showProducer();
	void addReducedItem( LRItem );//called in LRanalyser when an LRItem is reduced.
	void showAddressCode();

	vector<LRItem> augmentedGrammar;//sequence number of reducers stored here.
	vector<vector<AnalysisTableItem>> actionTable, gotoTable;
	vector<vector<Token>> canonicalCollection;
	vector<LRStakeEntry> LRStake;

	vector<Token> tokenSequence;
	long readerIndex;//points to the current input token
	long stakeIndex;//points to the top of LR stake

	vector<Tuple4> objectCode;
	vector<LRItem> reducedLRItem;
	vector<long> errorRecord;
};
