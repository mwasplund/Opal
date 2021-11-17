// <copyright file="Path.hs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Opal
{
	/// <summary>
	/// A container for a path string
	/// ROOT: The optional root of the path, if not present then the path is relative and must
	/// start with either the RelativeParentDirectory or RelativeDirectory special symbols
	///  '/' - Rooted in the current drive
	///  'A-Z:' - Rooted in a letter drive (Windows Specific)
	///  '//' - Server root
	/// </summary>
	export class Path
	{
	private:
		static constexpr char DirectorySeparator = '/';
		static constexpr char AlternateDirectorySeparator = '\\';
		static constexpr std::string_view AllValidDirectorySeparators = "/\\";
		static constexpr char LetterDriveSpecifier = ':';
		static constexpr char FileExtensionSeparator = '.';
		static constexpr std::string_view RelativeDirectory = ".";
		static constexpr std::string_view RelativeParentDirectory = "..";

	public:
		static Path Load(std::string value)
		{
			auto result = Path();

			result.LoadDirect(std::move(value));

			return result;
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Path"/> class.
		/// </summary>
		Path() :
			_value(),
			_rootEndLocation(),
			_fileNameStartLocation()
		{
			SetState({ RelativeDirectory }, "", "");
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Path"/> class.
		/// </summary>
		Path(std::string_view value) :
			_value(),
			_rootEndLocation(),
			_fileNameStartLocation()
		{
			ParsePath(value);
		}

		/// <summary>
		/// Gets a value indicating whether the path is empty
		/// </summary>
		bool IsEmpty() const
		{
			return _value == "./";
		}

		/// <summary>
		/// Gets a value indicating whether the path has a root
		/// </summary>
		bool HasRoot() const
		{
			return _rootEndLocation > 0;
		}

		/// <summary>
		/// Gets the path root
		/// </summary>
		std::string_view GetRoot() const
		{
			return std::string_view(_value.data(), _rootEndLocation);
		}

		/// <summary>
		/// Gets the parent directory
		/// </summary>
		Path GetParent() const
		{
			auto result = Path();

			// Take the root from the left hand side
			result._rootEndLocation = _rootEndLocation;

			// If there is a filename then return the directory
			// Otherwise return one less directory
			if (HasFileName())
			{
				// Pass along the path minus the filename
				result._value = _value.substr(0, _fileNameStartLocation);
				result._fileNameStartLocation = result._value.size();
			}
			else
			{
				// Pull apart the directories and remove the last one
				// TODO: This can be done in place and then a substring returned for perf gains
				auto directories = DecomposeDirectoriesString(GetDirectories());
				if (directories.empty())
				{
					// No-op when at the root
				}
				else if (directories.size() == 1 && directories[0] == RelativeDirectory)
				{
					// If this is only a relative folder symbol then replace with the parent symbol
					directories[0] = RelativeParentDirectory;
				}
				else if (directories.back() == RelativeParentDirectory)
				{
					// If this is entirely parent directories then add one more
					directories.push_back(RelativeParentDirectory);
				}
				else
				{
					// Otherwise pop off the top level folder
					directories.pop_back();
				}

				// Set the state of the result path
				result.SetState(directories, GetRoot(), "");
			}

			return result;
		}

		/// <summary>
		/// Gets a value indicating whether the path has a file name
		/// </summary>
		bool HasFileName() const
		{
			return _fileNameStartLocation < _value.size();
		}

		/// <summary>
		/// Gets the file name
		/// </summary>
		const std::string_view GetFileName() const
		{
			// Use the start location to return the end of the value that is the filename
			return std::string_view(
				_value.data() + _fileNameStartLocation,
				_value.size() - _fileNameStartLocation);
		}

		/// <summary>
		/// Gets a value indicating whether the file name has an stem
		/// </summary>
		bool HasFileStem() const
		{
			return !GetFileStem().empty();
		}

		/// <summary>
		/// Gets the file name minus the extension
		/// </summary>
		const std::string_view GetFileStem() const
		{
			// Everything before the last period is the stem
			auto fileName = GetFileName();
			auto lastSeparator = fileName.find_last_of(FileExtensionSeparator);
			if (lastSeparator != std::string::npos)
			{
				return fileName.substr(0, lastSeparator);
			}
			else
			{
				// Return the entire filename if no extension
				return fileName;
			}
		}

		/// <summary>
		/// Gets a value indicating whether the file name has an extension
		/// </summary>
		bool HasFileExtension() const
		{
			return !GetFileExtension().empty();
		}

		/// <summary>
		/// Gets the file extension
		/// </summary>
		const std::string_view GetFileExtension() const
		{
			// Everything after and including the last period is the extension
			auto fileName = GetFileName();
			auto lastSeparator = fileName.find_last_of(FileExtensionSeparator);
			if (lastSeparator != std::string::npos)
			{
				return fileName.substr(lastSeparator);
			}
			else
			{
				// Return the empty string
				return "";
			}
		}

		/// <summary>
		/// Set the filename
		/// </summary>
		void SetFilename(std::string_view value)
		{
			// Build the new final string
			SetState(
				DecomposeDirectoriesString(GetDirectories()),
				GetRoot(),
				value);
		}

		/// <summary>
		/// Set the file extension
		/// </summary>
		void SetFileExtension(std::string_view value)
		{
			// Build up the new filename and set the active state
			std::stringstream stringBuilder;
			stringBuilder << GetFileStem() << FileExtensionSeparator << value;
			SetFilename(stringBuilder.str());
		}

		/// <summary>
		/// Get a path relative to the provided base
		/// </summary>
		Path GetRelativeTo(const Path& base)
		{
			// If the root does not match then there is no way to get a relative path
			// simply return a copy of this path
			if ((base.HasRoot() && HasRoot() && base.GetRoot() != this->GetRoot()) ||
				(base.HasRoot() ^ this->HasRoot()))
			{
				return *this;
			}

			// Force the base filenames as directories
			auto baseDirectories = DecomposeDirectoriesString(base.GetDirectories());
			if (base.HasFileName())
			{
				baseDirectories.push_back(base.GetFileName());
			}

			// Determine how many of the directories match
			auto directories = DecomposeDirectoriesString(GetDirectories());
			auto minDirectories = std::min(baseDirectories.size(), directories.size());
			size_t countMatching = 0;
			for (auto i = 0u; i < minDirectories; i++)
			{
				if (baseDirectories[i] != directories[i])
				{
					break;
				}

				countMatching++;
			}

			// Add in up directories for any not matching in the base
			auto resultDirectories = std::vector<std::string_view>();
			if (countMatching == baseDirectories.size())
			{
				// Start with a single relative directory when no up directories required
				resultDirectories.push_back(RelativeDirectory);
			}
			else
			{
				for (auto i = countMatching; i < baseDirectories.size(); i++)
				{
					resultDirectories.push_back(RelativeParentDirectory);
				}
			}

			// Copy over the remaining entities from the target path
			for (auto i = countMatching; i < directories.size(); i++)
			{
				resultDirectories.push_back(directories[i]);
			}

			// Set the result path with no root
			auto result = Path();
			result.SetState(
				resultDirectories,
				"",
				GetFileName());

			return result;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const Path& rhs) const
		{
			return _value == rhs._value;
		}

		bool operator !=(const Path& rhs) const
		{
			return _value != rhs._value;
		}

		bool operator <(const Path& rhs) const
		{
			// TODO: Tests and better implementation
			return _value < rhs._value;
		}

		/// <summary>
		/// Concatenate paths
		/// </summary>
		Path operator +(const Path& rhs) const
		{
			if (rhs.HasRoot())
			{
				throw std::runtime_error(
					"Cannot combine a rooted path on the right hand side: " + rhs.ToString());
			}

			// Combine the directories
			auto resultDirectories = DecomposeDirectoriesString(GetDirectories());

			// Convert the left hand side filename to a directory
			if (HasFileName())
			{
				resultDirectories.push_back(GetFileName());
			}

			auto rhsDirectories = DecomposeDirectoriesString(rhs.GetDirectories());
			resultDirectories.insert(
				resultDirectories.end(),
				rhsDirectories.begin(),
				rhsDirectories.end());

			NormalizeDirectories(resultDirectories, HasRoot());

			// Set the state with the root from the LHS and the filename from the RHS
			auto result = Path();
			result.SetState(
				resultDirectories,
				GetRoot(),
				rhs.GetFileName());

			return result;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		const std::string& ToString() const
		{
			return _value;
		}

		std::string ToAlternateString() const
		{
			// Replace all normal separators with the windows version
			auto result = _value;
			std::replace(result.begin(), result.end(), DirectorySeparator, AlternateDirectorySeparator);
			return result;
		}

	private:
		/// <summary>
		/// Helper that loads a string directly into the path value
		/// </summary>
		void LoadDirect(std::string value)
		{
			_value = std::move(value);

			auto firstSeparator = _value.find_first_of(DirectorySeparator);
			if (firstSeparator != std::string::npos && IsRoot(std::string_view(_value.c_str(), firstSeparator)))
			{
				_rootEndLocation = firstSeparator;
			}

			auto lastSeparator = _value.find_last_of(DirectorySeparator);
			if (lastSeparator != std::string::npos && lastSeparator != _value.size() - 1)
			{
				_fileNameStartLocation = lastSeparator + 1;
			}
			else
			{
				_fileNameStartLocation = _value.size();
			}
		}

		void ParsePath(std::string_view value)
		{
			// Break out the individual components of the path
			std::vector<std::string_view> directories;
			std::string_view root;
			std::string_view fileName;
			DecomposeRawPathString(
				value,
				directories,
				root,
				fileName);

			// Normalize any unnecessary directories in the raw path
			bool hasRoot = !root.empty();
			NormalizeDirectories(directories, hasRoot);

			// Rebuild the string value
			SetState(
				directories,
				root,
				fileName);
		}

		void DecomposeRawPathString(
			std::string_view value,
			std::vector<std::string_view>& directories,
			std::string_view& root,
			std::string_view& fileName)
		{
			size_t current = 0;
			size_t next = 0;
			bool isFirst = true;
			while ((next = value.find_first_of(AllValidDirectorySeparators, current)) != std::string::npos)
			{
				auto directory = value.substr(current, next - current);

				// Check if the first entry is a root
				if (isFirst)
				{
					if (IsRoot(directory))
					{
						root = directory;
					}
					else
					{
						// Ensure that the unrooted path starts with a relative symbol
						if (!IsRelativeDirectory(directory))
						{
							directories.push_back(RelativeDirectory);
						}

						directories.push_back(directory);
					}

					isFirst = false;
				}
				else
				{
					directories.push_back(directory);
				}

				current = next + 1;
			}

			// Check if there are characters beyond the last separator
			if (current != value.size())
			{
				auto directory = value.substr(current, next - current);

				// Check if still on the first entry
				// Could be empty root or single filename
				if (isFirst)
				{
					if (IsRoot(directory))
					{
						root = directory;
					}
					else
					{
						// Ensure that the unrooted path starts with a relative symbol
						if (!IsRelativeDirectory(directory))
						{
							directories.push_back(RelativeDirectory);
						}

						fileName = directory;
					}

					isFirst = false;
				}
				else
				{
					fileName = directory;
				}
			}

			// If we saw nothing then add a single relative directory
			if (isFirst)
			{
				directories.push_back(RelativeDirectory);
			}
		}

		bool IsRelativeDirectory(const std::string_view directory)
		{
			return directory == RelativeDirectory || directory == RelativeParentDirectory;
		}

		std::vector<std::string_view> DecomposeDirectoriesString(std::string_view value) const
		{
			size_t current = 0;
			size_t next = 0;
			auto directories = std::vector<std::string_view>();
			while ((next = value.find_first_of(DirectorySeparator, current)) != std::string::npos)
			{
				auto directory = value.substr(current, next - current);
				if (!directory.empty())
				{
					directories.push_back(directory);
				}

				current = next + 1;
			}

			// Ensure the last separator was at the end of the string
			if (current != value.size())
			{
				throw std::runtime_error("The directories string must end in a separator");
			}

			return directories;
		}

		bool IsRoot(std::string_view value)
		{
			// Check for drive letter
			if (value.size() == 2)
			{
				if (std::isalpha(value[0]) && value[1] == LetterDriveSpecifier)
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Resolve any up directory tokens or empty (double separator) directories that are inside a path
		/// </summary>
		void NormalizeDirectories(
			std::vector<std::string_view>& directories,
			bool hasRoot) const
		{
			// Remove as many up directories as we can
			for (size_t i = 0; i < directories.size(); i++)
			{
				// Remove empty directories (double separator) or relative directories if rooted or not at start
				if (directories.at(i).empty() ||
					((hasRoot || i != 0) && directories.at(i) == RelativeDirectory))
				{
					directories.erase(
						directories.begin() + (i),
						directories.begin() + (i + 1));
					i -= 1;
				}
				else
				{
					// Check if we can combine any parent directories
					// Allow the first directory to remain a parent
					if (i != 0)
					{
						// Remove a parent directory if possible
						if (directories.at(i) == RelativeParentDirectory &&
							directories.at(i - 1) != RelativeParentDirectory)
						{
							// If the previous is a relative then just replace it
							if (directories.at(i - 1) == RelativeDirectory)
							{
								directories.erase(
									directories.begin() + (i - 1),
									directories.begin() + (i));
								i -= 1;
							}
							else
							{
								// Remove the directories and move back
								directories.erase(
									directories.begin() + (i - 1),
									directories.begin() + (i + 1));
								i -= 2;
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// Convert the components of the path into the string value
		/// </summary>
		void SetState(
			const std::vector<std::string_view>& directories,
			std::string_view root,
			std::string_view fileName)
		{
			std::stringstream stringBuilder;

			if (!root.empty())
			{
				stringBuilder << root << DirectorySeparator;
			}

			for (size_t i = 0; i < directories.size(); i++)
			{
				stringBuilder << directories[i] << DirectorySeparator;
			}

			if (!fileName.empty())
			{
				stringBuilder << fileName;
			}

			// Store the persistant state
			_value = stringBuilder.str();
			_rootEndLocation = root.size();
			_fileNameStartLocation = _value.size() - fileName.size();
		}

		const std::string_view GetDirectories() const
		{
			return std::string_view(
				_value.data() + _rootEndLocation,
				_fileNameStartLocation - _rootEndLocation);
		}

	private:
		std::string _value;
		size_t _rootEndLocation;
		size_t _fileNameStartLocation;
	};
}
