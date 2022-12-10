// <copyright file="LinuxProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Opal::System
{
	/// <summary>
	/// A linux platform specific process executable using system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class LinuxProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxProcessManager'/> class.
		/// </summary>
		LinuxProcessManager()
		{
		}

		/// <summary>
		/// Gets the current process file name
		/// </summary>
		Path GetCurrentProcessFileName() override final
		{
			throw std::runtime_error("Not implemented");
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		std::shared_ptr<IProcess> CreateProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			bool interceptInputOutput) override final
		{
			throw std::runtime_error("Not implemented");
		}
	};
}
