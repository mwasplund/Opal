// <copyright file="ScopedSystemRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ISystem.h"

namespace Opal::System
{
	/// <summary>
	/// A scoped file system registration helper
	/// </summary>
	export class ScopedSystemRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedSystemRegister'/> class.
		/// </summary>
		ScopedSystemRegister(std::shared_ptr<ISystem> listener)
		{
			ISystem::Register(std::move(listener));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedSystemRegister'/> class.
		/// </summary>
		~ScopedSystemRegister()
		{
			ISystem::Register(nullptr);
		}
	};
}
