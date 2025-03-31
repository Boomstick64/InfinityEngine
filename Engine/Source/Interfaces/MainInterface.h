#pragma once

class IMain
{
public:
	IMain() {}
	~IMain() {}

	virtual int Main() = 0;
};