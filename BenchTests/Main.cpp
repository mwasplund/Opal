#include "nanobench.h"
#include <filesystem>

import Opal;

using namespace Opal;

int main()
{
	{
		auto c = Path("C:/Path1/Path2/");
		auto d = Path("./Path3/Path4/");

		ankerl::nanobench::Bench().minEpochIterations(10000).run("Concatenate Paths", [&]
		{
			auto e = c + d;
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto c = Path("C:/Path1/Path2/");
		auto d = Path("../Path3/Path4/");
		
		ankerl::nanobench::Bench().minEpochIterations(10000).run("Concatenate Paths With Up Reference", [&]
		{
			auto e = c + d;
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		ankerl::nanobench::Bench().minEpochIterations(100000).run("Initialize Path With Root", [&]
		{
			auto e = Path("C:/Path1/Path2/");
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		ankerl::nanobench::Bench().minEpochIterations(100000).run("Create Windows Path With Root", [&]
		{
			auto e = Path::CreateWindows("C:\\Path1\\Path2\\");
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		ankerl::nanobench::Bench().minEpochIterations(100000).run("Initialize Path Relative", [&]
		{
			auto e = Path("./Path1/Path2/");
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto uut = System::STLFileSystem();
		ankerl::nanobench::Bench().minEpochIterations(10000).run("Get User Profile Directory", [&]
		{
			auto e = uut.GetUserProfileDirectory();
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto uut = System::STLFileSystem();
		ankerl::nanobench::Bench().minEpochIterations(10000).run("Get Current Directory", [&]
		{
			auto e = uut.GetCurrentDirectory();
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto uut = SemanticVersion(1);
		ankerl::nanobench::Bench().minEpochIterations(100000).run("SemanticVersion ToString Major Only", [&]
		{
			auto e = uut.ToString();
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto uut = SemanticVersion(1, 2);
		ankerl::nanobench::Bench().minEpochIterations(100000).run("SemanticVersion ToString With Minor", [&]
		{
			auto e = uut.ToString();
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}

	{
		auto uut = SemanticVersion(1, 2, 3);
		ankerl::nanobench::Bench().minEpochIterations(100000).run("SemanticVersion ToString With Minor and Path", [&]
		{
			auto e = uut.ToString();
			ankerl::nanobench::doNotOptimizeAway(e);
		});
	}
}