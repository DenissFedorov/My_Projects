#include <iostream>
#include "processing_poliz.h"
#include "class_of_err.h"
#include "lexem_class.h"
#include "scan.h"

using namespace std;

int main(int argc, char ** argv)
{
	try
	{
		Make_Interp A((char *)argv[1]);
		A.interpret();
		return 0;
	}

	catch(Error err)
	{
		if(err.get_row() != 1 || err.get_col() != 0)
		{	
			cout<< "row: " << err.get_row() << endl;
			cout<< "col: " << err.get_curplace() << endl;
		}
		cout<< err.get_mes();
		if(err.get_lex().get_type() != LEX_NULL)
		{
			switch(err.get_lex().get_type()){
			case LEX_NULL:
                                cout << "LEX_NULL\n";
                                break;
                        case LEX_AND:
                                cout << "LEX_AND\n";
                                break;
                        case LEX_BEGIN:
                                cout << "LEX_BEGIN\n";
                                break;
                        case LEX_BOOL:
                                cout << "LEX_BOOL\n";
                                break;
                        case LEX_DO:
                                cout << "LEX_DO\n";
                                break;
                        case LEX_ELSE:
                                cout << "LEX_ELSE\n";
                                break;
                        case LEX_END:
                                cout << "LEX_END\n";
                                break;
                        case LEX_IF:
                                cout << "LEX_IF\n";
                                break;
                        case LEX_FALSE:
                                cout << "LEX_FALSE\n";
                                break;
                        case LEX_INT:
                                cout << "LEX_INT\n";
                                break;
                        case LEX_NOT:
                                cout << "LEX_NOT\n";
                                break;
                        case LEX_OR:
                                cout << "LEX_OR\n";
                                break;
			case LEX_PROGRAM:
                                cout << "LEX_PROGRAM\n";
                                break;
                        case LEX_READ:
                                cout << "LEX_READ\n";
                                break;
                        case LEX_THEN:
                                cout << "LEX_THEN\n";
                                break;
                        case LEX_TRUE:
                                cout << "LEX_TRUE\n";
                                break;
                        case LEX_VAR:
                                cout << "LEX_VAR\n";
                                break;
                        case LEX_WHILE:
                                cout << "LEX_WHILE\n";
                                break;
                        case LEX_WRITE:
                                cout << "LEX_WRITE\n";
                                break;
                        case LEX_FIN:
                                cout << "LEX_FIN\n";
                                break;
                        case LEX_SEMICOLON:
                                cout << "LEX_SEMICOLON\n";
                                break;
                        case LEX_STRING:
                                cout << "LEX_STRING\n";
                                break;
                        case LEX_COMMA:
                                cout << "LEX_COMMA\n";
                                break;
                        case LEX_COLON:
                                cout << "LEX_COLON\n";
                                break;
			case LEX_ASSIGN:
                                cout << "LEX_ASSIGN\n";
                                break;
                        case LEX_LPAREN:
                                cout << "LEX_LPAREN\n";
                                break;
                        case LEX_RPAREN:
                                cout << "LEX_RPAREN\n";
                                break;
                        case LEX_EQ:
                                cout << "LEX_EQ\n";
                                break;
                        case LEX_LSS:
                                cout << "LEX_LSS\n";
                                break;
                        case LEX_GTR:
                                cout << "LEX_GTR\n";
                                break;
                        case LEX_PLUS:
                                cout << "LEX_PLUS\n";
                                break;
                        case LEX_MINUS:
                                cout << "LEX_MINUS\n";
                                break;
                        case LEX_TIMES:
                                cout << "LEX_TIMES\n";
                                break;
                        case LEX_SLASH:
                                cout << "LEX_SLASH\n";
                                break;
                        case LEX_LEQ:
                                cout << "LEX_LEQ\n";
                                break;
                        case LEX_NEQ:
                                cout << "LEX_NEQ\n";
                                break;
			case LEX_GEQ:
                                cout << "LEX_GEQ\n";
                                break;
                        case LEX_NUM:
                                cout << "LEX_NUM\n";
                                break;
                        case LEX_ID:
                                cout << "LEX_ID\n";
                                break;
                        case POLIZ_LABEL:
                                cout << "POLIZ_LABEL\n";
                                break;
                        case POLIZ_ADDRESS:
                                cout << "POLIZ_ADDRESS\n";
                                break;
                        case POLIZ_GO:
                                cout << "POLIZ_GO\n";
                                break;
                        case POLIZ_FGO:
                                cout << "POLIZ_FGO\n";
                                break;
			}
		}
		else if(err.get_str() != NULL)
		{
			cout << err.get_str() << endl;
		}
		else if(err.get_sym() != 0)
		{
			cout << err.get_sym() << endl;
		}	
		return 1;
	}	
};
