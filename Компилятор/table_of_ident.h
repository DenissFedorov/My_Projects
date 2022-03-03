#ifndef TABLE
#define TABLE
#include <iostream>
#include "identificator.h"

using namespace std;

class Table_of_id {
	Ident * id;
	int size;
	int top;
public:
	Table_of_id(int k)
	{
		id = new Ident[k];
		size = k;
		top = 1;
	}

	~Table_of_id()
	{
		delete []id;
	}

	Ident & operator[](int k)
	{
		return id[k];
	}

	int put(const char * buf)
	{
		for(int j=1; j<top; j++)
		{	
			if(strcmp(buf, id[j].get_name()) == 0)
				return j;
		}
		id[top].put_name(buf);
		top++;
		return top-1;
	}	
};
#endif
