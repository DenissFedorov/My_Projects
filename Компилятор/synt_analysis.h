#ifndef SYNT
#define SYNT
#include <iostream>
#include "stack.h"
#include "lexem_class.h"
#include "scan.h"
#include "poliz.h"
#include "class_of_err.h"

using namespace std;

class Analyzer {
	Lexem cur_lex;
	lexem_type cur_type;
	int cur_value;
	Scan scan;
	Stack <int, 100> stack_int;
	Stack <lexem_type, 100> stack_lex;
/*
 * Введем следующие замены (для простоты):
 * программа = PR
 * описания = DS
 * описание = D
 * переменная = V
 * константа = C
 * целочисленная = Z 
 * знак = SN
 * логическая = LG
 * строковая = ST
 * операторы = OS
 * оператор = O
 * составной оператор = MO
 * оператор-выражение = OE
 * выражение = E
*/
	void PR();
	void DS();
	void D();
	void OS();
	void O();
	void E();
	void E1();
	void E2();
	void E3();

	void dec(lexem_type t) //Занести в таблицу идентиф.
	{
		int i;
		while(!stack_int.is_empty())
		{
			i = stack_int.pop();
			if(TID[i].get_declare())
			{
				scan.close_file();
				scan.err.add_mes("Twice declare of ", TID[i].get_name());	
				throw scan.err;
			}	
			else
			{
				TID[i].put_declare();
				TID[i].put_type(t);
			}
		}
	}

	void check_id() //Описана ли переменная в TID?
	{
		if(TID[cur_value].get_declare() != 0)
		{
			stack_lex.push(TID[cur_value].get_type());
		}	
		else
		{
			scan.close_file();
			scan.err.add_mes("Not declared of ", TID[cur_value].get_name());	
			throw scan.err;
		}
	}

	void check_op() //Соотв.типов + преобраз.стека лексем
	{
		lexem_type op1, op2, act, control_1= LEX_INT, control_2 = LEX_BOOL;
		op2 = stack_lex.pop();
		if(stack_lex.is_empty())
		{	
			stack_lex.push(op2);
		}	
		else
		{
			act = stack_lex.pop();
			if(stack_lex.is_empty())
			{
				stack_lex.push(act);
				stack_lex.push(op2);
			}
			else
			{	
				op1 = stack_lex.pop();
				if(act == LEX_PLUS || act == LEX_MINUS || act == LEX_TIMES || act == LEX_SLASH)
					control_2 = LEX_INT;
				if(act == LEX_OR || act == LEX_AND)
				{
					control_1 = LEX_BOOL;
				}
				if((op1 == op2) && (op1 == control_1))
				{
					stack_lex.push(control_2);
				}	
				else
				{
					scan.close_file();
					scan.err.add_mes("Error: wrong types");
					throw scan.err;
				}	
				prog.put_lex(Lexem(act));
			}	
		}	
	}

	void check_not()
	{
		if(stack_lex.pop() != LEX_BOOL)
		{
			scan.close_file();
			scan.err.add_mes("Error: wrong type is in not");
			throw scan.err;
		}	
		else
		{	
			stack_lex.push(LEX_BOOL);
		}	
		prog.put_lex(Lexem(LEX_NOT));
	}

	void eq_type()
	{
		lexem_type lex = stack_lex.pop();
		if(lex != stack_lex.pop())
		{
			scan.close_file();
			scan.err.add_mes("Error: wrong types");
			throw scan.err;
		}	
	}

	void eq_bool()
	{
		if(stack_lex.pop() != LEX_BOOL)
		{
			scan.close_file();
			scan.err.add_mes("Error: expression isn't boolean");
			throw scan.err;
		}	
	}

	void check_id_in_read()
	{
		if(TID[cur_value].get_declare())
		{
			scan.close_file();
			scan.err.add_mes("Error: not declare of ", TID[cur_value].get_name());
			throw scan.err;
		}	
	}	

	void gl()
	{
		cur_lex = scan.get_lexem();
		cur_type = cur_lex.get_type();
		cur_value = cur_lex.get_value();
	}
public:
	int raw_err = 1;
	int stb_err = 1;
	Poliz prog;
	Analyzer(const char * pr): scan(pr), prog(100) {}
	void make_analysis()
	{
		gl();
		PR();
		prog.print();
		cout << endl << "Finish analysis. Success!\n\n";
	}
};

