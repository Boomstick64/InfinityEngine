#pragma once
#include "../MacroHelpers.h"

INTERFACE_TYPEDEF(MAIN_TYPE, Main)

class IMain
{
public:
	IMain() {}
	~IMain() {}

	virtual int Main() = 0;
};