// <copyright file="Log.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "EventTypeFilter.h"

namespace Opal
{
	/// <summary>
	/// The static logger class
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class Log
	{
	public:
		/// <summary>
		/// Register the single event listener
		/// </summary>
		static void RegisterListener(std::shared_ptr<TraceListener> listener)
		{
			s_listener = std::move(listener);
		}

		/// <summary>
		/// Get access to the single event listener
		/// </summary>
		static TraceListener& EnsureListener()
		{
			if (s_listener == nullptr)
				throw std::runtime_error("No Listener registered.");
			return *s_listener;
		}

		/// <summary>
		/// Set the active ids to use for each event
		/// </summary>
		static int GetActiveId()
		{
			return s_activeId;
		}

		static void SetActiveId(int value)
		{
			s_activeId = value;
		}

		/// <summary>
		/// Log a high priority message
		/// </summary>
		template<typename... Args>
		static void HighPriority(std::string_view message, Args&&... args)
		{
			EnsureListener().TraceEvent(TraceEventFlag::HighPriority, s_activeId, message, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a generic informational message
		/// </summary>
		template<typename... Args>
		static void Info(std::string_view message, Args&&... args)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Information, s_activeId, message, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a diagnostic message
		/// </summary>
		template<typename... Args>
		static void Diag(std::string_view message, Args&&... args)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Diagnostic, s_activeId, message, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a warning message
		/// </summary>
		template<typename... Args>
		static void Warning(std::string_view message, Args&&... args)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Warning, s_activeId, message, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log an error message
		/// </summary>
		template<typename... Args>
		static void Error(std::string_view message, Args&&... args)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Error, s_activeId, message, std::forward<Args>(args)...);
		}

	private:
		static int s_activeId;
		static std::shared_ptr<TraceListener> s_listener;
	};

#ifdef OPAL_IMPLEMENTATION
	int Log::s_activeId = 0;
	std::shared_ptr<TraceListener> Log::s_listener = nullptr;
#endif
}
