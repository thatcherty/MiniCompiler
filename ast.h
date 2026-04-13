#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>

using namespace std;

// struct because no need for private members
struct Var
{	
	virtual ~Var() = default;
	bool init = false;
	virtual double getVal() = 0;
	virtual void setVal(double v) = 0;
};

struct IntVar : Var
{
	int val;

	IntVar(int v, bool i)
	{
		val = v;
		init = i;
	}
	
	double getVal() override
	{
		return val;
	}

	void setVal(double v) override
	{
		val = v;
	}
};

struct DubVar : Var
{
	double val;

	DubVar(double v, bool i)
	{
		val = v;
		init = i;
	}
	
	double getVal() override
	{
		return val;
	}
	
	void setVal(double v) override
	{
		val = v;
	}
};

struct SymTbl
{
	unordered_map<string, Var*> sym_tbl;
};

struct Node
{
	virtual ~Node() {};
};

struct ExpNode : Node
{
	virtual double eval(SymTbl& st) = 0;
};

struct StmtNode : Node
{
	virtual void exe(SymTbl& st) = 0;
};

struct BlockNode : StmtNode
{
	vector<StmtNode*> stmts;

	void add(StmtNode* s)
	{
		stmts.push_back(s);
	}

	void exe(SymTbl& st) override
	{
		for (StmtNode* s : stmts)
		{
			s->exe(st);
		}
	}

	~BlockNode() override
	{
		for (StmtNode* s : stmts)
		{
			delete s;
		}	
	}
};

struct NumNode : ExpNode 
{
	int value;

	NumNode(int n)
	{
		value = n;
	};

	double eval(SymTbl& st) override 
	{
		return value;
	};
};

struct DubNode : ExpNode 
{
	double value;

	DubNode(double n)
	{
		value = n;
	};

	double eval(SymTbl& st) override 
	{
		return value;
	};
};

struct IdNode : ExpNode
{
	string val;

	IdNode(string id)
	{
		val = id;
	}

	double eval(SymTbl& st) override
	{
		return st.sym_tbl[val]->getVal();
	}

};

struct ParanNode : ExpNode
{
	ExpNode* node;

	ParanNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return node->eval(st);
	}

	~ParanNode() override
	{
		delete node;
	}
};

struct AddNode : ExpNode 
{
	ExpNode* left;
	ExpNode* right;

	AddNode(ExpNode* l, ExpNode *r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) + right->eval(st);
	};

	~AddNode() override
	{
		delete left;
		delete right;
	};
	
};

struct SubNode : ExpNode 
{
	ExpNode* left;
	ExpNode* right;

	SubNode(ExpNode* l, ExpNode *r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) - right->eval(st);
	};

	~SubNode() override
	{
		delete left;
		delete right;
	};
	
};

struct MultNode : ExpNode 
{
	ExpNode* left;
	ExpNode* right;

	MultNode(ExpNode* l, ExpNode *r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) * right->eval(st);
	};

	~MultNode() override
	{
		delete left;
		delete right;
	};
	
};

struct DivNode : ExpNode 
{
	ExpNode* left;
	ExpNode* right;

	DivNode(ExpNode* l, ExpNode *r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		double r = right->eval(st);
		if (r == 0)
		{
		    throw runtime_error("Divide by zero");
		}
		return left->eval(st) / r;
	};

	~DivNode() override
	{
		delete left;
		delete right;
	};
	
};

struct SinNode : ExpNode 
{
	ExpNode* node;

	SinNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return sin(node->eval(st));
	};

	~SinNode() override
	{
		delete node;
	};
	
};

struct TanNode : ExpNode 
{
	ExpNode* node;

	TanNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return tan(node->eval(st));
	};

	~TanNode() override
	{
		delete node;
	};
	
};

struct CosNode : ExpNode 
{
	ExpNode* node;

	CosNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return cos(node->eval(st));
	};

	~CosNode() override
	{
		delete node;
	};
	
};

struct NegNode : ExpNode
{
	ExpNode* node;

	NegNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return -1 * node->eval(st);
	};

	~NegNode() override
	{
		delete node;
	};
};

struct NotNode : ExpNode
{
	ExpNode* node;

	NotNode(ExpNode* n)
	{
		node = n;
	};

	double eval(SymTbl& st) override
	{
		return !( node->eval(st));
	};

	~NotNode() override
	{
		delete node;
	};
};

struct ExponNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	ExponNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return pow(left->eval(st), right->eval(st));
	};

	~ExponNode() override
	{
		delete left;
		delete right;
	};
};

struct GreaterNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	GreaterNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) > right->eval(st);
	};

	~GreaterNode() override
	{
		delete left;
		delete right;
	};
};

struct LesserNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	LesserNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) < right->eval(st);
	};

	~LesserNode() override
	{
		delete left;
		delete right;
	};
};

struct GreaterEqNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	GreaterEqNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) >= right->eval(st);
	};

	~GreaterEqNode() override
	{
		delete left;
		delete right;
	};
};

