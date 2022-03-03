#ifndef IDENT
#define IDENT
#include <iostream>
#include "lexems.h"

using namespace std;

class Ident {
	char * name = NULL;
	lexem_type type;
	int value;
	bool assign;
	bool declare;
public:
	Ident()
	{
		assign = false;
		declare = false;
	}

	~Ident()
	{
		delete []name;
	}

	char * get_name()
	{
		return name;
	}

	void put_name(const char * str)
	{
		name = new char[strlen(str) + 1];
		strcpy(name, str);

	}
	
	lexem_type get_type()
	{
		return type;
	}
	
	void put_type(lexem_type lex)
	{
		type = lex;
	}

	bool get_assign()
	{
		return assign;
	}

	void put_assign()
	{
		assign = true;
	}

	bool get_declare()
	{
		return declare;
	}

	void put_declare()
	{
		declare = true;
	}

	int get_value()
	{
		return value;
	}
	
	void put_value(int val)
	{
		value = val;
	}
};	
#endif
