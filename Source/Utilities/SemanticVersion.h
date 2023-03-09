// <copyright file="SemanticVersion.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Opal
{
	/// <summary>
	/// The semantic version class
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class SemanticVersion
	{
	public:
		/// <summary>
		/// Try parse the value
		/// </summary>
		static bool TryParse(const std::string_view value, SemanticVersion& result)
		{
			// TODO: Invert this so TryParse is the default and Parse adds an exception
			// it is way faster in the fail case
			try
			{
				result = Parse(value);
				return true;
			}
			catch (...)
			{
			}

			result = SemanticVersion();
			return false;
		}

		/// <summary>
		/// Parse the value
		/// </summary>
		static SemanticVersion Parse(const std::string_view value)
		{
			// Parse the integer values
			// TODO: Perform my own search to save string creation
			std::vector<short> values;
			auto stream = std::istringstream(std::string(value));
			std::string stringValue;
			while (std::getline(stream, stringValue, '.'))
			{
				auto intValue = std::stoi(stringValue);
				values.push_back(static_cast<short>(intValue));
			}
			
			if (values.size() < 1 || values.size() > 3)
			{
				throw std::runtime_error("The version string must one to three values.");
			}

			auto major = values[0];

			std::optional<int> minor = std::nullopt;
			if (values.size() >= 2)
				minor = values[1];

			std::optional<int> patch = std::nullopt;
			if (values.size() >= 3)
				patch = values[2];

			return SemanticVersion(
				major,
				minor,
				patch);
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		SemanticVersion() :
			_major(0),
			_minor(std::nullopt),
			_patch(std::nullopt)
		{
		}

		SemanticVersion(int major) :
			_major(major),
			_minor(std::nullopt),
			_patch(std::nullopt)
		{
		}

		SemanticVersion(int major, std::optional<int> minor) :
			_major(major),
			_minor(minor),
			_patch(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		SemanticVersion(int major, std::optional<int> minor, std::optional<int> patch) :
			_major(major),
			_minor(minor),
			_patch(patch)
		{
		}

		/// <summary>
		/// Gets or sets the version major
		/// </summary>
		int GetMajor() const
		{
			return _major;
		}

		/// <summary>
		/// Gets or sets the version minor
		/// </summary>
		bool HasMinor() const
		{
			return _minor.has_value();
		}
		int GetMinor() const
		{
			if (!HasMinor())
				throw std::runtime_error("Semantic version does not have a minor value");
			return _minor.value();
		}
		int GetMinorOrDefault() const
		{
			if (HasMinor())
				return _minor.value();
			else
				return 0;
		}

		/// <summary>
		/// Gets or sets the version patch
		/// </summary>
		bool HasPatch() const
		{
			return _patch.has_value();
		}
		int GetPatch() const
		{
			if (!HasPatch())
				throw std::runtime_error("Semantic version does not have a patch value");
			return _patch.value();
		}
		int GetPatchOrDefault() const
		{
			if (HasPatch())
				return _patch.value();
			else
				return 0;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const SemanticVersion& rhs) const
		{
			return _major == rhs._major &&
				GetMinorOrDefault() == rhs.GetMinorOrDefault() &&
				GetPatchOrDefault() == rhs.GetPatchOrDefault();
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const SemanticVersion& rhs) const
		{
			return _major != rhs._major ||
				GetMinorOrDefault() != rhs.GetMinorOrDefault() ||
				GetPatchOrDefault() != rhs.GetPatchOrDefault();
		}

		/// <summary>
		/// Comparison operator
		/// </summary>
		bool operator <(const SemanticVersion& rhs) const
		{
			return _major < rhs._major ||
				GetMinorOrDefault() < rhs.GetMinorOrDefault() ||
				GetPatchOrDefault() < rhs.GetPatchOrDefault();
		}
		bool operator >(const SemanticVersion& rhs) const
		{
			return _major > rhs._major ||
				GetMinorOrDefault() > rhs.GetMinorOrDefault() ||
				GetPatchOrDefault() > rhs.GetPatchOrDefault();
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		std::string ToString() const
		{
			// "{Major}(.{Minor}(.{Patch}))"
			std::stringstream stringBuilder;
			stringBuilder << _major;
			if (HasMinor())
			{
				stringBuilder << "." << GetMinor();
				if (HasPatch())
				{
					stringBuilder << "." << GetPatch();
				}
			}

			auto value = std::string(std::move(stringBuilder).str());
			return value;
		}

	private:
		int _major;
		std::optional<int> _minor;
		std::optional<int> _patch;
	};
}
