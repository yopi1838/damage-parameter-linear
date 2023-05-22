#include "pch.h"
#include "CppUnitTest.h"
#include "jmodelYopi.h"
using namespace jmodels;

#define private public
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unitTest1
{
	TEST_CLASS(unitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			JModelYopi j();
		}
	};
}
