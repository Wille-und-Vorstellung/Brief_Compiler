#include  "stdafx.h"
#include  "BriefCompiler.hpp"

/*
 * author: Ruogu Gao
 * date: 4.11.2015
 * description: 
 *	1. main test frame for the brief compiler
 *	2. 
 */
#define RELEASE
//#define TEST_SS

int _tmain(){
	//fibericating 
	BriefCompiler testCompiller;
	vector<ListTuple> lexResult;
	vector<Token> tokenTable;
	lexResult.clear();
	tokenTable.clear();

	string sourceFilePath("../testSource.txt");
#ifdef RELEASE
	testCompiller.activateCompiler(sourceFilePath);
	/*
	cout << "the Lexer output:" << endl;
	for ( unsigned int i=0; i < testCompiller.getLexerResult().size(); i++ ){
		cout << "< " << testCompiller.getLexerResult()[i].classMarco << ", " << testCompiller.getLexerResult()[i].tableIndex << " >" << endl;
	}
	cout << "------------------------" << endl;
	cout << "the token table:" << endl;
	for ( unsigned int j=0; j<testCompiller.getTokenTable().size(); j++ ){
		cout << j << "\'th row: " << testCompiller.getTokenTable()[j].classMacro << " | " << testCompiller.getTokenTable()[j].dvalue << " | " << testCompiller.getTokenTable()[j].svalue << endl;
	}
	*/
	cout << "the Syntax & Semantic below:" << endl;
	vector<Tuple4> obj;
	obj = testCompiller.getObjectCode();

	cout << "about to end" << endl;
#endif
	
#ifdef TEST_SS
	/*
	 * single test on SynSemantic Analyser
	 */
	SynSemantic testSS;
	//firstly, construct a virtual tokenSequence
	vector<Token> virtualTS;
	virtualTS.push_back(Token("", "a", -1));
	virtualTS.push_back(Token("", "b", -1));
	virtualTS.push_back(Token("", "a", -1));
	virtualTS.push_back(Token("", "b", -1));
	virtualTS.push_back(Token("", "#", -1));

	//activate SynSemantic
	testSS.activate(virtualTS, virtualTS);
	vector<LRItem> reduced;
	reduced = testSS.getReducedLRItem();

	cout << "Reduced LRItem below: " << endl;
	for (int i = 0; i < reduced.size(); i++){
		cout << reduced[i].leftSide.classMarco << " -> ";
		for (int j = 0; j < reduced[i].rightSide.size(); j++){
			cout << reduced[i].rightSide[j].classMarco << " ";
		}
		cout << endl;
	}


#endif
	return 117;
}