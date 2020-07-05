// <copyright file="SmartHandle.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	struct SmartHandle
	{
		SmartHandle(HANDLE handle) :
			_handle(handle)
		{
		}

		SmartHandle() :
			_handle(INVALID_HANDLE_VALUE)
		{
		}

		SmartHandle(const SmartHandle&) = delete;
		SmartHandle& operator=(const SmartHandle&) = delete;

		SmartHandle(SmartHandle&& other) :
			_handle(other._handle)
		{
			other._handle = INVALID_HANDLE_VALUE;
		}

		SmartHandle& operator=(SmartHandle&& other)
		{
			Close();
			_handle = other._handle;
			other._handle = INVALID_HANDLE_VALUE;
			return *this;
		}

		~SmartHandle()
		{
			Close();
		}

		void Close()
		{
			if (_handle != INVALID_HANDLE_VALUE)
			{
				if (!CloseHandle(_handle))
					throw "TODO: Better error";
				_handle = INVALID_HANDLE_VALUE;
			}
		}

		HANDLE _handle;
	};
}
