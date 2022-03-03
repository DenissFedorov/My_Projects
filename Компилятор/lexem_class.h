#ifndef LEX
#define LEX
#include <iostream>
#include "lexems.h"

using namespace std;

class Lexem {
	lexem_type t_lex;
	int v_lex;
public:
	Lexem(lexem_type t = LEX_NULL, int v = 0): t_lex(t), v_lex(v) {}
	lexem_type get_type(){
		return t_lex;
	}
	int get_value(){
		return v_lex;
	}
	friend ostream & operator<<(ostream &out, Lexem lex)
	{
		out <<'('<< lex.t_lex <<','<< lex.v_lex <<')'<< endl;
		return out;
	}	
};
#endif
