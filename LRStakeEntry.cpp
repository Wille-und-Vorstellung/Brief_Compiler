#pragma once
#include "stdafx.h"
#include "LRStakeEntry.hpp"

//implements
LRStakeEntry::LRStakeEntry(const LRStakeEntry& a){
	classMacro = a.classMacro;
	state = a.state;
};