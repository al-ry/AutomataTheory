#include "Utils.h"


int main()
{

	Grammar gr;
	//std::string gram = R"(<S> -> <S> a | ( <S> ) | b)";
	std::string gram = R"(<S> -> id = <E>
<S> -> while <E> do <S>
<E> -> <E> + <E>
<E> -> id)";

//	std::string gram = R"(<S> -> <A> a 
//<S> -> b
//<A> -> c <A> <S>)";

//	std::string gram = R"(<S> -> <A> b for while
//<S> -> int <O> float for
//<O> -> <A>
//<A> -> for <A> 
//<A> -> e)";

//	std::string gram = R"(<S> -> <A> <B>
//<A> -> a
//<A> -> c <A>
//<B> -> b <A>)";


//	std::string gram = R"(<S> –> <S> + <A> 
//<S> -> <S> * <A> 
//<S> -> 5 
//<A> -> ( <A> ) 
//<A> -> i)";


//	std::string gram = R"(<S> -> <S> c <A> 
//<S> -> a
//<A> -> a <A> b
//<A> -> c)";


//	std::string gram = R"(<U> -> <A> <B> <C>
//<A> -> a <A> 
//<A> -> e
//<B> -> b <B>
//<B> -> e
//<C> -> c <C>
//<C> -> e)";

	CreateGrammar(gram);




	//CreateGrammar("<A> -> l\n<A> -> <A> a c r\n<A> -> <A> a q\n<B> -> <B> o\n<B> -> <B> c\n<B> -> <O> o\n<B> -> y");
	//CreateGrammar("<A> -> l\n<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> a b\n <A> -> <A> a c r\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
	//CreateGrammar("<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> b\n <A> -> <A> c\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
	//CreateGrammar("<S> -> <A> <B> <C> _|_ | <A> a\n<A> -> <A> a\n<A> -> e\n<B> -> <B> b\n<B> -> e\n<C> -> <C> c\n<C> -> e");
	return 0;
}