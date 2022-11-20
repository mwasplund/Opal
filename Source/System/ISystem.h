// <copyright file="ISystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// The system interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class ISystem
	{
	public:
		/// <summary>
		/// Gets the current active system
		/// </summary>
		static ISystem& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No system implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active system
		/// </summary>
		static void Register(std::shared_ptr<ISystem> system)
		{
			_current = std::move(system);
		}

	public:
		/// <summary>
		/// Get current system time
		/// </summary>
		virtual std::filesystem::file_time_type GetCurrentTime() = 0;

	private:
		static std::shared_ptr<ISystem> _current;
	};

	std::shared_ptr<ISystem> ISystem::_current = nullptr;
}
