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
	vector<Token> blockReader();
	void dispatcher( vector<Token> );
	Token accessTokenSequence(long );

	vector<Token> tokenSequence;
	long blockReaderIndex;
	vector<Tuple4> objectCode;
};
