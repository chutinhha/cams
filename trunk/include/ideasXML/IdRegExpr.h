#ifndef __REGEXPR_H_OGM
#define __REGEXPR_H_OGM

#include <string>

class xsyntax
{
	public:
		unsigned getErrorPos(){ return errorPos; }
		void setErrorPos(unsigned pos){ errorPos = pos; }
	protected:
		unsigned errorPos;
};

#define MAXSTATES	100

class CIdRegExpr
{
	public:
		CIdRegExpr();
		CIdRegExpr(const char *pattern);

		void compile(const char *pattern);
		int search(const char *str, unsigned start = 0);
		int searchLen(const char *str, unsigned &len, unsigned start = 0);

		const char *operator = (const char *pattern);

		void clear_after_error();

	private:
		enum {
			no_error = 0,
			compiler_error,
			simulation_error
		};

		short _error_type;

		void set_error_type(short et);

	protected:
		struct State
		{
			char the_char;
			unsigned next1, next2;
		} automaton[MAXSTATES];

		void clear_automaton();

		class Deque
		{
			public:
				Deque();
				~Deque();

				void push(int n);
				void put(int n);
				int pop();

				int isEmpty();

			protected:
				struct Element {
					int n;
					Element *next;
				} *head, *tail;
		} deque;

		// compiler
		unsigned j, state;
		const char *p;

		unsigned list();
		unsigned element();
		unsigned v();
		void error();

		int isLetter(char c);

		// automaton simulation
		int simulate(const char *str, int j);
};


inline const char *CIdRegExpr::operator = (const char *pattern)
{
	compile(pattern);
	return pattern;
}

inline int CIdRegExpr::search(const char *str, unsigned start)
{
	unsigned dummy;
	return searchLen(str, dummy, start);
}

// error codes of CIdRegExpr::search and CIdRegExpr::searchLen
#define REGEXPR_NOT_FOUND		-1
#define REGEXPR_NOT_COMPILED	-2

#endif //__REGEXPR_H_OGM
