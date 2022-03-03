#ifndef ERROR
#define ERROR
#include <iostream>
#include "lexem_class.h"

using namespace std;

class Error {
	int curplace;
	int row;
	int col;
	string mes;
	Lexem lex;
	char * str;
	char k;
public:
	Error()
	{
		row = 1;
		col = 0;
		curplace = 0;
		str = NULL;
	}

	Lexem get_lex()
	{
		return lex;
	}

	void set_curplace(int a)
	{
		curplace = a;
	}

	int get_curplace()
	{
		return curplace;
	}

	char get_sym()
	{
		return k;
	}

	char * get_str()
	{
		return str;
	}

	int get_row()
	{
		return row;
	}

	void set_row(int a)
	{
		row = a;
	}

	int get_col()
	{
		return col;
	}

	void set_col(int a)
	{
		col = a;
	}

	string get_mes()
	{
		return mes;
	}

	void add_mes(string a)
	{
		mes = a;
	}

	void add_mes(string a, Lexem B)
	{
		mes = a;
		lex = B;
	}

	void add_mes(string a, char c)
	{
		mes = a;
		k = c;
	}	

	void add_mes(string a, char * c)
	{	
		mes = a;
		str = c;
	}	

	void add_row()
	{
		row++;
	}

	void add_col()
	{
		col++;
	}
};	
		
#endif
