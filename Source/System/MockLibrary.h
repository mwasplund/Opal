// <copyright file="MockLibrary.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILibrary.h"

namespace Opal::System
{
	/// <summary>
	/// A mock library executable using system
	/// </summary>
	export class MockLibrary : public ILibrary
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockLibrary'/> class.
		/// </summary>
		MockLibrary(
			std::string name,
			std::vector<std::string>& requests) :
			m_name(std::move(name)),
			m_requests(requests),
			_functions()
		{
		}

		/// <summary>
		/// Register a known function pointer
		/// </summary>
		void RegisterFunction(std::string name, void* function)
		{
			_functions.emplace(
				std::move(name),
				function);
		}

		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		void* GetFunction(std::string_view name) override final
		{
			std::stringstream message;
			message << "GetFunction: " << m_name << " [" << name << "]";

			m_requests.push_back(message.str());

			auto function = _functions.find(name.data());
			if (function != _functions.end())
			{
				return function->second;
			}
			else
			{
				auto errorMessage = "Cannot find function: " + std::string(name);
				throw std::runtime_error(errorMessage);
			}
		}

	private:
		// Input
		std::string m_name;
		std::vector<std::string>& m_requests;

		std::map<std::string, void*> _functions;
	};
}
