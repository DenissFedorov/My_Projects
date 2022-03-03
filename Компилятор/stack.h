#ifndef STACK
#define STACK
#include <iostream>

using namespace std;

template <class A, int size> class Stack {
	A st[size];
	int top;
public:
	Stack()
	{
		top = 0;
	}

	void print()
	{
		cout << "**********\n";
		for(int i=0; i<top; i++)
			cout << st[i] << " ";
		cout<<endl;
		cout << "**********\n";
	}	

	void reset()
	{
		top = 0;
	}

	void push(A el)
	{
		if(!is_full())
		{
			st[top] = el;
			top++;
		}
		else
		{
			throw "Stack is full";
		}		
	}

	A pop()
	{
		if(!is_empty())
		{
			top--;
			return st[top];
		}
		else
			throw "Stack is empty";
	}
	
	bool is_empty()
	{
		return top == 0;
	}

	bool is_full()
	{
		return top == size;
	}
};
#endif
