#pragma once
#include "Utils/PathTests.h"

TestState RunPathTests() 
 {
	auto className = "PathTests";
	auto testClass = std::make_shared<Soup::UnitTests::PathTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	// state += Soup::Test::RunTest(className, "Initialize_Empty", [&testClass]() { testClass->Initialize_Empty(); });
	state += Soup::Test::RunTest(className, "Initialize_RelativePath_Simple", [&testClass]() { testClass->Initialize_RelativePath_Simple(); });
	state += Soup::Test::RunTest(className, "Initialize_RelativePath_Parent", [&testClass]() { testClass->Initialize_RelativePath_Parent(); });
	state += Soup::Test::RunTest(className, "Initialize_RelativePath_Complex", [&testClass]() { testClass->Initialize_RelativePath_Complex(); });
	state += Soup::Test::RunTest(className, "Initialize_WindowsRoot", [&testClass]() { testClass->Initialize_WindowsRoot(); });
	state += Soup::Test::RunTest(className, "Initialize_LinuxRoot", [&testClass]() { testClass->Initialize_LinuxRoot(); });
	state += Soup::Test::RunTest(className, "Initialize_SimpleAbsolutePath", [&testClass]() { testClass->Initialize_SimpleAbsolutePath(); });
	state += Soup::Test::RunTest(className, "Parse_AlternativeDirectoriesPath", [&testClass]() { testClass->Parse_AlternativeDirectoriesPath(); });
	state += Soup::Test::RunTest(className, "Parse_RemoveEmptyDirectoryInside", [&testClass]() { testClass->Parse_RemoveEmptyDirectoryInside(); });
	state += Soup::Test::RunTest(className, "Parse_RemoveParentDirectoryInside", [&testClass]() { testClass->Parse_RemoveParentDirectoryInside(); });
	state += Soup::Test::RunTest(className, "Parse_RemoveTwoParentDirectoryInside", [&testClass]() { testClass->Parse_RemoveTwoParentDirectoryInside(); });
	state += Soup::Test::RunTest(className, "Parse_LeaveParentDirectoryAtStart", [&testClass]() { testClass->Parse_LeaveParentDirectoryAtStart(); });
	state += Soup::Test::RunTest(className, "Parse_CurrentDirectoryAtStart", [&testClass]() { testClass->Parse_CurrentDirectoryAtStart(); });
	state += Soup::Test::RunTest(className, "Parse_CurrentDirectoryAtStartAlternate", [&testClass]() { testClass->Parse_CurrentDirectoryAtStartAlternate(); });
	state += Soup::Test::RunTest(className, "Concatenate_Simple", [&testClass]() { testClass->Concatenate_Simple(); });
	state += Soup::Test::RunTest(className, "Concatenate_Empty", [&testClass]() { testClass->Concatenate_Empty(); });
	state += Soup::Test::RunTest(className, "Concatenate_RootFile", [&testClass]() { testClass->Concatenate_RootFile(); });
	state += Soup::Test::RunTest(className, "Concatenate_RootFolder", [&testClass]() { testClass->Concatenate_RootFolder(); });
	state += Soup::Test::RunTest(className, "Concatenate_UpDirectory", [&testClass]() { testClass->Concatenate_UpDirectory(); });
	state += Soup::Test::RunTest(className, "Concatenate_TwoRelative", [&testClass]() { testClass->Concatenate_TwoRelative(); });
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