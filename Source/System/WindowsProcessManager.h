// <copyright file="WindowsProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"
#include "WindowsProcess.h"

namespace Opal::System
{
	/// <summary>
	/// A windows platform specific process executable using system
	/// </summary>
	export class WindowsProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsProcessManager'/> class.
		/// </summary>
		WindowsProcessManager()
		{
		}

		/// <summary>
		/// Gets the current process file name
		/// </summary>
		Path GetCurrentProcessFileName() override final
		{
			auto buffer = std::array<char, 1024>();

			// Pass in null handle to get current process information
			auto handle = nullptr;
			auto sizeRead = GetModuleFileNameA(
				handle,
				buffer.data(),
				static_cast<DWORD>(buffer.size()));
			if (sizeRead == 0)
			{
				auto error = GetLastError();
				// Convert raw error into a std exception
				throw std::runtime_error("GetCurrentProcessFileName GetModuleFileNameA Failed: " + std::to_string(error));
			}
			else if (sizeRead == buffer.size())
			{
				// Note: This may have perfectly fit, but there must be a null terminator
				throw std::runtime_error("Ran out of room in the buffer");
			}

			return Path(std::string(buffer.data(), buffer.size()));
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
			return std::make_shared<WindowsProcess>(
				executable,
				arguments,
				workingDirectory,
				interceptInputOutput);
		}
	};
}
