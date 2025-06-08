#pragma once

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