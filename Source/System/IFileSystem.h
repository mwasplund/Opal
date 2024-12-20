﻿// <copyright file="IFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IInputFile.h"
#include "IOutputFile.h"

#ifdef SOUP_BUILD
export
#endif
namespace Opal::System
{
	struct DirectoryEntry
	{
		::Opal::Path Path;
		bool IsDirectory;
		uint64_t Size;
		std::time_t AccessTime;
		std::time_t CreateTime;
		std::time_t ModifiedTime;

		// TODO: Make generic
		uint32_t Attributes;
	};

	/// <summary>
	/// The file system interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	class IFileSystem
	{
	public:
		/// <summary>
		/// Gets the current active file system
		/// </summary>
		static IFileSystem& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No file system implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active file system
		/// </summary>
		static void Register(std::shared_ptr<IFileSystem> fileSystem)
		{
			_current = std::move(fileSystem);
		}

	public:
		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		virtual Path GetUserProfileDirectory() = 0;

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		virtual Path GetCurrentDirectory() = 0;

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		virtual bool Exists(const Path& path) = 0;

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		virtual bool TryGetLastWriteTime(const Path& path, std::filesystem::file_time_type& value) = 0;

		/// <summary>
		/// Set the last write time of the file/directory
		/// </summary>
		virtual void SetLastWriteTime(const Path& path, std::filesystem::file_time_type value) = 0;

		/// <summary>
		/// Get the last write time of all files in a directory
		/// </summary>
		virtual bool TryGetDirectoryFilesLastWriteTime(
			const Path& path,
			std::function<void(const Path& file, std::filesystem::file_time_type)>& callback) = 0;

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		virtual bool TryOpenRead(const Path& path, bool isBinary, std::shared_ptr<IInputFile>& result) = 0;
		virtual std::shared_ptr<IInputFile> OpenRead(const Path& path, bool isBinary) = 0;

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		virtual std::shared_ptr<IOutputFile> OpenWrite(const Path& path, bool isBinary) = 0;

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		virtual void Rename(const Path& source, const Path& destination) = 0;

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		virtual void CopyFile(const Path& source, const Path& destination) = 0;

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		virtual void CreateDirectory(const Path& path) = 0;

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		virtual std::vector<DirectoryEntry> GetDirectoryChildren(const Path& path) = 0;

		/// <summary>
		/// Delete the directory
		/// </summary>
		virtual void DeleteDirectory(const Path& path, bool recursive) = 0;

	private:
		static std::shared_ptr<IFileSystem> _current;
	};

#ifdef OPAL_IMPLEMENTATION
	std::shared_ptr<IFileSystem> IFileSystem::_current = nullptr;
#endif
}
