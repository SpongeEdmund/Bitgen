#ifndef __LUT_CALC_H__
#define __LUT_CALC_H__

#include <stack>
#include <string>
#include <iostream>
#include <functional>
#include <boost/spirit/include/classic_core.hpp>

////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;

////////////////////////////////////////////////////////////////////////////
// push_bool

struct push_bool{
	push_bool(stack<bool>& eval_) : eval(eval_) {}

	void operator()(char const* str, char const* /*end*/) const{
		bool n = atoi(str) != 0 ? true : false;
		eval.push(n);
//		cout << "push\t" << bool(n) << endl;
	}

	stack<bool>& eval;
};

//////////////////////////////////////////////////////////////////////////
// operations: *, +, @

template<class _Ty> 
struct logical_xor : public binary_function<_Ty, _Ty, bool>{	
	bool operator()(const _Ty& _Left, const _Ty& _Right) const{
		return (_Left ^ _Right);
	}
};

template <typename op>
struct do_op{
	do_op(op const& the_op, stack<bool>& eval_) : m_op(the_op), eval(eval_) {}

	void operator()(char const*, char const*) const{
		bool rhs = eval.top();
		eval.pop();
		bool lhs = eval.top();
		eval.pop();

// 		cout << "popped " << lhs << " and " << rhs << " from the stack. ";
// 		cout << "pushing " << m_op(lhs, rhs) << " onto the stack.\n";
		eval.push(m_op(lhs, rhs));
	}

	op m_op;
	stack<bool>& eval;
};

template <class op>
do_op<op>
make_op(op const& the_op, stack<bool>& eval){
	return do_op<op>(the_op, eval);
}

//////////////////////////////////////////////////////////////////////////
// operation : ~

struct do_not{
	do_not(stack<bool>& eval_) : eval(eval_) {}

	void operator()(char const*, char const*) const{
		bool lhs = eval.top();
		eval.pop();

// 		cout << "popped " << lhs << " from the stack. ";
		lhs = !lhs;
//		cout << "pushing " << lhs << " onto the stack.\n";
		eval.push(lhs);
	}

	stack<bool>& eval;
};

////////////////////////////////////////////////////////////////////////////
// bool calculator

struct boolCalc : public grammar<boolCalc>{
	boolCalc(stack<bool>& eval_) : eval(eval_) {}

	template <typename ScannerT>
	struct definition{
		definition(boolCalc const& self){
			integer =
				lexeme_d[ (+digit_p)[push_bool(self.eval)] ]
			;

			factor =
				integer
				|   '(' >> expression >> ')'
				|   ('~' >> factor)[do_not(self.eval)]
				|	('!' >> factor)[do_not(self.eval)]
/*				|   ('+' >> factor)*/
			;

			term =
				factor
				>> *(
					 ('*' >> factor)[make_op(logical_and<bool>(), self.eval)]
				|    ('&' >> factor)[make_op(logical_and<bool>(), self.eval)]
				|    ('@' >> factor)[make_op(logical_xor<bool>(), self.eval)]
				|    ('^' >> factor)[make_op(logical_xor<bool>(), self.eval)]
					);

			expression =
				term
				>> *(
					 ('+' >> term)[make_op(logical_or<bool>(), self.eval)]
				|    ('|' >> term)[make_op(logical_or<bool>(), self.eval)]
					);
		}

		rule<ScannerT> expression, term, factor, integer;
		rule<ScannerT> const& start() const { return expression; }
	};

	stack<bool>& eval;
};

//////////////////////////////////////////////////////////////////////////
// calcExpVal

struct Exp2LUT{
	Exp2LUT(const std::string& expr, int bitAmount);

	void ExpToTable   ();
	void BinaryToTable();
	void HexToTable   ();

	std::vector<int> toVec() const { return _vecTable; }
	std::string 	 toStr() const { return _strTable; }

	int				 _bitAmount;
	std::string      _expr;
	std::string      _strTable;
	std::vector<int> _vecTable;
};

#endif