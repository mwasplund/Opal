// <copyright file="PathTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class PathTests
	{
	public:
		// [[Fact]]
		void Initialize_Default()
		{
			auto uut = Path();
			Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
			Assert::IsFalse(uut.HasFileName(), "Verify has no filename.");
			Assert::IsFalse(uut.HasFileStem(), "Verify has no file stem.");
			Assert::IsFalse(uut.HasFileExtension(), "Verify has no file extension.");
			Assert::AreEqual("./", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual(".\\", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_Empty()
		{
			auto exception = Assert::Throws<std::runtime_error>([&]()
			{
				auto uut = Path("");
			});
			Assert::AreEqual("A path must have a directory separator", exception.what(), "Verify exception value matches.");
		}

		// [[Fact]]
		void Initialize_RelativePath_Simple()
		{
			auto uut = Path("./");
			Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
			Assert::IsFalse(uut.HasFileName(), "Verify has filename.");
			Assert::IsFalse(uut.HasFileStem(), "Verify has file stem.");
			Assert::IsFalse(uut.HasFileExtension(), "Verify has file extension.");
			Assert::AreEqual("./", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual(".\\", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_RelativePath_Parent()
		{
			auto uut = Path("../");
			Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
			Assert::IsFalse(uut.HasFileName(), "Verify has filename.");
			Assert::IsFalse(uut.HasFileStem(), "Verify has file stem.");
			Assert::IsFalse(uut.HasFileExtension(), "Verify has file extension.");
			Assert::AreEqual("../", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual("..\\", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_RelativePath_Complex()
		{
			auto uut = Path("./myfolder/anotherfolder/file.txt");
			Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
			Assert::IsTrue(uut.HasFileName(), "Verify has filename.");
			Assert::AreEqual("file.txt", uut.GetFileName(), "Verify file name matches.");
			Assert::IsTrue(uut.HasFileStem(), "Verify has file stem.");
			Assert::AreEqual("file", uut.GetFileStem(), "Verify file stem matches.");
			Assert::IsTrue(uut.HasFileExtension(), "Verify has file extension.");
			Assert::AreEqual(".txt", uut.GetFileExtension(), "Verify file extension matches.");
			Assert::AreEqual("./myfolder/anotherfolder/file.txt", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual(".\\myfolder\\anotherfolder\\file.txt", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_WindowsRoot()
		{
			auto uut = Path("C:/");
			Assert::IsTrue(uut.HasRoot(), "Verify has root.");
			Assert::AreEqual("C:", uut.GetRoot(), "Verify root matches.");
			Assert::IsFalse(uut.HasFileName(), "Verify has no filename.");
			Assert::IsFalse(uut.HasFileStem(), "Verify has no file stem.");
			Assert::IsFalse(uut.HasFileExtension(), "Verify has no file extension.");
			Assert::AreEqual("C:/", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual("C:\\", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_LinuxRoot()
		{
			auto uut = Path("/");
			Assert::IsTrue(uut.HasRoot(), "Verify has root.");
			Assert::AreEqual("", uut.GetRoot(), "Verify root matches.");
			Assert::IsFalse(uut.HasFileName(), "Verify has no filename.");
			Assert::IsFalse(uut.HasFileStem(), "Verify has no file stem.");
			Assert::IsFalse(uut.HasFileExtension(), "Verify has no file extension.");
			Assert::AreEqual("/", uut.ToString(), "Verify string value matches.");
			Assert::AreEqual("\\", uut.ToAlternateString(), "Verify alternate string value matches.");
		}

		// [[Fact]]
		void Initialize_SimpleAbsolutePath()
		{
			auto uut = Path("C:/myfolder/anotherfolder/file.txt");
			Assert::IsTrue(uut.HasRoot(), "Verify is root.");
			Assert::AreEqual("C:", uut.GetRoot(), "Verify root matches.");
			Assert::IsTrue(uut.HasFileName(), "Verify has filename.");
			Assert::AreEqual("file.txt", uut.GetFileName(), "Verify file name matches.");
			Assert::IsTrue(uut.HasFileStem(), "Verify has file stem.");
			Assert::AreEqual("file", uut.GetFileStem(), "Verify file stem matches.");
			Assert::IsTrue(uut.HasFileExtension(), "Verify has file extension.");
			Assert::AreEqual(".txt", uut.GetFileExtension(), "Verify file extension matches.");
			Assert::AreEqual("C:/myfolder/anotherfolder/file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_AlternativeDirectoriesPath()
		{
			auto uut = Path::Parse("C:\\myfolder/anotherfolder\\file.txt");
			Assert::IsTrue(uut.HasRoot(), "Verify is root.");
			Assert::AreEqual("C:", uut.GetRoot(), "Verify root matches.");
			Assert::IsTrue(uut.HasFileName(), "Verify has filename.");
			Assert::AreEqual("file.txt", uut.GetFileName(), "Verify file name matches.");
			Assert::IsTrue(uut.HasFileStem(), "Verify has file stem.");
			Assert::AreEqual("file", uut.GetFileStem(), "Verify file stem matches.");
			Assert::IsTrue(uut.HasFileExtension(), "Verify has file extension.");
			Assert::AreEqual(".txt", uut.GetFileExtension(), "Verify file extension matches.");
			Assert::AreEqual("C:/myfolder/anotherfolder/file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_RemoveEmptyDirectoryInside()
		{
			auto uut = Path::Parse("C:/myfolder//file.txt");
			Assert::AreEqual("C:/myfolder/file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_RemoveParentDirectoryInside()
		{
			auto uut = Path::Parse("C:/myfolder/../file.txt");
			Assert::AreEqual("C:/file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_RemoveTwoParentDirectoryInside()
		{
			auto uut = Path::Parse("C:/myfolder/myfolder2/../../file.txt");
			Assert::AreEqual("C:/file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_LeaveParentDirectoryAtStart()
		{
			auto uut = Path::Parse("../file.txt");
			Assert::AreEqual("../file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_CurrentDirectoryAtStart()
		{
			auto uut = Path::Parse("./file.txt");
			Assert::AreEqual("./file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_CurrentDirectoryAtStartAlternate()
		{
			auto uut = Path::Parse(".\\../file.txt");
			Assert::AreEqual("../file.txt", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_RelativeOnly()
		{
			auto uut = Path::Parse(".");
			Assert::AreEqual("./", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Parse_UpOnly()
		{
			auto uut = Path::Parse("..");
			Assert::AreEqual("../", uut.ToString(), "Verify string value matches.");
		}

		// [[Fact]]
		void Concatenate_Simple()
		{
			auto path1 = Path("C:/MyRootFolder/");
			auto path2 = Path("./MyFolder/MyFile.txt");
			auto uut = path1 + path2;

			Assert::AreEqual("C:/MyRootFolder/MyFolder/MyFile.txt", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void Concatenate_Empty()
		{
			auto path1 = Path("C:/MyRootFolder/");
			auto path2 = Path();
			auto uut = path1 + path2;

			// Changes the assumed file into a folder
			Assert::AreEqual("C:/MyRootFolder/", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void Concatenate_RootFile()
		{
			auto path1 = Path("C:/");
			auto path2 = Path("./MyFile.txt");
			auto uut = path1 + path2;

			Assert::AreEqual("C:/MyFile.txt", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void Concatenate_RootFolder()
		{
			auto path1 = Path("C:/");
			auto path2 = Path("./MyFolder/");
			auto uut = path1 + path2;

			Assert::AreEqual("C:/MyFolder/", uut.ToString(), "Verify value matches.");
		}
	
		// [[Fact]]
		void Concatenate_UpDirectory()
		{
			auto path1 = Path("C:/MyRootFolder/");
			auto path2 = Path("../NewRoot/MyFile.txt");
			auto uut = path1 + path2;

			Assert::AreEqual("C:/NewRoot/MyFile.txt", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void Concatenate_TwoRelative()
		{
			auto path1 = Path("./Folder1/");
			auto path2 = Path("./Other/MyFile.txt");
			auto uut = path1 + path2;

			Assert::AreEqual("./Folder1/Other/MyFile.txt", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void Concatenate_UpDirectoryBeginning()
		{
			auto path1 = Path("../MyRootFolder/");
			auto path2 = Path("../NewRoot/MyFile.txt");
			auto uut = path1 + path2;

			Assert::AreEqual("../NewRoot/MyFile.txt", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void SetFileExtension_Replace()
		{
			auto uut = Path("../MyFile.txt");
			uut.SetFileExtension("awe");

			Assert::AreEqual("../MyFile.awe", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void SetFileExtension_Replace_Rooted()
		{
			auto uut = Path("C:/MyFolder/MyFile.txt");
			uut.SetFileExtension("awe");

			Assert::AreEqual("C:/MyFolder/MyFile.awe", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void SetFileExtension_Add()
		{
			auto uut = Path("../MyFile");
			uut.SetFileExtension("awe");

			Assert::AreEqual("../MyFile.awe", uut.ToString(), "Verify value matches.");
		}

		// [[Fact]]
		void GetRelativeTo_Empty()
		{
			auto uut = Path("./File.txt");
			auto base = Path();

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("./File.txt", result.ToString(), "Verify result matches.");
		}

		// [[Fact]]
		void GetRelativeTo_SingleRelative()
		{
			auto uut = Path("./Folder/File.txt");
			auto base = Path("./Folder/");

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("./File.txt", result.ToString(), "Verify result matches.");
		}

		// [[Fact]]
		void GetRelativeTo_UpParentRelative()
		{
			auto uut = Path("../Folder/Target");
			auto base = Path("../Folder/");

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("./Target", result.ToString(), "Verify result matches.");
		}

		// [[Fact]]
		void GetRelativeTo_MismatchRelative()
		{
			auto uut = Path("./Folder1/File.txt");
			auto base = Path("./Folder2/");

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("../Folder1/File.txt", result.ToString(), "Verify result matches.");
		}

		// [[Fact]]
		void GetRelativeTo_Rooted_DifferentRoot()
		{
			auto uut = Path("C:/Folder1/File.txt");
			auto base = Path("D:/Folder2/");

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("C:/Folder1/File.txt", result.ToString(), "Verify result matches.");
		}

		// [[Fact]]
		void GetRelativeTo_Rooted_SingleFolder()
		{
			auto uut = Path("C:/Folder1/File.txt");
			auto base = Path("C:/Folder1/");

			auto result = uut.GetRelativeTo(base);

			Assert::AreEqual("./File.txt", result.ToString(), "Verify result matches.");
		}
	};
}
