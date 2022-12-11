// <copyright file="MockProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcess.h"

namespace Opal::System
{
	/// <summary>
	/// A mock process executable using system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class MockProcess : public IProcess
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcess'/> class.
		/// </summary>
		MockProcess(
			int id,
			std::vector<std::string>& requests,
			int exitCode,
			std::string stdOut,
			std::string stdErr) :
			m_id(id),
			m_requests(requests),
			m_isFinished(false),
			m_stdOut(std::move(stdOut)),
			m_stdErr(std::move(stdErr)),
			m_exitCode(exitCode)
		{
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		void Start() override final
		{
			std::stringstream message;
			message << "ProcessStart: " << m_id;

			m_requests.push_back(message.str());
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			std::stringstream message;
			message << "WaitForExit: " << m_id;

			m_requests.push_back(message.str());

			m_isFinished = true;
		}

		/// <summary>
		/// Get the exit code
		/// </summary>
		int GetExitCode() override final
		{
			std::stringstream message;
			message << "GetExitCode: " << m_id;

			m_requests.push_back(message.str());

			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_exitCode;
		}

		/// <summary>
		/// Get the standard output
		/// </summary>
		std::string GetStandardOutput() override final
		{
			std::stringstream message;
			message << "GetStandardOutput: " << m_id;

			m_requests.push_back(message.str());

			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdOut;
		}

		/// <summary>
		/// Get the standard error output
		/// </summary>
		std::string GetStandardError() override final
		{
			std::stringstream message;
			message << "GetStandardError: " << m_id;

			m_requests.push_back(message.str());

			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdErr;
		}

	private:
		// Input
		int m_id;
		std::vector<std::string>& m_requests;

		// Result
		bool m_isFinished;
		std::string m_stdOut;
		std::string m_stdErr;
		int m_exitCode;
	};
}
