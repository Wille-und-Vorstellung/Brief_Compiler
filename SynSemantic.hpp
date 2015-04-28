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

	SynSemantic(){
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
	void LRAnalyser( Token );
	void constructAugmentedGrammar();
	void constructAnalysisTable();
	void constructCanonicalCollection();
	//auxilary
	Token accessTokenSequence(long );
	vector<Token> first( vector<Token> );
	vector<LRItem> getClosure( vector<LRItem> );
	vector<LRItem> gotoTransition( Token , vector<LRItem> );
	
	int transcribeTableIndex( Token );
	void semanticActionDispatcher(long );

	//debug
	void showProducer();
	void addReducedItem( LRItem );//called in LRanalyser when an LRItem is reduced.
	void showAddressCode();

	vector<LRItem> augmentedGrammar;
	vector<vector<string>> actionTable, gotoTable;
	vector<vector<Token>> canonicalCollection;

	vector<Token> tokenSequence;
	long blockReaderIndex;
	vector<Tuple4> objectCode;

	vector<LRItem> reducedLRItem;
};