void Analyzer::PR()
{
	if(cur_type != LEX_PROGRAM)
	{
		scan.close_file();
		scan.err.add_mes("Error: expected LEX_PROGRAM, unexpected lexem is ", cur_lex);
		throw scan.err;
	}	
	gl();
	if(cur_type != LEX_LFIGURE)
	{
		scan.close_file();
		scan.err.add_mes("Error: expected LEX_LFIGURE, unexpected lexem is ", cur_lex);	
		throw scan.err;
	}	
	gl();
	DS();
	OS();
	if(cur_type != LEX_RFIGURE)
	{
		scan.close_file();
		scan.err.add_mes("Error: unexpected lexem is ", cur_lex);
		throw scan.err;
	}
	gl();	
	if(cur_type != LEX_FIN)
	{	
		scan.close_file();
		scan.err.add_mes("Error: expected LEX_FIN ", cur_lex);
		throw scan.err;
	}
	scan.close_file();
}

void Analyzer::DS()
{
	while(cur_type == LEX_INT || cur_type == LEX_STRING || cur_type == LEX_BOOL)
	{
		D();
		gl();
	}
}

void Analyzer::D()
{
	int type;
	if(cur_type == LEX_INT)
	{
		type = 0;
		gl();
		stack_int.push(cur_value);
		dec(LEX_INT);
	}
	else if(cur_type == LEX_BOOL)
	{
		type = 1;
		gl();
		stack_int.push(cur_value);
		dec(LEX_BOOL);
	}
	else
	{
		scan.close_file();
		scan.err.add_mes("Error: expected LEX_INT or LEX_BOOL, unexpected lexem is ", cur_lex);	
		throw scan.err;
	}	
	if(cur_type != LEX_ID)
	{
		scan.close_file();
		scan.err.add_mes("Error: expected LEX_ID, unexpected lexem is ", cur_lex);
		throw scan.err;
	}	
	else
	{
		gl();
		while(cur_type == LEX_COMMA)
		{
			gl();
			if(cur_type != LEX_ID)
			{
				scan.close_file();
				scan.err.add_mes("Error: expected LEX_ID, unexpected lexem is ", cur_lex);
				throw scan.err;
			}	
			else
			{
				stack_int.push(cur_value);
				if(type == 0)
					dec(LEX_INT);
				else if(type == 1)
					dec(LEX_BOOL);
				gl();
			}
		}
		if(cur_type != LEX_SEMICOLON)
		{
			scan.close_file();
			scan.err.add_mes("expected LEX_SEMICOLON, unexpected lexem is ", cur_lex);
			throw scan.err;
		}	
	}
}

void Analyzer::OS()
{
	while(cur_type == LEX_IF || cur_type == LEX_WHILE || cur_type == LEX_READ || cur_type == LEX_WRITE || cur_type == LEX_DO || cur_type == LEX_NOT || cur_type == LEX_ID)
	{
		O();
	}
}

