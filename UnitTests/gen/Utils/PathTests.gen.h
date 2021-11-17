#pragma once
#include "Utils/PathTests.h"

TestState RunPathTests() 
 {
	auto className = "PathTests";
	auto testClass = std::make_shared<Soup::UnitTests::PathTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Load_Empty", [&testClass]() { testClass->Load_Empty(); });
	state += Soup::Test::RunTest(className, "Load_Root", [&testClass]() { testClass->Load_Root(); });
	state += Soup::Test::RunTest(className, "Load_AbsoluteFile", [&testClass]() { testClass->Load_AbsoluteFile(); });
	state += Soup::Test::RunTest(className, "DefaultInitializer", [&testClass]() { testClass->DefaultInitializer(); });
	state += Soup::Test::RunTest(className, "Empty", [&testClass]() { testClass->Empty(); });
	state += Soup::Test::RunTest(className, "RelativePath_Simple", [&testClass]() { testClass->RelativePath_Simple(); });
	state += Soup::Test::RunTest(className, "RelativePath_Parent", [&testClass]() { testClass->RelativePath_Parent(); });
	state += Soup::Test::RunTest(className, "RelativePath_Complex", [&testClass]() { testClass->RelativePath_Complex(); });
	state += Soup::Test::RunTest(className, "SimpleAbsolutePath", [&testClass]() { testClass->SimpleAbsolutePath(); });
	state += Soup::Test::RunTest(className, "AlternativeDirectoriesPath", [&testClass]() { testClass->AlternativeDirectoriesPath(); });
	state += Soup::Test::RunTest(className, "RemoveEmptyDirectoryInside", [&testClass]() { testClass->RemoveEmptyDirectoryInside(); });
	state += Soup::Test::RunTest(className, "RemoveParentDirectoryInside", [&testClass]() { testClass->RemoveParentDirectoryInside(); });
	state += Soup::Test::RunTest(className, "RemoveTwoParentDirectoryInside", [&testClass]() { testClass->RemoveTwoParentDirectoryInside(); });
	state += Soup::Test::RunTest(className, "LeaveParentDirectoryAtStart", [&testClass]() { testClass->LeaveParentDirectoryAtStart(); });
	state += Soup::Test::RunTest(className, "CurrentDirectoryAtStart", [&testClass]() { testClass->CurrentDirectoryAtStart(); });
	state += Soup::Test::RunTest(className, "CurrentDirectoryAtStartAlternate", [&testClass]() { testClass->CurrentDirectoryAtStartAlternate(); });
	state += Soup::Test::RunTest(className, "Concatenate_Simple", [&testClass]() { testClass->Concatenate_Simple(); });
	state += Soup::Test::RunTest(className, "Concatenate_Empty", [&testClass]() { testClass->Concatenate_Empty(); });
	state += Soup::Test::RunTest(className, "Concatenate_RootFile", [&testClass]() { testClass->Concatenate_RootFile(); });
	state += Soup::Test::RunTest(className, "Concatenate_RootFolder", [&testClass]() { testClass->Concatenate_RootFolder(); });
	state += Soup::Test::RunTest(className, "Concatenate_UpDirectory", [&testClass]() { testClass->Concatenate_UpDirectory(); });
	state += Soup::Test::RunTest(className, "Concatenate_UpDirectoryBeginning", [&testClass]() { testClass->Concatenate_UpDirectoryBeginning(); });
	state += Soup::Test::RunTest(className, "SetFileExtension_Replace", [&testClass]() { testClass->SetFileExtension_Replace(); });
	state += Soup::Test::RunTest(className, "SetFileExtension_Replace_Rooted", [&testClass]() { testClass->SetFileExtension_Replace_Rooted(); });
	state += Soup::Test::RunTest(className, "SetFileExtension_Add", [&testClass]() { testClass->SetFileExtension_Add(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_Empty", [&testClass]() { testClass->GetRelativeTo_Empty(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_SingleRelative", [&testClass]() { testClass->GetRelativeTo_SingleRelative(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_UpParentRelative", [&testClass]() { testClass->GetRelativeTo_UpParentRelative(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_MismatchRelative", [&testClass]() { testClass->GetRelativeTo_MismatchRelative(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_Rooted_DifferentRoot", [&testClass]() { testClass->GetRelativeTo_Rooted_DifferentRoot(); });
	state += Soup::Test::RunTest(className, "GetRelativeTo_Rooted_SingleFolder", [&testClass]() { testClass->GetRelativeTo_Rooted_SingleFolder(); });

	return state;
}