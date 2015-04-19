#include "stdafx.h"
#include "SynSemantic.hpp"

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

void SynSemantic::activate( vector<Token> ){/////////////////////////


};

vector<Token> SynSemantic::blockReader(){//////////////////////////


};

long SynSemantic::dispatcher( vector<Token> ){//////////////////



};