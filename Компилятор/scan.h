#ifndef SCAN
#define SCAN
#include <iostream>
#include <cstring>
#include "lexems.h"
#include "table_of_ident.h"
#include "class_of_err.h"

using namespace std;

Table_of_id TID(100);

class Scan {
	enum state {H, CHISLO, ID, SOST_OP, SRAVN, NE_RAVNO, SYMBOL};
	FILE * fp = NULL;
	char c;
	int scobka = 0;
	state cur_st;
	char buf[80];
	int top_buf; //Номер, куда можно занести эл-т
public:
	Error err;
	static const char * TW[];
        static const char * TD[];
	static lexem_type main_words[], other_words[];
	
	void clear() //Очистить буфер
	{
		top_buf = 0;
		for(int i=0; i<80; i++)
			buf[i]='\0';
	}
	
	void add_buf() //Добавить в буфер
	{
		buf[top_buf] = c;
		top_buf++;
	}
	
	int look_buf(const char * buf, const char ** list)
	{ // Найти номер эл-та в массиве эл-тов
		int i = 0;
		while(list[i] != NULL)
		{
			if(strcmp(buf, list[i]) == 0)
				return i;
			i++;
		}
		return 0;
	}

	void gc()
	{
		c = fgetc(fp);
		err.add_col();
	}

	Scan(const char * s)
	{
		if((fp = fopen(s, "r")) == 0 )
		{
			err.set_row(0);
			err.add_mes("Error: can't open this file");
		       	throw err;
		}	
		cur_st = H;
		clear();
		gc();
	}

	void close_file()
	{
		fclose(fp);
	}

	Lexem get_lexem()
	{
		if(err.get_row() != 1)
		{	
			err.set_curplace(err.get_col() + 1);
		}
		else
		{
			err.set_curplace(err.get_col());
		}	
		int cur_ch;
		int err_num = 0;
		int j;
		cur_st = H;
		do
		{
			if(c == EOF)
			{
				return Lexem();
			}
			switch(cur_st)
			{
			case H:	
				if(c == ' ' || c == '\n' || c =='\r' || c == '\t')
				{
					if(c == '\n')
					{
						err.add_row();
						err.set_col(0);
					}
					gc();
				}	
				else if(isdigit(c) != 0)
				{
					cur_st = CHISLO;
					cur_ch = c - '0';
					gc();
				}
				else if(isalpha(c) != 0)
				{
					cur_st = ID;
					clear();
					add_buf();
					gc();
				}
				else if(c == '}' && scobka > 0)
				{
					scobka--;
					clear();
					add_buf();
					gc();
					j = look_buf(buf, TD);
					return Lexem(other_words[j], j);
				}
				else if(c == '}' && scobka==0)
				{
					err.add_mes("Error: Curly braces mismatch");
					throw err;
				}	
				else if(c == '{')
				{
					scobka++;
					cur_st = SOST_OP;
					clear();
					add_buf();
					gc();
				}
				else if(c == ':' || c == '<' || c == '>')
				{
					cur_st = SRAVN;
					clear();
					add_buf();
					gc();
				}
				else if(c == '@')
					return Lexem(LEX_FIN);
				else if(c == '!')
				{
					cur_st = NE_RAVNO;
					clear();
					add_buf();
					gc();
				}
				else
					cur_st = SYMBOL;
				break;
			case CHISLO:
				if(isdigit(c) != 0)
				{
					cur_ch = 10*cur_ch + c - '0';
					gc();
				}
				else 
					return Lexem(LEX_NUM, cur_ch);
				break;	
			case ID:
				if(isdigit(c) || isalpha(c))
				{
					add_buf();
					gc();
				}
				else if(j = look_buf(buf, TW)) // Если это спец. слово
				{
					return Lexem(main_words[j] , j);
				}	
				else
				{
					j = TID.put(buf);
					return Lexem(LEX_ID, j);
				}
				break;
			case SOST_OP:
				if(c == '@' || c == '{')
				{

					err.add_mes("Unexpected symbol ", c);
					throw err;
				}	
				else
				{
					j = look_buf(buf, TD);
					return Lexem(other_words[j], j);
				}	
				break;
			case SRAVN:
				if(c == '=')
				{
					add_buf();
					gc();
					j = look_buf(buf, TD);
					return Lexem(other_words[j], j);
				}
				else
				{
					j = look_buf(buf, TD);
					return Lexem(other_words[j], j);
				}
				break;
			case NE_RAVNO:
				if(c == '=')
				{
					add_buf();
					gc();
					j = look_buf(buf, TD);
					return Lexem(LEX_NEQ, j);
				}
				else
				{
					err.add_mes("Expected symbol =");
					throw err;
				}	
				break;
			case SYMBOL:
				clear();
				add_buf();
				if(j = look_buf(buf, TD))
				{
					gc();
					return Lexem(other_words[j], j);
				}
				else
				{
					err.add_mes("Unexpected symbol ", c);
					throw err;
				}
				break;
			}
		}
		while(1);
	}
};	

lexem_type Scan::main_words[] = {LEX_NULL, LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT, LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_STRING, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_NULL};

lexem_type Scan::other_words[] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_LFIGURE, LEX_RFIGURE, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_NULL};

const char * Scan::TW[] = {"", "and", "begin", "boolean", "do", "else", "end", "if", "false", "int", "not", "or", "program", "read", "string", "then", "true", "var", "while", "write", NULL};

const char * Scan::TD[] = {"", "@", ";", ",", ":", "=", "(", ")", "\{", "}", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL};

#endif
