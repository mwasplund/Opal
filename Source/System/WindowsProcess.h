// <copyright file="WindowsProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcess.h"

namespace Opal::System
{
	/// <summary>
	/// A windows splatform specific process executable using system
	/// </summary>
	export class WindowsProcess : public IProcess
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsProcess'/> class.
		/// </summary>
		WindowsProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory) :
			m_executable(executable),
			m_arguments(arguments),
			m_workingDirectory(workingDirectory),
			m_threadHandle(),
			m_processHandle(),
			m_stdOutReadHandle(),
			m_stdOutWriteHandle(),
			m_stdErrReadHandle(),
			m_stdErrWriteHandle(),
			m_stdInReadHandle(),
			m_stdInWriteHandle(),
			m_isFinished(false),
			m_stdOut(),
			m_stdErr(),
			m_exitCode(-1)
		{
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		void Start() override final
		{
			std::stringstream argumentsValue;
			argumentsValue << "\"" << m_executable.ToAlternateString() << "\"" << " " << m_arguments;
			std::string argumentsString = argumentsValue.str();

			// Setup the input/output streams
			// TODO: We need to read from the buffer to ensure it doesn't deadlock on the wait forever
			int pipeBufferSize = 5 * 1024 * 1024;

			// Set the bInheritHandle flag so pipe handles are inherited.
			SECURITY_ATTRIBUTES securityAttributes; 
			securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
			securityAttributes.bInheritHandle = true; 
			securityAttributes.lpSecurityDescriptor = nullptr; 

			// Create a pipe for the child process's STDOUT.
			HANDLE childStdOutRead;
			HANDLE childStdOutWrite;
			if (!CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdOutReadHandle = SmartHandle(childStdOutRead);
			m_stdOutWriteHandle = SmartHandle(childStdOutWrite);

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!SetHandleInformation(m_stdOutReadHandle.Get(), HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDERR.
			HANDLE childStdErrRead;
			HANDLE childStdErrWrite;
			if (!CreatePipe(&childStdErrRead, &childStdErrWrite, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdErrReadHandle = SmartHandle(childStdErrRead);
			m_stdErrWriteHandle = SmartHandle(childStdErrWrite);

			// Ensure the read handle to the pipe for STDERR is not inherited.
			if (!SetHandleInformation(m_stdErrReadHandle.Get(), HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDIN.
			HANDLE childStdInRead;
			HANDLE childStdInWrite;
			if (!CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdInReadHandle = SmartHandle(childStdInRead);
			m_stdInWriteHandle = SmartHandle(childStdInWrite);

			// Ensure the write handle to the pipe for STDIN is not inherited.
			if (!SetHandleInformation(m_stdInWriteHandle.Get(), HANDLE_FLAG_INHERIT, 0))
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
			startupInfo.hStdError = m_stdErrWriteHandle.Get();
			startupInfo.hStdOutput = m_stdOutWriteHandle.Get();
			startupInfo.hStdInput = m_stdInReadHandle.Get();
			startupInfo.dwFlags |= STARTF_USESTDHANDLES;

			PROCESS_INFORMATION processInfo = {};
			ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

			// Start the process
			if (!CreateProcessA(
				m_executable.ToString().c_str(),
				argumentsString.data(),
				processAttributes,
				threadAttributes,
				inheritHandles,
				creationFlags,
				environment,
				m_workingDirectory.IsEmpty() ? nullptr : m_workingDirectory.ToString().c_str(),
				&startupInfo,
				&processInfo))
			{
				auto error = GetLastError();
				throw std::runtime_error("Execute CreateProcessA Failed: " + std::to_string(error));
			}

			// Store the runtime handles
			m_processHandle = SmartHandle(processInfo.hProcess);
			m_threadHandle = SmartHandle(processInfo.hThread);
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			// Wait until child process exits.
			auto waitResult = WaitForSingleObject(m_processHandle.Get(), INFINITE);
			switch (waitResult)
			{
				case WAIT_OBJECT_0:
					// All good
					break;
				case WAIT_ABANDONED:
					throw std::runtime_error("Execute WaitForSingleObject Abandoned");
					break;
				case WAIT_TIMEOUT:
					throw std::runtime_error("Execute WaitForSingleObject Timeout");
					break;
				case WAIT_FAILED:
					throw std::runtime_error("Execute WaitForSingleObject Failed: " + std::to_string(GetLastError()));
					break;
				default:
					throw std::runtime_error("Execute WaitForSingleObject Failed Unknown");
			}

			// Get the exit code
			DWORD exitCode;
			if (!GetExitCodeProcess(m_processHandle.Get(), &exitCode))
			{
				auto error = GetLastError();
				throw std::runtime_error("Execute GetExitCodeProcess Failed: " + std::to_string(error));
			}
			m_exitCode = exitCode;

			// Close the child write handle to ensure we stop reading
			m_stdOutWriteHandle.Close();
			m_stdErrWriteHandle.Close();

			// Read all and write to stdout
			// TODO: May want to switch over to a background thread with peak to read in order
			DWORD dwRead;
			const int BufferSize = 256;
			char buffer[BufferSize + 1];

			// Read on output
			while (true)
			{
				if(!ReadFile(m_stdOutReadHandle.Get(), buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				m_stdOut << std::string_view(buffer, dwRead);
			}

			// Read all errors
			while (true)
			{
				if(!ReadFile(m_stdErrReadHandle.Get(), buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				m_stdErr << std::string_view(buffer, dwRead);
			}

			m_isFinished = true;
		}

		/// <summary>
		/// Get the exit code
		/// </summary>
		int GetExitCode() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_exitCode;
		}

		/// <summary>
		/// Get the standard output
		/// </summary>
		std::string GetStandardOutput() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdOut.str();
		}

		/// <summary>
		/// Get the standard error output
		/// </summary>
		std::string GetStandardError() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdErr.str();
		}

	private:
		// Input
		Path m_executable;
		std::string m_arguments;
		Path m_workingDirectory;

		// Runtime
		SmartHandle m_processHandle;
		SmartHandle m_threadHandle;
		SmartHandle m_stdOutReadHandle;
		SmartHandle m_stdOutWriteHandle;
		SmartHandle m_stdErrReadHandle;
		SmartHandle m_stdErrWriteHandle;
		SmartHandle m_stdInReadHandle;
		SmartHandle m_stdInWriteHandle;

		// Result
		bool m_isFinished;
		std::stringstream m_stdOut;
		std::stringstream m_stdErr;
		int m_exitCode;
	};
}
