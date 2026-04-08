%code requires {
    #include "ast.h"
}

%{
#include <iostream>
#include "ast.h"

using namespace std;

int yylex();

void yyerror(const char* s)
{
	printf("%s\n", s);
}

// ast root
StmtNode* root = nullptr;
SymTbl st {};
	
%}

%union {
	int num;
	float dub;
	char* id;
	ExpNode* exp;
	StmtNode* stmt;
	BlockNode* block;
}

%token <num> NUMBER
%token <dub> DOUBLE
%token <id> ID

%type <exp> exp
%type <stmt> input
%type <block> stmt_list stmt if_stmt while_stmt decl_stmt assign_stmt

%token LE GE EQ NE IF WHILE SIN COS TAN PRINT ELSE INT FLOAT CHAR
%token '(' ')' '!' '%' '=' '{' '}' ',' '>' '<' '^'

%left '>' '<' LE GE EQ NE '!'
%left '+' '-'
%left '*' '/'
%left '^'
%left PARAN
%right UMINUS

%start input

%%

input:
	stmt_list
	{
		root = $1;
	}
	;

stmt_list:
	stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	|
	stmt_list stmt
	{
		$1->add($2);
		$$ = $1;
	}
	;

stmt:
	if_stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	|
	while_stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	|
	decl_stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	|
	assign_stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	|
	PRINT '(' exp ')' ';'
	{
		$$ = new OutNode($3);
	}
	;

if_stmt:
	IF '(' exp ')' '{' stmt '}' ';'
	{
		$$ = new IfNode($3, $6);
	}
	|
	IF '(' exp ')' '{' stmt '}' ELSE '{' stmt '}' ';'
	{
		$$ = new IfNode($3, $6, $10);
	}
	;

while_stmt:
	WHILE '(' exp ')' '{' stmt '}' ';'
	{
		$$ = new WhileNode($3, $6);
	}
	;

decl_stmt:
	INT ID '=' exp ';'
	{
		printf("Found id in decl_stmt: %s\n", $2);
	}
	;

assign_stmt:
	ID '=' exp ';'
	{
		printf("Found id in assign_stmt: %s\n", $1);
	}
	;

exp:
	'(' exp ')' %prec PARAN
	{
		$$ = new ParanNode($2);	
	}
	|
	exp '+' exp
	{
		$$ = new AddNode($1, $3);
	}
	|
	exp '-' exp
	{
		$$ = new SubNode($1, $3);
	}
	|
	exp '*' exp
	{
		$$ = new MultNode($1, $3);
	}
	|
	exp '/' exp
	{
		$$ = new DivNode($1,$3);
	}
	|
	SIN '(' exp ')'
	{
		$$ = new SinNode($3);
	}
	|
	TAN '(' exp ')'
	{
		$$ = new TanNode($3);
	}
	|
	COS '(' exp ')'
	{
		$$ = new CosNode($3);
	}
	|
	'-' exp %prec UMINUS
	{
		$$ = new NegNode($2);
	}
	|
	exp '>' exp
	{
		$$ = new GreaterNode($1, $3);
	}
	|
	exp '<' exp
	{
		$$ = new LesserNode($1, $3);
	}
	|
	exp GE exp
	{
		$$ = new GreaterEqNode($1, $3);
	}
	|
	exp LE exp
	{
		$$ = new LesserEqNode($1, $3);
	}
	|
	exp EQ exp
	{
		$$ = new EqualNode($1, $3);
	}
	|
	exp NE exp
	{
		$$ = new NotEqualNode($1, $3);
	}
	|
	'!' exp
	{
		$$ = new NotNode($2);
	}
	|
	exp '^' exp
	{
		$$ = new ExponNode($1, $3);
	}
	|
	NUMBER
	{
		$$ = new NumNode($1);
	}
	|
	DOUBLE
	{
		$$ = new DubNode($1);
	}
	;

%%

int main()
{
	if (yyparse() == 0 && root != nullptr)
	{
		root->exe(st);
		delete root;
	}
	return 0;
	
}
