#pragma once

template<typename T>
class StackHandle
{
public:
	StackHandle()
	{
		Object = T();
	}

	T& GetObj()
	{
		return Object;
	}

private:
	T Object;
};