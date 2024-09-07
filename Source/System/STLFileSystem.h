// <copyright file="STLFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"
#include "STLInputFile.h"
#include "STLOutputFile.h"

namespace Opal::System
{
	/// <summary>
	/// The standard library file system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class STLFileSystem : public IFileSystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='STLFileSystem'/> class.
		/// </summary>
		STLFileSystem()
		{
		}

		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		Path GetUserProfileDirectory() override final
		{
			#ifdef _WIN32
				auto buffer = std::array<char, MAX_PATH + 2>();
				HRESULT result = SHGetFolderPath(
					nullptr,
					CSIDL_PROFILE,
					nullptr,
					SHGFP_TYPE_CURRENT,
					buffer.data());
				if (result != S_OK)
					throw std::runtime_error("SHGetFolderPath failed.");

				// Ensure the directory ends with a separator
				auto userProfileLength = strlen(buffer.data());
				buffer[userProfileLength] = '\\';
				buffer[userProfileLength + 1] = '\0';
				return Path::CreateWindows(std::string(buffer.data(), userProfileLength + 1));
			#elif defined(__linux__)
				return Path(std::getenv("HOME"));
			#else
				#error Unknown Platform
			#endif
		}

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		Path GetCurrentDirectory() override final
		{
			auto current = std::filesystem::current_path();
			return Path::CreateWindows(std::format("{}/", current.string()));
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		bool Exists(const Path& path) override final
		{
			return std::filesystem::exists(path.ToString());
		}

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		bool TryGetLastWriteTime(const Path& path, std::filesystem::file_time_type& value) override final
		{
			#ifdef _WIN32
				WIN32_FILE_ATTRIBUTE_DATA attrs;
				if (!GetFileAttributesExA(path.ToString().c_str(), GetFileExInfoStandard, &attrs))
				{
					DWORD error = GetLastError();
					if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
						return false;
					else
						throw std::runtime_error("Unexpected error get last write time: " + path.ToString());
				}
				else
				{
					auto fileDuration = std::chrono::file_clock::duration(
						(static_cast<uint64_t>(attrs.ftLastWriteTime.dwHighDateTime) << 32) |
						static_cast<uint64_t>(attrs.ftLastWriteTime.dwLowDateTime));
					value = std::filesystem::file_time_type(fileDuration);
					return true;
				}
			#else
				std::error_code error;
				auto fileTime = std::filesystem::last_write_time(path.ToString(), error);
				if (error)
				{
					return false;
				}
				else
				{
					value = fileTime;
					return true;
				}
			#endif
		}

		/// <summary>
		/// Get the last write time of all files in a directory
		/// </summary>
		bool TryGetDirectoryFilesLastWriteTime(
			const Path& path,
			std::function<void(const Path& file, std::filesystem::file_time_type)>& callback) override final
		{
			if (path.HasFileName())
				throw std::runtime_error("Path was not a directory");

			#ifdef _WIN32
				auto searchPath = path.ToString() + "*";

				WIN32_FIND_DATAA findData;
				HANDLE findHandle = FindFirstFileExA(
					searchPath.c_str(),
					FindExInfoBasic,
					&findData,
					FindExSearchNameMatch,
					nullptr,
					0);

				if (findHandle == INVALID_HANDLE_VALUE)
				{
					auto error = GetLastError();
					if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
						return false;
					else
						throw std::runtime_error("FindFirstFileExA Failed");
				}

				do
				{
					std::string_view fileName = findData.cFileName;
					auto filePath = Path();
					if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	 				{
						if (fileName == std::string_view(".."))
						{
							continue;
						}
						else if (fileName == std::string_view("."))
						{
							continue;
						}
						else
						{
							// Encode for Path
							filePath = Path(std::format("./{}/", fileName));
						}
					}
					else
					{
						filePath = Path(std::format("./{}", fileName));
					}

					auto lastWriteTimeDuration = std::chrono::file_clock::duration(
						(static_cast<uint64_t>(findData.ftLastWriteTime.dwHighDateTime) << 32) |
						static_cast<uint64_t>(findData.ftLastWriteTime.dwLowDateTime));
					auto lastWriteTime = std::filesystem::file_time_type(lastWriteTimeDuration);
					callback(filePath, lastWriteTime);
				} while (FindNextFileA(findHandle, &findData));

				if (!FindClose(findHandle))
					throw std::runtime_error("Failed to close find handle");

				return true;
			#else
				throw std::runtime_error("Not Implemented");
			#endif
		}

		/// <summary>
		/// Set the last write time of the file/directory
		/// </summary>
		void SetLastWriteTime(const Path& path, std::filesystem::file_time_type value) override final
		{
			std::filesystem::last_write_time(path.ToString(), value);
		}

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		bool TryOpenRead(const Path& path, bool isBinary, std::shared_ptr<IInputFile>& result) override final
		{
			std::ios_base::openmode mode = std::fstream::in;
			if (isBinary)
			{
				mode = static_cast<std::ios_base::openmode>(mode | std::fstream::binary);
			}

			auto file = std::ifstream(path.ToString(), mode);
			if (file.fail())
			{
				result = nullptr;
				return false;
			}
			else
			{
				result = std::make_shared<STLInputFile>(std::move(file));
				return true;
			}
		}

		std::shared_ptr<IInputFile> OpenRead(const Path& path, bool isBinary) override final
		{
			std::ios_base::openmode mode = std::fstream::in;
			if (isBinary)
			{
				mode = static_cast<std::ios_base::openmode>(mode | std::fstream::binary);
			}

			auto file = std::ifstream(path.ToString(), mode);
			if (file.fail())
			{
				auto message = "OpenRead Failed: File missing. " + path.ToString();
				throw std::runtime_error(std::move(message));
			}

			return std::make_shared<STLInputFile>(std::move(file));
		}

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		std::shared_ptr<IOutputFile> OpenWrite(const Path& path, bool isBinary) override final
		{
			std::ios_base::openmode mode = std::fstream::out;
			if (isBinary)
			{
				mode = static_cast<std::ios_base::openmode>(mode | std::fstream::binary);
			}

			auto file = std::ofstream(path.ToString(), mode);
			if (file.fail())
			{
				auto message = "OpenWrite Failed: " + path.ToString();
				throw std::runtime_error(std::move(message));
			}

			return std::make_shared<STLOutputFile>(std::move(file));
		}

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		virtual void Rename(const Path& source, const Path& destination) override final
		{
			std::filesystem::rename(
				source.ToString(),
				destination.ToString());
		}

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		void CopyFile(const Path& source, const Path& destination) override final
		{
			std::filesystem::copy(
				source.ToString(),
				destination.ToString(),
				std::filesystem::copy_options::overwrite_existing);
		}

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		void CreateDirectory(const Path& path) override final
		{
			std::filesystem::create_directories(path.ToString());
		}

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		std::vector<DirectoryEntry> GetDirectoryChildren(const Path& path) override final
		{
			auto result = std::vector<DirectoryEntry>();
			for(auto& child : std::filesystem::directory_iterator(path.ToString()))
			{
				auto directoryEntry = DirectoryEntry();
				directoryEntry.Path = Path(child.path().string());
				directoryEntry.IsDirectory = child.is_directory();

				// TODO: Remove when C++20 is ready
				#if defined (_WIN32)
					struct _stat64 fileInfo;
					auto pathString = directoryEntry.Path.ToString();
					if (_stat64(pathString.c_str(), &fileInfo) != 0)
						throw std::runtime_error("Failed to get file stats.");
					directoryEntry.Size = fileInfo.st_size;
					directoryEntry.AccessTime = fileInfo.st_atime;
					directoryEntry.ModifiedTime = fileInfo.st_mtime;
					directoryEntry.CreateTime = fileInfo.st_ctime;
					directoryEntry.Attributes = GetFileAttributesA(pathString.c_str());
				#else
					throw std::runtime_error("GetLastWriteTime: Not Implemented");
				#endif

				result.push_back(std::move(directoryEntry));
			}

			return result;
		}

		/// <summary>
		/// Delete the directory
		/// </summary>
		void DeleteDirectory(const Path& path, bool recursive) override final
		{
			if (recursive)
			{
				std::filesystem::remove_all(path.ToString());
			}
			else
			{
				std::filesystem::remove(path.ToString());
			}
		}
	};
}
