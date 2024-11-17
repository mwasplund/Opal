// <copyright file="MockSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ISystem.h"

namespace Opal::System
{
	/// <summary>
	/// The mock system
	/// TODO: Move into test project
	/// </summary>
	export class MockSystem : public ISystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockSystem'/> class.
		/// </summary>
		MockSystem() :
			_requests()
		{
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Get current system time
		/// </summary>
		std::filesystem::file_time_type GetCurrentTime() override final
		{
			std::stringstream message;
			message << "GetCurrentTime";
			_requests.push_back(message.str());

			return std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::time_point<std::chrono::system_clock>());
		}

	private:
		std::vector<std::string> _requests;
	};
}