struct LesserEqNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	LesserEqNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) <= right->eval(st);
	};

	~LesserEqNode() override
	{
		delete left;
		delete right;
	};
};

struct EqualNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	EqualNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) == right->eval(st);
	};

	~EqualNode() override
	{
		delete left;
		delete right;
	};
};

struct NotEqualNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	NotEqualNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) != right->eval(st);
	};

	~NotEqualNode() override
	{
		delete left;
		delete right;
	};
};

struct OrNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	OrNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) || right->eval(st);
	};

	~OrNode() override
	{
		delete left;
		delete right;
	};
};

struct AndNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	AndNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		return left->eval(st) && right->eval(st);
	};

	~AndNode() override
	{
		delete left;
		delete right;
	};
};

struct ModNode : ExpNode
{
	ExpNode* left;
	ExpNode* right;

	ModNode(ExpNode* l, ExpNode* r)
	{
		left = l;
		right = r;
	};

	double eval(SymTbl& st) override
	{
		double l = left->eval(st);
        double r = right->eval(st);

        if (r == 0)
        {
            throw runtime_error("Modulo by zero");
        }

        return fmod(l, r);	
     };

	~ModNode() override
	{
		delete left;
		delete right;
	};
};

struct FactNode : ExpNode
{
	ExpNode* left;

	FactNode(ExpNode* l)
	{
		left = l;
	};

	double eval(SymTbl& st) override
	{
		double l = left->eval(st);

        if (l < 0)
        {
            throw runtime_error("Factorial undefined for negative values");
        }
		
        return tgamma(l + 1);	
     };

	~FactNode() override
	{
		delete left;
	};
};

struct IfNode : BlockNode
{
	ExpNode* cond;
	StmtNode* i_stmt;
	StmtNode* e_stmt;

	// for if stmt only
	IfNode(ExpNode* c, StmtNode* s)
	{
		cond = c;
		i_stmt = s;
		e_stmt = nullptr;
	};

	// if else support
	IfNode(ExpNode* c, StmtNode* s, StmtNode* e)
	{
		cond = c;
		i_stmt = s;
		e_stmt = e;
	};

	void exe(SymTbl& st) override
	{
		if (cond->eval(st))
		{
			i_stmt->exe(st);
		}
		else if (e_stmt != nullptr)
		{
			e_stmt->exe(st);
		}
		
	};
	
	~IfNode() override
	{
		delete cond;
		delete i_stmt;
		delete e_stmt;
	};

};

struct WhileNode : BlockNode
{
	ExpNode* cond;
	StmtNode* stmt;

	WhileNode(ExpNode* c, StmtNode* s)
	{
		cond = c;
		stmt = s;
	};

	void exe(SymTbl& st) override
	{
		while (cond->eval(st))
		{
			stmt->exe(st);
		}
		
	};
	
	~WhileNode() override
	{
		delete cond;
		delete stmt;
	};

};

struct OutNode : BlockNode
{
	ExpNode* exp;
	
	OutNode(ExpNode* e)
	{
		exp = e;
	};

	void exe(SymTbl& st) override
	{
		double res = exp->eval(st);

		if (fabs(res) < 1e-12)
		{
			cout << 0.0 << endl;
		}
		else
		{
			cout << res << endl;
		}
	};

	~OutNode() override
	{
		delete exp;
	};
};

struct DeclIntNode : BlockNode
{
	string id;
	ExpNode* exp;
	
	DeclIntNode(string i, ExpNode* e)
	{
		id = i;
		exp = e;
	};

	void exe(SymTbl& st) override
	{
	
		if (st.sym_tbl.find(id) != st.sym_tbl.end())
		{
			throw runtime_error("Variable already declared."); 
		}

		st.sym_tbl[id] = new IntVar(exp->eval(st), true);

	};

	~DeclIntNode() override
	{
		delete exp;
	}

};

struct DeclDubNode : BlockNode
{
	string id;
	ExpNode* exp;
	
	DeclDubNode(string i, ExpNode* e)
	{
		id = i;
		exp = e;
	};

	void exe(SymTbl& st) override
	{
	
		if (st.sym_tbl.find(id) != st.sym_tbl.end())
		{
			throw runtime_error("Variable already declared."); 
		}

		st.sym_tbl[id] = new DubVar(exp->eval(st), true);
	};

	~DeclDubNode() override
	{
		delete exp;
	}

};



struct AssignNode : BlockNode
{
	string id;
	ExpNode* exp;
	
	AssignNode(string i, ExpNode* e)
	{
		id = i;
		exp = e;
	};

	void exe(SymTbl& st) override
	{
	
		if (st.sym_tbl.find(id) == st.sym_tbl.end())
		{
			throw runtime_error("Variable not declared."); 
		}

		st.sym_tbl[id]->setVal(exp->eval(st));
	};

	~AssignNode() override
	{
		delete exp;
	}

};
