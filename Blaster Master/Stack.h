#pragma once
#include <vector>

template<class type> class Stack
{
private:
	std::vector<type> _stack;

public:
	Stack() {}
	Stack(Stack& value);
	void Push(type value);
	type Pop();
	type Peak();
	bool Contain(type value);
	void PopToValue(type value);
	int Size();
};

template<class type>
inline Stack<type>::Stack(Stack& value)
{
	for (auto x : value._stack)
	{
		this->_stack.push_back(x);
	}
}

template<class type>
void Stack<type>::Push(type value)
{
	this->_stack.push_back(value);
}

template<class type>
type Stack<type>::Pop()
{
	type value = this->_stack[this->_stack.size() - 1];
	this->_stack.pop_back();
	return value;
}

template<class type>
type Stack<type>::Peak()
{
	return this->_stack.at(this->_stack.size() - 1);
}

template<class type>
bool Stack<type>::Contain(type value)
{
	auto i = std::find(this->_stack.begin(), this->_stack.end(), value);
	if (i != this->_stack.end())
		return true;
	else
		return false;
}

template<class type>
void Stack<type>::PopToValue(type value)
{
	auto i = std::find(this->_stack.begin(), this->_stack.end(), value);
	if (i != this->_stack.end())
	{
		this->_stack.erase(i, this->_stack.end());
	}
}

template<class type>
inline int Stack<type>::Size()
{
	return this->_stack.size();
}
