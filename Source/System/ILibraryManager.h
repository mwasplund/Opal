// <copyright file="ILibraryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILibrary.h"

namespace Opal::System
{
	/// <summary>
	/// The library manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class ILibraryManager
	{
	public:
		/// <summary>
		/// Gets the current active library manager
		/// </summary>
		static ILibraryManager& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No library manager implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active library manager
		/// </summary>
		static void Register(std::shared_ptr<ILibraryManager> value)
		{
			_current = std::move(value);
		}

	public:
		/// <summary>
		/// Loads a library with the provided path
		/// </summary>
		virtual std::shared_ptr<ILibrary> LoadDynamicLibrary(
			const Path& library) = 0;

	private:
		static std::shared_ptr<ILibraryManager> _current;
	};

	std::shared_ptr<ILibraryManager> ILibraryManager::_current = nullptr;
}