void Analyzer::O()
{
	int a0, a1, a2, a3;
	if(cur_type == LEX_IF)
	{
		gl();
		E();
		eq_bool(); //Будет ли рез-т выражения логическим?
		a2 = prog.get_free_place();
		prog.add_free_place();
		prog.put_lex(Lexem(POLIZ_FGO));
		if(cur_type == LEX_LFIGURE)
                {
                        gl();
                        while(cur_type != LEX_RFIGURE)
                                O();
                        gl();
                }
                else
		{
                        O();
		}	
		a3 = prog.get_free_place();
		prog.add_free_place();
		prog.put_lex(Lexem(POLIZ_GO));
		prog.put_lex(Lexem(POLIZ_LABEL, prog.get_free_place()), a2); 
		if(cur_type == LEX_ELSE)
		{
			gl();
			O();
			prog.put_lex(Lexem(POLIZ_LABEL, prog.get_free_place()), a3);
		}
		else
		{
			scan.close_file();
			scan.err.add_mes("Error: expected LEX_ELSE, unexpected lexem is ", cur_lex);	
			throw scan.err;
		}	
//		stack_lex.pop();	
	}
	else if(cur_type == LEX_WHILE)
	{
		a0 = prog.get_free_place();
		gl();
		E();
		eq_bool();
		a1 = prog.get_free_place();
		prog.add_free_place();
		prog.put_lex(Lexem(POLIZ_FGO));
		if(cur_type == LEX_LFIGURE)
		{
			gl();
			while(cur_type != LEX_RFIGURE)
			{
				O();
			}	
			gl();
		}
		else
			O();
		prog.put_lex(Lexem(POLIZ_LABEL, a0));
		prog.put_lex(Lexem(POLIZ_GO));
		prog.put_lex(Lexem(POLIZ_LABEL, prog.get_free_place()), a1);
	}	
	else if(cur_type == LEX_WRITE)
	{
		gl();
		if(cur_type == LEX_LPAREN)
		{
			gl();
			E();
			if(cur_type == LEX_RPAREN)
			{
				gl();
				prog.put_lex(Lexem(LEX_WRITE));
				if(cur_type == LEX_SEMICOLON)
				{
					gl();
				}
				else
				{
					scan.close_file();
					scan.err.add_mes("Error: expected LEX_SEMICOLON, unexpected lexem is ", cur_lex);
					throw scan.err;
				}
				stack_lex.reset();
			}	
			else
			{
				scan.close_file();
				scan.err.add_mes("Error: expected LEX_RPAREN, unexpected lexem is ", cur_lex);
				throw scan.err;
			}	
		}
		else
		{
			scan.close_file();
			scan.err.add_mes("Error: expected LEX_LPAREN, unexpected lexem is ", cur_lex);
			throw scan.err;
		}
	}
	else if(cur_type == LEX_READ)
	{
		gl();
		if(cur_type == LEX_LPAREN)
		{
			gl();
			if(cur_type == LEX_ID)
			{
				check_id_in_read();
				prog.put_lex(Lexem(POLIZ_ADDRESS, cur_value));
				gl();
			}	
			else
			{
				scan.close_file();
				scan.err.add_mes("Error: expected LEX_ID, unexpected lexem is ", cur_lex);
				throw scan.err;
			}	
			if(cur_type == LEX_RPAREN)
			{
				gl();
				prog.put_lex(Lexem(LEX_READ));
			}
			else
			{
				scan.close_file();
				scan.err.add_mes("Error: expected LEX_RPAREN, unexpected lexem is ", cur_lex);	
				throw scan.err;
			}
		}
		else
		{
			scan.close_file();
			scan.err.add_mes("Error: expected LEX_LPAREN, unxepected lexem is ", cur_lex);
			throw scan.err;
		}	
	}	
	else if(cur_type == LEX_ID)
	{
		check_id();
		prog.put_lex(Lexem(POLIZ_ADDRESS, cur_value));
		gl();
		if(cur_type == LEX_ASSIGN)
		{
			gl();
			E();
			eq_type();
			prog.put_lex(Lexem(LEX_ASSIGN));
			if(cur_type != LEX_SEMICOLON)
			{
				scan.close_file();
				scan.err.add_mes("Error: expected LEX_SEMICOLON, unexpected lexem is ", cur_lex);
				throw scan.err;
			}	
			gl();
		}
		else
		{
			scan.close_file();
			scan.err.add_mes("Error: expected LEX_ASSIGN, unexpected lexem is ", cur_lex);	
			throw scan.err;
		}	
	}
	else 
		OS();	
				
}

void Analyzer::E()
{
	E1();
	if(cur_type == LEX_EQ || cur_type == LEX_LSS || cur_type == LEX_LEQ || cur_type == LEX_GTR || cur_type == LEX_GEQ || cur_type == LEX_NEQ)
	{
		stack_lex.push(cur_type);
		gl();
		E();
		check_op();
	}
}

void Analyzer::E1()
{
	E2();
	while(cur_type == LEX_PLUS || cur_type == LEX_MINUS || cur_type == LEX_OR)
	{
		check_op();
		stack_lex.push(cur_type);
		gl();
		E();
		check_op();
	}
}

void Analyzer::E2()
{
	E3();
	while(cur_type == LEX_TIMES || cur_type == LEX_SLASH || cur_type == LEX_AND)
	{
		check_op();
		stack_lex.push(cur_type);
		gl();
		E();
		check_op();
	}
}

void Analyzer::E3()
{
	if(cur_type == LEX_ID) //здесь проверка
	{
		check_id();
		prog.put_lex(Lexem(LEX_ID, cur_value));
		gl();
	}
	else if(cur_type == LEX_NUM)
	{
		stack_lex.push(LEX_INT);
		prog.put_lex(cur_lex);
		gl();
	}
	else if(cur_type == LEX_TRUE)
	{
		stack_lex.push(LEX_BOOL);
		prog.put_lex(Lexem(LEX_TRUE, 1));
		gl();
	}
	else if(cur_type == LEX_FALSE)
	{
		stack_lex.push(LEX_BOOL);
		prog.put_lex(Lexem(LEX_FALSE, 0));
		gl();
	}
	else if(cur_type == LEX_NOT)
	{
		gl();
		E3();
		check_not();
	}
	else if(cur_type == LEX_LPAREN)
	{
		gl();
		E();
		if(cur_type == LEX_RPAREN)
			gl();
		else
		{
			scan.close_file();
			scan.err.add_mes("Error: expected LEX_RPAREN, unexpected lexem is ", cur_lex);
			throw scan.err;
		}
	}
	else
	{
		scan.close_file();
		scan.err.add_mes("Error: unexpected lexem is ", cur_lex);	
		throw scan.err;
	}	
}

#endif
