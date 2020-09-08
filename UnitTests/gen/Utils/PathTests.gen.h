#pragma once
#include "Utils/PathTests.h"

TestState RunPathTests() 
 {
	auto className = "PathTests";
	auto testClass = std::make_shared<Soup::UnitTests::PathTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "DefaultInitializer", [&testClass]() { testClass->DefaultInitializer(); });
	state += SoupTest::RunTest(className, "Empty", [&testClass]() { testClass->Empty(); });
	state += SoupTest::RunTest(className, "RelativePath_Simple", [&testClass]() { testClass->RelativePath_Simple(); });
	state += SoupTest::RunTest(className, "RelativePath_Parent", [&testClass]() { testClass->RelativePath_Parent(); });
	state += SoupTest::RunTest(className, "RelativePath_Complex", [&testClass]() { testClass->RelativePath_Complex(); });
	state += SoupTest::RunTest(className, "SimpleAbsolutePath", [&testClass]() { testClass->SimpleAbsolutePath(); });
	state += SoupTest::RunTest(className, "AlternativeDirectoriesPath", [&testClass]() { testClass->AlternativeDirectoriesPath(); });
	state += SoupTest::RunTest(className, "RemoveEmptyDirectoryInside", [&testClass]() { testClass->RemoveEmptyDirectoryInside(); });
	state += SoupTest::RunTest(className, "RemoveParentDirectoryInside", [&testClass]() { testClass->RemoveParentDirectoryInside(); });
	state += SoupTest::RunTest(className, "RemoveTwoParentDirectoryInside", [&testClass]() { testClass->RemoveTwoParentDirectoryInside(); });
	state += SoupTest::RunTest(className, "LeaveParentDirectoryAtStart", [&testClass]() { testClass->LeaveParentDirectoryAtStart(); });
	state += SoupTest::RunTest(className, "CurrentDirectoryAtStart", [&testClass]() { testClass->CurrentDirectoryAtStart(); });
	state += SoupTest::RunTest(className, "CurrentDirectoryAtStartAlternate", [&testClass]() { testClass->CurrentDirectoryAtStartAlternate(); });
	state += SoupTest::RunTest(className, "Concatenate_Simple", [&testClass]() { testClass->Concatenate_Simple(); });
	state += SoupTest::RunTest(className, "Concatenate_Empty", [&testClass]() { testClass->Concatenate_Empty(); });
	state += SoupTest::RunTest(className, "Concatenate_RootFile", [&testClass]() { testClass->Concatenate_RootFile(); });
	state += SoupTest::RunTest(className, "Concatenate_RootFolder", [&testClass]() { testClass->Concatenate_RootFolder(); });
	state += SoupTest::RunTest(className, "Concatenate_UpDirectory", [&testClass]() { testClass->Concatenate_UpDirectory(); });
	state += SoupTest::RunTest(className, "Concatenate_UpDirectoryBeginning", [&testClass]() { testClass->Concatenate_UpDirectoryBeginning(); });
	state += SoupTest::RunTest(className, "SetFileExtension_Replace", [&testClass]() { testClass->SetFileExtension_Replace(); });
	state += SoupTest::RunTest(className, "SetFileExtension_Replace_Rooted", [&testClass]() { testClass->SetFileExtension_Replace_Rooted(); });
	state += SoupTest::RunTest(className, "SetFileExtension_Add", [&testClass]() { testClass->SetFileExtension_Add(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_Empty", [&testClass]() { testClass->GetRelativeTo_Empty(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_SingleRelative", [&testClass]() { testClass->GetRelativeTo_SingleRelative(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_UpParentRelative", [&testClass]() { testClass->GetRelativeTo_UpParentRelative(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_MismatchRelative", [&testClass]() { testClass->GetRelativeTo_MismatchRelative(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_Rooted_DifferentRoot", [&testClass]() { testClass->GetRelativeTo_Rooted_DifferentRoot(); });
	state += SoupTest::RunTest(className, "GetRelativeTo_Rooted_SingleFolder", [&testClass]() { testClass->GetRelativeTo_Rooted_SingleFolder(); });

	return state;
}