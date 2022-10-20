// <copyright file="ScopedLibraryManagerRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILibraryManager.h"

namespace Opal::System
{
	/// <summary>
	/// A scoped library manager registration helper
	/// </summary>
	export class ScopedLibraryManagerRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedLibraryManagerRegister'/> class.
		/// </summary>
		ScopedLibraryManagerRegister(std::shared_ptr<ILibraryManager> listener)
		{
			ILibraryManager::Register(std::move(listener));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedLibraryManagerRegister'/> class.
		/// </summary>
		~ScopedLibraryManagerRegister()
		{
			ILibraryManager::Register(nullptr);
		}
	};
}
