// <copyright file="ConsoleTraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "TraceListener.h"

namespace Opal
{
	// MSVC Bug https://developercommunity.visualstudio.com/t/Modules-ICE-when-using-cout-inside-of-/10299789
	void DoWriteLine(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	/// <summary>
	/// Console logger that wraps the base <see cref="TraceListener"/>
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class ConsoleTraceListener : public TraceListener
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ConsoleTraceListener'/> class.
		/// </summary>
		ConsoleTraceListener() :
			TraceListener()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='ConsoleTraceListener'/> class.
		/// </summary>
		ConsoleTraceListener(
			std::string name,
			std::shared_ptr<IEventFilter> filter,
			bool showEventType,
			bool showEventId) :
			TraceListener(
				std::move(name),
				std::move(filter),
				showEventType,
				showEventId)
		{
		}

		/// <summary>
		/// Writes a message and newline terminator
		/// </summary>
		virtual void WriteLine(const std::string& message) override final
		{
			// if (NeedIndent)
			// {
			//	 WriteIndent();
			// }

			SetConsoleColor();
			DoWriteLine(message);
			// TODO: restore color

			// NeedIndent = true;
		}

	private:
		void SetConsoleColor()
		{
			// TODO
			// switch (_currentEvent)
			// {
			//	 case TraceEventType.Error:
			//		 Console.ForegroundColor = ConsoleColor.Red;
			//		 break;
			//	 case TraceEventType.Warning:
			//		 Console.ForegroundColor = ConsoleColor.Yellow;
			//		 break;
			// }
		}
	};
}
