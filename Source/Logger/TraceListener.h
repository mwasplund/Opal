// <copyright file="TraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

#ifdef SOUP_BUILD
export
#endif
namespace Opal
{
	enum class TraceEventFlag : uint32_t
	{
		// High Priority message.
		HighPriority = 1 << 0,
		// Informational message.
		Information = 1 << 1,
		// Diagnostic trace.
		Diagnostic = 1 << 2,
		// Noncritical problem.
		Warning = 1 << 3,
		// Recoverable error.
		Error = 1 << 4,
		// Fatal error or application crash.
		Critical = 1 << 5,
	};

	class IEventFilter
	{
	public:
		virtual bool ShouldTrace(TraceEventFlag eventType) = 0;
	};

	/// <summary>
	/// Base trace listener used to determine what events and properties to include in logs
	/// </summary>
	class TraceListener
	{
	protected:
		/// <summary>
		/// Initializes a new instance of the <see cref='TraceListener'/> class.
		/// </summary>
		TraceListener() :
			TraceListener("", nullptr, true, true)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TraceListener'/> class using the specified name as the
		/// listener.
		/// </summary>
		TraceListener(
			std::string name,
			std::shared_ptr<IEventFilter> filter,
			bool showEventType,
			bool showEventId) :
			_name(std::move(name)),
			_filter(std::move(filter)),
			_showEventType(showEventType),
			_showEventId(showEventId)
		{
		}

		/// <summary>
		/// Implementation dependant write methods
		/// </summary>
		virtual void WriteLine(const std::string& message) = 0;

	public:
		/// <summary>
		/// Gets a value indicating whether there is a custom event filter
		/// </summary>
		bool HasFilter() const
		{
			return _filter != nullptr;
		}

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event type
		/// </summary>
		bool GetShowEventType() const
		{
			return _showEventType;
		}

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event id
		/// </summary>
		bool GetShowEventId() const
		{
			return _showEventId;
		}
		void SetShowEventId(bool value)
		{
			_showEventId = value;
		}

		/// <summary>
		/// All other TraceEvent methods come through this one.
		/// </summary>
		void TraceEvent(
			TraceEventFlag eventType,
			int id,
			std::string_view message)
		{
			if (HasFilter() && ! _filter->ShouldTrace(eventType))
			{
				return;
			}

			// Build up the resulting message with required header/footer
			auto builder = std::stringstream();
			WriteHeader(builder, eventType, id);
			builder << message;

			bool isEmpty = builder.rdbuf()->in_avail() == 0;
			if (isEmpty)
				WriteLine("");
			else
				WriteLine(builder.str());
		}

		/// <summary>
		/// Trace Event
		/// </summary>
		// void TraceEvent(
		//	 string source,
		//	 TraceEventFlag eventType,
		//	 int id,
		//	 string format,
		//	 params object[] args)
		// {
		//	 if (Filter != null && !Filter.ShouldTrace( source, eventType, id, format, args, null, null))
		//	 {
		//		 return;
		//	 }

		//	 WriteHeader(source, eventType, id);
		//	 if (args != null)
		//	 {
		//		 WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
		//	 }
		//	 else
		//	 {
		//		 WriteLine(format);
		//	 }
		// }

	private:
		/// <summary>
		/// Write the header to the target listener
		/// </summary>
		void WriteHeader(
			std::ostream& stream,
			TraceEventFlag eventType,
			int id)
		{
			if (GetShowEventType())
			{
				switch (eventType)
				{
					case TraceEventFlag::HighPriority:
						stream << "HIGH";
						break;
					case TraceEventFlag::Information:
						stream << "INFO";
						break;
					case TraceEventFlag::Diagnostic:
						stream << "DIAG";
						break;
					case TraceEventFlag::Warning:
						stream << "WARN";
						break;
					case TraceEventFlag::Error:
						stream << "ERRO";
						break;
					case TraceEventFlag::Critical: 
						stream << "CRIT";
						break;
					default:
						stream << "UNKN";
						break;
				}

				stream << ": ";
			}

			if (GetShowEventId())
			{
				stream << std::to_string(id) << ">";
			}
		}

	private:
		std::string _name;
		std::shared_ptr<IEventFilter> _filter;
		bool _showEventType;
		bool _showEventId;
	};
}
