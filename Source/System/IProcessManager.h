﻿// <copyright file="IProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcess.h"

namespace Opal::System
{
	/// <summary>
	/// The process manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class IProcessManager
	{
	public:
		/// <summary>
		/// Gets the current active process manager
		/// </summary>
		static IProcessManager& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No process manager implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active process manager
		/// </summary>
		static void Register(std::shared_ptr<IProcessManager> value)
		{
			_current = std::move(value);
		}

	public:
		/// <summary>
		/// Gets the process file name
		/// </summary>
		virtual Path GetCurrentProcessFileName() = 0;

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		virtual std::shared_ptr<IProcess> CreateProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			bool interceptInputOutput) = 0;

	private:
		static std::shared_ptr<IProcessManager> _current;
	};

	std::shared_ptr<IProcessManager> IProcessManager::_current = nullptr;
}
