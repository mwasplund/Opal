// <copyright file="IProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// The process interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IProcess
	{
	public:
		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		virtual void Start() = 0;

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		virtual void WaitForExit() = 0;

		/// <summary>
		/// Get the exit code
		/// </summary>
		virtual int GetExitCode() = 0;

		/// <summary>
		/// Get the standard output
		/// </summary>
		virtual std::string GetStandardOutput() = 0;

		/// <summary>
		/// Get the standard error output
		/// </summary>
		virtual std::string GetStandardError() = 0;
	};
}
