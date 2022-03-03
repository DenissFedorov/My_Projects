#include <iostream>
#include "stack.h"
#include "poliz.h"
#include "scan.h"
#include "synt_analysis.h"

class Interpreter {
	Lexem lex;
public:
	Error err;
	void interp(Poliz & prog)
	{
		Stack <int, 100> elem;
		int size = prog.get_free_place();
		int num_lex = 0;
		int cur_lex_val;
		int j;
		while(num_lex < size)
		{
			lex = prog[num_lex];
			switch(lex.get_type())
			{
				case POLIZ_ADDRESS:
				case POLIZ_LABEL:
				case LEX_NUM:
				case LEX_TRUE:
				case LEX_FALSE:
					elem.push(lex.get_value());
					break;
				case LEX_NOT:
					elem.push(!elem.pop());
					break;
				case LEX_AND:
					cur_lex_val = elem.pop();
					elem.push(elem.pop() && cur_lex_val);
					break;
				case LEX_OR:
					cur_lex_val = elem.pop();
					elem.push(elem.pop() || cur_lex_val);
					break;
				case LEX_ID:
					cur_lex_val = lex.get_value();
					if(TID[cur_lex_val].get_assign() != 0)
					{
						elem.push(TID[cur_lex_val].get_value());
						break;
					}
					else
					{
						err.add_mes("POLIZ: undefine id ", TID[cur_lex_val].get_name());
						throw err;
					}	
				case POLIZ_GO://Переход
					num_lex = elem.pop() - 1;
					break;
				case POLIZ_FGO:
					cur_lex_val = elem.pop();
					if(!elem.pop())
					{
						num_lex = cur_lex_val - 1;
					}	
					break;
				case LEX_WRITE:
					cout << elem.pop() << endl;
					break;
				case LEX_READ:
					int k;
					cur_lex_val = elem.pop();
					if(TID[cur_lex_val].get_type() == LEX_INT)
					{
						cout << "int ";
						cout << TID[cur_lex_val].get_name() << endl;
						cin >> k;
					}
					else
					{
						char b[6];
						cout << "boolean ";
						cout << TID[cur_lex_val].get_name() << endl;
						cin >> b;
						if(strcmp(b, "true") == 0)
							k = 1;
						else if(strcmp(b, "false") == 0)
							k = 0;
						else
						{
							err.add_mes("POLIZ:this value is not boolean");
							throw err;
						}	
					}
					TID[cur_lex_val].put_value(k);
					TID[cur_lex_val].put_assign();
					break;
				case LEX_PLUS:
					elem.push(elem.pop() + elem.pop());
					break;
				case LEX_MINUS:
					cur_lex_val = elem.pop();
					elem.push(elem.pop()-cur_lex_val);
					break;
				case LEX_TIMES:
					elem.push(elem.pop()*elem.pop());
					break;
				case LEX_SLASH:
					cur_lex_val = elem.pop();
					if(cur_lex_val != 0)
					{
						elem.push(elem.pop()/cur_lex_val);
						break;
					}
					else
					{
						err.add_mes("POLIZ: divide by zero");
						throw err;
					}	
				case LEX_EQ:
					elem.push(elem.pop() == elem.pop());
					break;
				case LEX_LSS:
					cur_lex_val = elem.pop();
					elem.push(elem.pop()<cur_lex_val);
					break;
				case LEX_LEQ:
					cur_lex_val = elem.pop();
					elem.push(elem.pop()<=cur_lex_val);
					break;
				case LEX_GTR:
					cur_lex_val = elem.pop();
					elem.push(elem.pop() > cur_lex_val);
					break;
				case LEX_GEQ:
					cur_lex_val = elem.pop();
					elem.push(elem.pop()>=cur_lex_val);
					break;
				case LEX_NEQ:
					cur_lex_val = elem.pop();
					elem.push(elem.pop()!=cur_lex_val);
					break;
				case LEX_ASSIGN:
					cur_lex_val = elem.pop();
					j = elem.pop();
					TID[j].put_value(cur_lex_val);
					TID[j].put_assign();
					break;
				default:
					err.add_mes("POLIZ: unexpected elem");
					throw err;
			}
			num_lex++;
		};
		cout<<"Program is end!!!\n";
	}
};

class Make_Interp {
	Analyzer an;
	Interpreter inter;
public:
	Make_Interp(char * p): an(p) {};
	void interpret()
	{
		an.make_analysis();
		inter.interp(an.prog);
	}
};


					




