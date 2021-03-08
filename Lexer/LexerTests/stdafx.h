#pragma once
#include <stdio.h>
#include <tchar.h>

#define BOOST_TEST_INCLUDED
#pragma warning(push, 3)
#pragma warning(disable : 4456 4702)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <sstream>
#include <string>
// TODO: reference additional headers your program requires here