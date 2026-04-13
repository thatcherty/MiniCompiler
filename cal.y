%code requires {
    #include "ast.h"
}

%{
#include <string>
#include <iostream>
#include <cstdio>
#include "ast.h"

int yylex();
extern FILE* yyin;

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
	double dub;
	char* txt;
	ExpNode* exp;
	StmtNode* stmt;
	BlockNode* block;
}

%token <num> NUMBER
%token <dub> DOUBLE
%token <txt> ID

%type <exp> exp
%type <stmt> input
%type <block> stmt_list stmt if_stmt while_stmt decl_stmt assign_stmt print_stmt

%token LE GE EQ NE IF WHILE SIN COS TAN PRINT ELSE INT FLOAT OR AND
%token '(' ')' '!' '%' '=' '{' '}' ',' '>' '<' '^'

%left OR
%left AND
%left EQ NE
%left '>' '<' LE GE 
%left '+' '-'
%left '*' '/' '%'
%right '^'
%right UMINUS
%right NOT
%left '!'
%left PARAN


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
	print_stmt
	{
		$$ = new BlockNode();
		$$->add($1);
	}
	;

if_stmt:
	IF '(' exp ')' '{' stmt_list '}' ';'
	{
		$$ = new IfNode($3, $6);
	}
	|
	IF '(' exp ')' '{' stmt_list '}' ELSE '{' stmt_list '}' ';'
	{
		$$ = new IfNode($3, $6, $10);
	}
	;

while_stmt:
	WHILE '(' exp ')' '{' stmt_list '}' ';'
	{
		$$ = new WhileNode($3, $6);
	}
	;

decl_stmt:
	INT ID '=' exp ';'
	{
		std::string temp = $2;
		$$ = new DeclIntNode(temp, $4);
	}
	|
	FLOAT ID '=' exp ';'
	{
		std::string temp = $2;
		$$ = new DeclDubNode(temp, $4);
	}
	;

assign_stmt:
	ID '=' exp ';'
	{
		std::string temp = $1;
		$$ = new AssignNode(temp, $3);
	}
	;

print_stmt:
	PRINT '(' exp ')' ';'
	{
		$$ = new OutNode($3);
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
	'!' exp %prec NOT
	{
		$$ = new NotNode($2);
	}
	|
	exp '!'
	{
		$$ = new FactNode($1);
	}
	|
	exp '^' exp
	{
		$$ = new ExponNode($1, $3);
	}
	|
	exp OR exp
	{
		$$ = new OrNode($1, $3);
	}
	|
	exp AND exp
	{
		$$ = new AndNode($1, $3);
	}
	|
	exp '%' exp
	{
		$$ = new ModNode($1, $3);
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
	|
	ID
	{
		std::string temp = $1;
		$$ = new IdNode(temp);
	}
	;

%%

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		yyin = fopen(argv[1], "r");
		if (!yyin)
		{
			perror("Failed to open input file");
			return 1;
		}
	}
	
	if (yyparse() == 0 && root != nullptr)
	{
		root->exe(st);
		delete root;
	}

	if (argc > 1 && yyin)
	{
		fclose(yyin);
	}
	
	return 0;
	
}
