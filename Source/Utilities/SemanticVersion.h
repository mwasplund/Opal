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
			// Parse the integer values
			auto firstSeparator = value.find_first_of('.');
			if (firstSeparator == std::string_view::npos)
			{
				uint16_t majorVersion;
				auto parseMajorResult = std::from_chars(
					value.data(),
					value.data() + value.size(),
					majorVersion);
				if (parseMajorResult.ec == std::errc::invalid_argument) {
					result = SemanticVersion();
					return false;
				}

				result = SemanticVersion(majorVersion);
				return true;
			}
			else
			{
				auto majorVersionText = value.substr(0, firstSeparator);
				uint16_t majorVersion;
				auto parseMajorResult = std::from_chars(
					majorVersionText.data(),
					majorVersionText.data() + majorVersionText.size(),
					majorVersion);
				if (parseMajorResult.ec == std::errc::invalid_argument) {
					result = SemanticVersion();
					return false;
				}

				auto lastSeparator = value.find_first_of('.', firstSeparator + 1);
				if (lastSeparator == std::string_view::npos)
				{
					auto minorVersionText = value.substr(firstSeparator + 1);
					uint16_t minorVersion;
					auto parseMinorResult = std::from_chars(
						minorVersionText.data(),
						minorVersionText.data() + minorVersionText.size(),
						minorVersion);
					if (parseMinorResult.ec == std::errc::invalid_argument) {
						result = SemanticVersion();
						return false;
					}

					result = SemanticVersion(majorVersion, minorVersion);
					return true;
				}
				else
				{
					auto minorVersionText = value.substr(firstSeparator + 1, lastSeparator - firstSeparator - 1);
					uint16_t minorVersion;
					auto parseMinorResult = std::from_chars(
						minorVersionText.data(),
						minorVersionText.data() + minorVersionText.size(),
						minorVersion);
					if (parseMinorResult.ec == std::errc::invalid_argument) {
						result = SemanticVersion();
						return false;
					}

					auto patchVersionText = value.substr(lastSeparator + 1);
					uint16_t patchVersion;
					auto parsePatchResult = std::from_chars(
						patchVersionText.data(),
						patchVersionText.data() + patchVersionText.size(),
						patchVersion);
					if (parsePatchResult.ec == std::errc::invalid_argument) {
						result = SemanticVersion();
						return false;
					}

					result = SemanticVersion(majorVersion, minorVersion, patchVersion);
					return true;
				}
			}
		}

		/// <summary>
		/// Parse the value
		/// </summary>
		static SemanticVersion Parse(const std::string_view value)
		{
			SemanticVersion result;
			if (TryParse(value, result))
				return result;
			else
				throw new std::runtime_error("Invalid semantic version");
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

		SemanticVersion(uint16_t major) :
			_major(major),
			_minor(std::nullopt),
			_patch(std::nullopt)
		{
		}

		SemanticVersion(uint16_t major, std::optional<uint16_t> minor) :
			_major(major),
			_minor(minor),
			_patch(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		SemanticVersion(uint16_t major, std::optional<uint16_t> minor, std::optional<uint16_t> patch) :
			_major(major),
			_minor(minor),
			_patch(patch)
		{
		}

		/// <summary>
		/// Gets or sets the version major
		/// </summary>
		uint16_t GetMajor() const
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
		uint16_t GetMinor() const
		{
			if (!HasMinor())
				throw std::runtime_error("Semantic version does not have a minor value");
			return _minor.value();
		}
		uint16_t GetMinorOrDefault() const
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
		uint16_t GetPatch() const
		{
			if (!HasPatch())
				throw std::runtime_error("Semantic version does not have a patch value");
			return _patch.value();
		}
		uint16_t GetPatchOrDefault() const
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
			auto result = std::to_string(_major);
			if (HasMinor())
			{
				result.append(".");
				result.append(std::to_string(_minor.value()));
				
				if (HasPatch())
				{
					result.append(".");
					result.append(std::to_string(_patch.value()));
					return result;
				}
			}

			return result;
		}

	private:
		uint16_t _major;
		std::optional<uint16_t> _minor;
		std::optional<uint16_t> _patch;
	};
}
