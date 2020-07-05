module;
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <locale>
#include <map>
#include <queue>
#include <sstream>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#include <shlobj.h>
#include <psapi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifdef CreateProcess
#undef CreateProcess
#endif
#elif defined(__linux__)
#else
#error Uknown Platform
#endif

export module Opal;

#include "Utilities/Path.h"
#include "Utilities/SemanticVersion.h"

#include "IO/SystemConsoleManager.h"
#include "IO/MockConsoleManager.h"
#include "IO/ScopedConsoleManagerRegister.h"

#include "Logger/Log.h"
#include "Logger/ConsoleTraceListener.h"
#include "Logger/ScopedTraceListenerRegister.h"
#include "Logger/TestTraceListener.h"

#include "Memory/IReferenceCounted.h"
#include "Memory/Reference.h"
#include "Memory/ReferenceCounted.h"

#include "System/MockEnvironment.h"
#include "System/MockFileSystem.h"
#include "System/MockProcessManager.h"
#include "System/ScopedFileSystemRegister.h"
#include "System/ScopedProcessManagerRegister.h"
#include "System/STLEnvironment.h"
#include "System/STLFileSystem.h"

#ifdef _WIN32
#include "System/SmartHandle.h"
#include "System/WindowsDynamicLibraryManager.h"
#include "System/WindowsProcessManager.h"
#endif