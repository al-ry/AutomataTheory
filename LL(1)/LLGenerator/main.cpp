#include "Utils.h"
#include "Generator.h"
#include <fstream>
#include <iostream>
#include <string>


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		return 1;
	}

	std::ifstream input("in\\" + std::string(argv[1]));
	std::ofstream outputGuideSet( "out\\GuideSet" + std::string(argv[2]));
	std::ofstream outputTable("out\\Table" + std::string(argv[2]));


	auto grammar = CreateGrammar(input);
	PrintGrammar(grammar, outputGuideSet);

	auto table = GenerateTable(grammar);
	PrintTable(table, outputTable);

	return 0;
}



//int main()
//{
//
//	Grammar gr;
//	//std::string gram = R"(<S> -> <S> a | ( <S> ) | b)";
//
//
////	std::string gram = R"(<S> -> id = <E>
////<S> -> while <E> do <S>
////<E> -> <E> + <E>
////<E> -> id)";
//

////	std::string gram = R"(<A> -> + <E> <A>
////<A> -> e
////<A> -> k
////<A> -> b
////<A> -> f
////<E> -> id <A>)";
//
////	std::string gram = R"(<S> -> type <F>
////<F> -> <I> = <T>
////<F> -> <I> = <T> ; <F>
////<T> -> int
////<T> -> record <G> end
////<G> -> <I> : <T>
////<G> -> <I> : <T> ; <G>
////<I> -> a
////<I> -> b
////<I> -> c)";
//
////	std::string gram = R"(<P> -> <S>
////<S> -> <S>  a
////<S> -> ( <S> )
////<S> -> b)";
//
//
//
//
////	std::string gram = R"(<S> -> <A> a 
////<S> -> b
////<A> -> c <A> <S>)";
//
////	std::string gram = R"(<S> -> <A> b for while
////<S> -> int <O> float for
////<O> -> <A>
////<A> -> for <A> 
////<A> -> e)";
//
////	std::string gram = R"(<S> -> <A> <B>
////<A> -> a
////<A> -> c <A>
////<B> -> b <A>)";
//
//
////	std::string gram = R"(<S> �> <S> + <A> 
////<S> -> <S> * <A> 
////<S> -> 5 
////<A> -> ( <A> ) 
////<A> -> i)";
//
//
////	std::string gram = R"(<S> -> <S> c <A> 
////<S> -> a
////<A> -> a <A> b
////<A> -> c)";
//
////
////	std::string gram = R"(<U> -> <A> <B> <C>
////<A> -> a <A> 
////<A> -> e
////<B> -> b <B>
////<B> -> e
////<C> -> c <C>
////<C> -> e)";
//
//	auto grammar = CreateGrammar(gram);
//
//	auto table = GenerateTable(grammar);
//
//	std::string name = "output.txt";
//
//	std::ofstream file(name);
//
//	PrintTable(table, file);
//
//
//
//
//	//CreateGrammar("<A> -> l\n<A> -> <A> a c r\n<A> -> <A> a q\n<B> -> <B> o\n<B> -> <B> c\n<B> -> <O> o\n<B> -> y");
//	//CreateGrammar("<A> -> l\n<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> a b\n <A> -> <A> a c r\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
//	//CreateGrammar("<A> -> <A> a\n<A> -> <B> z\n <A> -> <A> b\n <A> -> <A> c\n<A> -> <A> a q\n<A> -> <B> b\n <A> -> <B> f\n<A> -> <Z> a\n<A> -> <Z> t\n<A> -> <T> t");
//	//CreateGrammar("<S> -> <A> <B> <C> _|_ | <A> a\n<A> -> <A> a\n<A> -> e\n<B> -> <B> b\n<B> -> e\n<C> -> <C> c\n<C> -> e");
//	return 0;
//}