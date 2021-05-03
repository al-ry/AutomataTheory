#include "Utils.h"


int main()
{

	Grammar gr;
	std::string gram = R"(<S> -> <S> a | ( <S> ) | b)";
	CreateGrammar(gram);
	//CreateGrammar("<A> -> l\n<A> -> <A> a c r\n<A> -> <A> a q\n<B> -> <B> o\n<B> -> <B> c\n<B> -> <O> o\n<B> -> y");
	//CreateGrammar("<A> -> l\n<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> a b\n <A> -> <A> a c r\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
	//CreateGrammar("<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> b\n <A> -> <A> c\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
	//CreateGrammar("<S> -> <A> <B> <C> _|_ | <A> a\n<A> -> <A> a\n<A> -> e\n<B> -> <B> b\n<B> -> e\n<C> -> <C> c\n<C> -> e");
	return 0;
}