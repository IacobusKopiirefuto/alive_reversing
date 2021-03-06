#include "stdafx.h"
#include "Sys.hpp"
#include "Function.hpp"
#include "Input.hpp"
#include "../AliveLibAE/Sys.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F7718, LPSTR, sCommandLine_9F7718, 0);
ALIVE_VAR(1, 0x9F771C, HINSTANCE, sInstance_9F771C, 0);
ALIVE_VAR(1, 0x9F772C, int, sCmdShow_9F772C, 0);
ALIVE_VAR(1, 0xA8A600, DWORD, sIsAKeyDown_A8A600, 0);

int Sys_IsAnyKeyDown_48E6C0()
{
    // AE impl
    if (!RunningAsInjectedDll())
    {
        return ::Sys_IsAnyKeyDown_4EDDF0();
    }

    // AO impl
    return sIsAKeyDown_A8A600;
}

void Sys_Main(HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd)
{
    sInstance_9F771C = hInstance;
    sCmdShow_9F772C = nShowCmd;
    sCommandLine_9F7718 = lpCmdLine;
    Sys_Main_Common();
}

EXPORT LPSTR CC Sys_GetCommandLine_48E920()
{
    return sCommandLine_9F7718;
}

EXPORT TWindowHandleType CC Sys_GetWindowHandle_48E930()
{
    AE_IMPLEMENTED();
    return Sys_GetWindowHandle_4EE180();
}

EXPORT void CC Sys_Set_Hwnd_48E340(TWindowHandleType hwnd)
{
    AE_IMPLEMENTED();
    Sys_Set_Hwnd_4F2C50(hwnd);
}

EXPORT int CC Sys_WindowClass_Register_48E9E0(LPCSTR lpClassName, LPCSTR lpWindowName, int X, int Y, int nWidth, int nHeight)
{
    AE_IMPLEMENTED();
    return Sys_WindowClass_Register_4EE22F(lpClassName, lpWindowName, X, Y, nWidth, nHeight);
}

EXPORT void SYS_EventsPump_44FF90()
{
    AE_IMPLEMENTED();
    SYS_EventsPump_494580();
}

}
