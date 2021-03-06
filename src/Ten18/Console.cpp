#include "Ten18/Console.h"
#include "Ten18/Expect.h"
#include "Ten18/Util.h"

using namespace Ten18;

static void RedirectFileStreamToConsole(DWORD stdFileHandle, FILE* fileStream)
{
    auto hstd = GetStdHandle(stdFileHandle);
    auto hcon = _open_osfhandle(checked_reinterpret_cast<std::intptr_t>(hstd), _O_TEXT);
    auto fp = _fdopen(hcon, "w");
    Ten18_ASSERT(fp != nullptr);
    *fileStream = *fp;
    
    const std::size_t numBytesToBuffer = 123;
    Ten18_EXPECT.Zero = std::setvbuf(fileStream, nullptr, numBytesToBuffer == 0 ? _IONBF : _IOFBF, numBytesToBuffer);
}

void Console::Initialize(const wchar_t* title)
{
    Ten18_EXPECT.True = AllocConsole();
    Ten18_EXPECT.True = SetConsoleTitle(title);

    RedirectFileStreamToConsole(STD_OUTPUT_HANDLE, stdout);
    RedirectFileStreamToConsole(STD_ERROR_HANDLE, stderr);    
}

void Console::Flush()
{
    Ten18_IGNORE = std::fflush(stdout);
    Ten18_IGNORE = std::fflush(stderr);
}

void Console::WriteLine(const char* fmt)
{
    DebugOut(fmt);
}
