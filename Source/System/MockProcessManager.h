// <copyright file="MockProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"
#include "MockProcess.h"

namespace Opal::System
{
	/// <summary>
	/// The mock process manager
	/// TODO: Move into test project
	/// </summary>
	export class MockProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		MockProcessManager() :
			m_uniqueId(1),
			_requests(),
			_processFileName(Path("C:/testlocation/SoupCMDTest.exe")),
			_executeResults(),
			_executeCallbacks()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		MockProcessManager(Path processFileName) :
			_requests(),
			_processFileName(std::move(processFileName)),
			_executeResults(),
			_executeCallbacks()
		{
		}

		/// <summary>
		/// Create a result 
		/// </summary>
		void RegisterExecuteResult(std::string command, std::string output)
		{
			_executeResults.emplace(
				std::move(command),
				std::move(output));
		}

		/// <summary>
		/// Create a callback 
		/// </summary>
		void RegisterExecuteCallback(std::string command, std::function<void()> callback)
		{
			_executeCallbacks.emplace(
				std::move(command),
				std::move(callback));
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Gets the process file name
		/// </summary>
		Path GetCurrentProcessFileName() override final
		{
			std::stringstream message;
			message << "GetCurrentProcessFileName";

			_requests.push_back(message.str());
			return _processFileName;
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		std::shared_ptr<IProcess> CreateProcess(
			const Path& executable,
			std::vector<std::string> arguments,
			const Path& workingDirectory,
			bool interceptInputOutput) override final
		{
			std::stringstream message;
			auto id = m_uniqueId++;
			message << "CreateProcess: " << id << " " << interceptInputOutput << " [" << workingDirectory.ToString() << "] " << executable.ToString();
			for (auto& argument : arguments)
				message << " " << argument;

			_requests.push_back(message.str());

			// Check if there is a registered output
			auto findOutput = _executeResults.find(message.str());
			if (findOutput != _executeResults.end())
			{
				return std::make_shared<MockProcess>(
					id,
					_requests,
					0,
					findOutput->second,
					std::string());
			}
			else
			{
				return std::make_shared<MockProcess>(
					id,
					_requests,
					0,
					std::string(),
					std::string());
			}
			
			// Check if there is a registered callback
			auto findCallback = _executeCallbacks.find(message.str());
			if (findCallback != _executeCallbacks.end())
			{
				findCallback->second();
			}
		}

	private:
		std::atomic<int> m_uniqueId;
		std::vector<std::string> _requests;
		Path _processFileName;
		std::map<std::string, std::string> _executeResults;
		std::map<std::string, std::function<void()>> _executeCallbacks;
	};
}
