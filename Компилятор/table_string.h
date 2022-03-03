#ifndef STRING
#define STRING
#include <iostream>
#include <cstring>

using namespace std;

class Table_string {
	string * str;
	int size;
	int top;
public:
	Table_string(int k)
	{
		str = new string[k];
		size = k;
		top = 1;
	}

	~Table_string()
	{
		delete []str;
	}

	string & operator[] (int k)
	{
		return str[k];
	}

	int put(string s)
	{
		str[top] = s;
		top ++;
	}
};

#endif
