#include "Utils.h"


int main()
{

	Grammar gr; 

	CreateGrammar("<S> -> <A> <B> <C> _|_ | <A> a\n<A> -> <A> a\n<A> -> e\n<B> -> <B> b\n<B> -> e\n<C> -> <C> c\n<C> -> e");
	return 0;
}