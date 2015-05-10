#pragma once
#include "stdafx.h"
#include "Token.hpp"

//implements
Token& Token::operator= (const Token & a){
	attribute = a.attribute;
	classMarco = a.classMarco;
	tableIndex = a.tableIndex;
	return *this;
};