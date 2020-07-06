// <copyright file="SmartHandle.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	export class SmartHandle
	{
	public:
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

		HANDLE Get()
		{
			return _handle;
		}

		bool IsValid()
		{
			return _handle != INVALID_HANDLE_VALUE;
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

	private:
		HANDLE _handle;
	};
}
