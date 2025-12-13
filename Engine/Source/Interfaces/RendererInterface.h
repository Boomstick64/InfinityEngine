#pragma once
#include "../MacroHelpers.h"

INTERFACE_TYPEDEF(RENDERER_TYPE, Renderer)
INTERFACE_TYPEDEF(WINDOW_TYPE, DefaultWindow)


class IRenderer
{
public:

	IRenderer() {}
	~IRenderer() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual DefaultWindow* GetWindowInterface() = 0;
	virtual void SetWindowInterface(DefaultWindow* WindowInterface) = 0;
};