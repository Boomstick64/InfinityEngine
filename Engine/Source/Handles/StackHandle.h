#pragma once

template<typename T>
class StackHandle
{
public:
	StackHandle() : Object()
	{

	}

	~StackHandle() {}
	
	T& GetObj()
	{
		return Object;
	}

private:
	T Object;
};