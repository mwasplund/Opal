// <copyright file="IFile.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// The file interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class IFile
	{
	public:
		/// <summary>
		/// Close the file
		/// </summary>
		virtual void Close() = 0;
	};
}
