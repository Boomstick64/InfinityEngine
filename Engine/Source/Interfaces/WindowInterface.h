#pragma once
#include "../MacroHelpers.h"

INTERFACE_TYPEDEF(WINDOW_TYPE, DefaultWindow)

class IWindow
{
public:
	IWindow() {}
	~IWindow() {}

	virtual void Register() = 0;
	virtual void Create() = 0;
	virtual void Show() = 0;
	virtual void Update() = 0;
};