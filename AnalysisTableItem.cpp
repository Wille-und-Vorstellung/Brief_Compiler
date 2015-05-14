#pragma once
#include "stdafx.h"
#include "AnalysisTableItem.hpp"

//implements
AnalysisTableItem::AnalysisTableItem(const AnalysisTableItem& a){
	actionType = a.actionType;
	stateNumber = a.stateNumber;
	semanticActionID = a.semanticActionID;
};