#pragma once
#include "Utils/SemanticVersionTests.h"

TestState RunSemanticVersionTests() 
 {
	auto className = "SemanticVersionTests";
	auto testClass = std::make_shared<Soup::UnitTests::SemanticVersionTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "DefaultInitializer", [&testClass]() { testClass->DefaultInitializer(); });
	state += SoupTest::RunTest(className, "InitializeValues(1, 2, 3)", [&testClass]() { testClass->InitializeValues(1, 2, 3); });
	state += SoupTest::RunTest(className, "InitializeValues(3, 2, 1)", [&testClass]() { testClass->InitializeValues(3, 2, 1); });
	state += SoupTest::RunTest(className, "InitializeValues(1, 1, 1)", [&testClass]() { testClass->InitializeValues(1, 1, 1); });
	state += SoupTest::RunTest(className, "InitializeValues(100, 200, 300)", [&testClass]() { testClass->InitializeValues(100, 200, 300); });
	state += SoupTest::RunTest(className, "InitializeValues(1, 2, 333)", [&testClass]() { testClass->InitializeValues(1, 2, 333); });
	state += SoupTest::RunTest(className, "InitializeValues(1, 222, 3)", [&testClass]() { testClass->InitializeValues(1, 222, 3); });
	state += SoupTest::RunTest(className, "InitializeValues(111, 2, 3)", [&testClass]() { testClass->InitializeValues(111, 2, 3); });
	state += SoupTest::RunTest(className, "OperatorEqual", [&testClass]() { testClass->OperatorEqual(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualMajor", [&testClass]() { testClass->OperatorNotEqualMajor(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualMinor", [&testClass]() { testClass->OperatorNotEqualMinor(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualPatch", [&testClass]() { testClass->OperatorNotEqualPatch(); });
	state += SoupTest::RunTest(className, "ParseValues(\"1.2.3\", 1, 2, 3)", [&testClass]() { testClass->ParseValues("1.2.3", 1, 2, 3); });
	state += SoupTest::RunTest(className, "ParseValues(\"3.2.1\", 3, 2, 1)", [&testClass]() { testClass->ParseValues("3.2.1", 3, 2, 1); });
	state += SoupTest::RunTest(className, "ParseValues(\"1.1.1\", 1, 1, 1)", [&testClass]() { testClass->ParseValues("1.1.1", 1, 1, 1); });
	state += SoupTest::RunTest(className, "ParseValues(\"100.200.300\", 100, 200, 300)", [&testClass]() { testClass->ParseValues("100.200.300", 100, 200, 300); });
	state += SoupTest::RunTest(className, "ParseValues(\"1.2.333\", 1, 2, 333)", [&testClass]() { testClass->ParseValues("1.2.333", 1, 2, 333); });
	state += SoupTest::RunTest(className, "ParseValues(\"1.222.3\", 1, 222, 3)", [&testClass]() { testClass->ParseValues("1.222.3", 1, 222, 3); });
	state += SoupTest::RunTest(className, "ParseValues(\"111.2.3\", 111, 2, 3)", [&testClass]() { testClass->ParseValues("111.2.3", 111, 2, 3); });
	state += SoupTest::RunTest(className, "ParseValues(\"0.0.0\", 0, 0, 0)", [&testClass]() { testClass->ParseValues("0.0.0", 0, 0, 0); });
	state += SoupTest::RunTest(className, "TryParseValues(\"1.2.3\", true, 1, 2, 3)", [&testClass]() { testClass->TryParseValues("1.2.3", true, 1, 2, 3); });
	state += SoupTest::RunTest(className, "TryParseValues(\"0.0.0\", true, 0, 0, 0)", [&testClass]() { testClass->TryParseValues("0.0.0", true, 0, 0, 0); });
	state += SoupTest::RunTest(className, "TryParseValues(\"\", false, 0, 0, 0)", [&testClass]() { testClass->TryParseValues("", false, 0, 0, 0); });
	state += SoupTest::RunTest(className, "TryParseValues(\"1\", false, 0, 0, 0)", [&testClass]() { testClass->TryParseValues("1", false, 0, 0, 0); });
	state += SoupTest::RunTest(className, "TryParseValues(\"1.2\", false, 0, 0, 0)", [&testClass]() { testClass->TryParseValues("1.2", false, 0, 0, 0); });
	state += SoupTest::RunTest(className, "ToStringValues(1, 2, 3, \"1.2.3\")", [&testClass]() { testClass->ToStringValues(1, 2, 3, "1.2.3"); });
	state += SoupTest::RunTest(className, "ToStringValues(0, 0, 0, \"0.0.0\")", [&testClass]() { testClass->ToStringValues(0, 0, 0, "0.0.0"); });

	return state;
}