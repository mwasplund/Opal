// <copyright file="MockFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"
#include "MockDirectory.h"
#include "MockFile.h"

namespace Opal::System
{
	/// <summary>
	/// The mock file system
	/// TODO: Move into test project
	/// </summary>
	export class MockFileSystem : public IFileSystem
	{
	private:
		std::vector<std::string> _requests;
		std::map<Path, std::shared_ptr<MockDirectory>> _directories;
		std::map<Path, std::shared_ptr<MockFile>> _files;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockFileSystem'/> class.
		/// </summary>
		MockFileSystem() :
			_requests(),
			_directories(),
			_files()
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
		/// Create a test directory
		/// </summary>
		void CreateMockDirectory(Path path, std::shared_ptr<MockDirectory> directory)
		{
			_directories.emplace(
				std::move(path),
				std::move(directory));
		}

		/// <summary>
		/// Create a test file
		/// </summary>
		void CreateMockFile(Path path, std::shared_ptr<MockFile> file)
		{
			_files.emplace(
				std::move(path),
				std::move(file));
		}

		/// <summary>
		/// Create a test file
		/// </summary>
		std::shared_ptr<MockFile> GetMockFile(Path path)
		{
			auto file = _files.find(path);
			if (file != _files.end())
			{
				return file->second;
			}
			else
			{
				auto message = "Cannot find file: " + path.ToString();
				throw std::runtime_error(message);
			}
		}

		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		Path GetUserProfileDirectory() override final
		{
			_requests.push_back("GetCurrentDirectory");

			return Path("C:/Users/Me/");
		}

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		Path GetCurrentDirectory() override final
		{
			_requests.push_back("GetCurrentDirectory");

			return Path("C:/Current/");
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		bool Exists(const Path& path) override final
		{
			std::stringstream message;
			message << "Exists: " << path.ToString();
			_requests.push_back(message.str());

			if (path.HasFileName())
			{
				auto file = _files.find(path);
				return file != _files.end();
			}
			else
			{
				auto directory = _directories.find(path);
				return directory != _directories.end();
			}
		}

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		bool TryGetLastWriteTime(const Path& path, std::filesystem::file_time_type& value) override final
		{
			std::stringstream message;
			message << "TryGetLastWriteTime: " << path.ToString();
			_requests.push_back(message.str());

			auto file = _files.find(path);
			if (file != _files.end())
			{
				value = file->second->LastWriteTime;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Get the last write time of all files in a directory
		/// </summary>
		bool TryGetDirectoryFilesLastWriteTime(
			const Path& path,
			std::function<void(const Path& file, std::filesystem::file_time_type)>& callback) override final
		{
			std::stringstream message;
			message << "TryGetDirectoryFilesLastWriteTime: " << path.ToString();
			_requests.push_back(message.str());

			auto directory = _directories.find(path);
			if (directory == _directories.end())
			{
				return false;
			}
			else
			{
				callback(Path("./"), directory->second->LastWriteTime);

				for (auto& child : directory->second->Children)
				{
					callback(child, directory->second->LastWriteTime);
				}

				return true;
			}
		}

		/// <summary>
		/// Set the last write time of the file/directory
		/// </summary>
		void SetLastWriteTime(const Path& path, std::filesystem::file_time_type /*value*/) override final
		{
			std::stringstream message;
			message << "SetLastWriteTime: " << path.ToString();
			_requests.push_back(message.str());
		}

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		bool TryOpenRead(const Path& path, bool isBinary, std::shared_ptr<IInputFile>& result) override final
		{
			std::stringstream message;
			if (isBinary)
			{
				message << "TryOpenReadBinary: " << path.ToString();
			}
			else
			{
				message << "TryOpenRead: " << path.ToString();
			}
			_requests.push_back(message.str());

			auto file = _files.find(path);
			if (file != _files.end())
			{
				// Reset the existing content offset and return it.
				auto& content = file->second->Content;
				content.seekg(0, std::ios_base::beg);
				result = std::make_shared<MockInputFile>(file->second);
				return true;
			}
			else
			{
				result = nullptr;
				return false;
			}
		}

		std::shared_ptr<IInputFile> OpenRead(const Path& path, bool isBinary) override final
		{
			std::stringstream message;
			if (isBinary)
			{
				message << "OpenReadBinary: " << path.ToString();
			}
			else
			{
				message << "OpenRead: " << path.ToString();
			}
			_requests.push_back(message.str());

			auto file = _files.find(path);
			if (file != _files.end())
			{
				// Reset the existing content offset and return it.
				auto& content = file->second->Content;
				content.seekg(0);
				return std::make_shared<MockInputFile>(file->second);
			}
			else
			{
				auto errorMessage = "Cannot open read: " + path.ToString();
				throw std::runtime_error(errorMessage);
			}
		}

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		std::shared_ptr<IOutputFile> OpenWrite(const Path& path, bool isBinary) override final
		{
			std::stringstream message;
			if (isBinary)
			{
				message << "OpenWriteBinary: " << path.ToString();
			}
			else
			{
				message << "OpenWrite: " << path.ToString();
			}
			_requests.push_back(message.str());

			auto file = _files.find(path);
			if (file != _files.end())
			{
				// Reset the existing content offset and return it.
				auto& content = file->second->Content;
				content.str("");
				content.clear();
				return std::make_shared<MockOutputFile>(file->second);
			}
			else
			{
				// Create the file if it does not exist
				auto insert = _files.emplace(path, std::make_shared<MockFile>());
				return std::make_shared<MockOutputFile>(insert.first->second);
			}
		}

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		virtual void Rename(const Path& source, const Path& destination) override final
		{
			std::stringstream message;
			message << "Rename: [" << source.ToString() << "] -> [" << destination.ToString() << "]";
			_requests.push_back(message.str());
		}

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		void CopyFile(const Path& source, const Path& destination) override final
		{
			std::stringstream message;
			message << "CopyFile: [" << source.ToString() << "] -> [" << destination.ToString() << "]";
			_requests.push_back(message.str());
		}

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		void CreateDirectory(const Path& path) override final
		{
			std::stringstream message;
			message << "CreateDirectory: " << path.ToString();
			_requests.push_back(message.str());
		}

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		std::vector<DirectoryEntry> GetDirectoryChildren(const Path& path) override final
		{
			std::stringstream message;
			message << "GetDirectoryChildren: " << path.ToString();
			_requests.push_back(message.str());

			auto result = std::vector<DirectoryEntry>();
			return result;
		}

		/// <summary>
		/// Delete the directory
		/// </summary>
		void DeleteDirectory(const Path& path, bool recursive) override final
		{
			std::stringstream message;
			if (recursive)
				message << "DeleteDirectoryRecursive: ";
			else
				message << "DeleteDirectory: ";
				
			message << path.ToString();
			_requests.push_back(message.str());
		}
	};
}
