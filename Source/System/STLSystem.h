// <copyright file="STLSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ISystem.h"

namespace Opal::System
{
	/// <summary>
	/// The standard library system
	/// </summary>
	export class STLSystem : public ISystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='STLSystem'/> class.
		/// </summary>
		STLSystem()
		{
		}

		/// <summary>
		/// Get current system time
		/// </summary>
		std::filesystem::file_time_type GetCurrentTime() override final
		{
			return std::chrono::file_clock::now();
		}
	};
}
