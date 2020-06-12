// <copyright file="PlatformProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Opal::System
{
	/// <summary>
	/// A platform specific process executable using system
	/// </summary>
	export class PlatformProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='PlatformProcessManager'/> class.
		/// </summary>
		PlatformProcessManager()
		{
		}

		/// <summary>
		/// Gets the process file name
		/// </summary>
		Path GetProcessFileName() override final
		{
			auto buffer = std::array<char, 1024>();

			// Pass in null handle to get current process information
			auto handle = nullptr;
			auto sizeRead = GetModuleFileNameA(
				handle,
				buffer.data(),
				buffer.size());
			if (sizeRead == 0)
			{
				auto error = GetLastError();
				// Convert raw error into a std exception
				throw std::runtime_error("GetProcessFileName GetModuleFileNameA Failed: " + std::to_string(error));
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
		ProcessResult Execute(
			const Path& application,
			const std::string& arguments,
			const Path& workingDirectory) override final
		{
			std::stringstream argumentsValue;
			argumentsValue << "\"" << application.ToAlternateString() << "\"" << " " << arguments;

			std::string argumentsString = argumentsValue.str();

			// Aggregate all the output
			std::stringstream stdOut;
			std::stringstream stdErr;

			// Convert to c style since we do not have access to stdlib in this helper
			// TODO: Remove crazyness when we get a good implementation of modules
			auto outputCallback = [&stdOut](const char* value, int length) 
			{ 
				stdOut << value;
			};
			auto errorCallback = [&stdErr](const char* value, int length)
			{
				stdErr << value;
			};

			// Setup the input/output streams

			// TODO: We need to read from the buffer to ensure it doesn't deadlock on the wait forever
			int pipeBufferSize = 5 * 1024 * 1024;

			// Set the bInheritHandle flag so pipe handles are inherited.
			SECURITY_ATTRIBUTES securityAttributes; 
			securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
			securityAttributes.bInheritHandle = true; 
			securityAttributes.lpSecurityDescriptor = nullptr; 

			// Create a pipe for the child process's STDOUT.
			SmartHandle childStdOutRead;
			SmartHandle childStdOutWrite;
			if (!CreatePipe(&childStdOutRead._handle, &childStdOutWrite._handle, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!SetHandleInformation(childStdOutRead._handle, HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDERR.
			SmartHandle childStdErrRead;
			SmartHandle childStdErrWrite;
			if (!CreatePipe(&childStdErrRead._handle, &childStdErrWrite._handle, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");

			// Ensure the read handle to the pipe for STDERR is not inherited.
			if (!SetHandleInformation(childStdErrRead._handle, HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDIN.
			SmartHandle childStdInRead;
			SmartHandle childStdInWrite;
			if (!CreatePipe(&childStdInRead._handle, &childStdInWrite._handle, &securityAttributes, 0))
				throw std::runtime_error("Execute CreatePipe Failed");

			// Ensure the write handle to the pipe for STDIN is not inherited.
			if (!SetHandleInformation(childStdInWrite._handle, HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Setup the process creation parameters
			LPSECURITY_ATTRIBUTES processAttributes = nullptr;
			LPSECURITY_ATTRIBUTES threadAttributes = nullptr;
			bool inheritHandles = true;
			DWORD creationFlags = 0;
			void* environment = nullptr;

			STARTUPINFOA startupInfo = {};
			ZeroMemory(&startupInfo, sizeof(STARTUPINFOA));
			startupInfo.cb = sizeof(startupInfo);
			startupInfo.hStdError = childStdErrWrite._handle;
			startupInfo.hStdOutput = childStdOutWrite._handle;
			startupInfo.hStdInput = childStdInRead._handle;
			startupInfo.dwFlags |= STARTF_USESTDHANDLES;

			PROCESS_INFORMATION processInfo = {};
			ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

			// Start the process
			if (!CreateProcessA(
				application.ToString().c_str(),
				const_cast<char*>(argumentsString.c_str()),
				processAttributes,
				threadAttributes,
				inheritHandles,
				creationFlags,
				environment,
				workingDirectory.ToString().c_str(),
				&startupInfo,
				&processInfo))
			{
				auto error = GetLastError();
				throw std::runtime_error("Execute CreateProcessA Failed: " + std::to_string(error));
			}

			// Throw the process and thread handles into smart pointers to ensure they are cleaned up
			auto processHandle = SmartHandle(processInfo.hProcess);
			auto threadHandle = SmartHandle(processInfo.hThread);

			// Wait until child process exits.
			WaitForSingleObject(processHandle._handle, INFINITE);

			// Get the exit code
			DWORD exitCode;
			if (!GetExitCodeProcess(processHandle._handle, &exitCode))
				throw std::runtime_error("Execute GetExitCodeProcess Failed");

			// Close the child write handle to ensure we stop reading
			childStdOutWrite.Close();
			childStdErrWrite.Close();

			// Read all and write to stdout
			// TODO: May want to switch over to a background thread with peak to read in order
			DWORD dwRead = -1;
			const int BufferSize = 256;
			char buffer[BufferSize + 1];

			// Read on output
			while (true)
			{
				if(!ReadFile(childStdOutRead._handle, buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				buffer[dwRead] = '\0';
				outputCallback(buffer, dwRead);
			}

			// Read all errors
			while (true)
			{
				if(!ReadFile(childStdErrRead._handle, buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				buffer[dwRead] = '\0';
				errorCallback(buffer, dwRead);
			}

			return {
				static_cast<int>(exitCode),
				stdOut.str(),
				stdErr.str(),
			};
		}
	};
}
