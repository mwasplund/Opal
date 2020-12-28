// <copyright file="MockLibraryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILibraryManager.h"
#include "MockLibrary.h"

namespace Opal::System
{
	/// <summary>
	/// The mock library manager
	/// TODO: Move into test project
	/// </summary>
	export class MockLibraryManager : public ILibraryManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockLibraryManager'/> class.
		/// </summary>
		MockLibraryManager() :
			_libraries(),
			_requests()
		{
		}

		/// <summary>
		/// Register a known library
		/// </summary>
		std::shared_ptr<MockLibrary> RegisterLibrary(Path path)
		{
			auto library = std::make_shared<MockLibrary>(path.ToString(), _requests);
			_libraries.emplace(
				std::move(path),
				library);

			return library;
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Creates a Library for the provided executable path
		/// </summary>
		std::shared_ptr<ILibrary> LoadDynamicLibrary(
			const Path& path) override final
		{
			std::stringstream message;
			message << "LoadDynamicLibrary: " << path.ToString();

			_requests.push_back(message.str());

			auto library = _libraries.find(path);
			if (library != _libraries.end())
			{
				return library->second;
			}
			else
			{
				auto errorMessage = "Cannot load library: " + path.ToString();
				throw std::runtime_error(errorMessage);
			}
		}

	private:
		std::map<Path, std::shared_ptr<MockLibrary>> _libraries;
		std::vector<std::string> _requests;
	};
}
