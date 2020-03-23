#include <windows.h>
#include "../config.h"
#include "../minhook/include/MinHook.h"

typedef int (WINAPI *FN_MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);

static FN_MessageBoxW fn_MessageBoxW = NULL;

INT WINAPI
DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    return (*fn_MessageBoxW)(hWnd, L"Hooked!", lpCaption, uType);
}

BOOL DoHook(void)
{
    return (MH_CreateHookCast(&MessageBoxW, &DetourMessageBoxW, &fn_MessageBoxW) == MH_OK);
}

BOOL DoEnableHook(BOOL bEnable)
{
    if (bEnable)
    {
        if (MH_EnableHookCast(&MessageBoxW) != MH_OK)
            return FALSE;
    }
    else
    {
        if (MH_DisableHookCast(&MessageBoxW) != MH_OK)
            return FALSE;
    }

    return TRUE;
}

EXTERN_C BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        if (MH_Initialize() != MH_OK || !DoHook() || !DoEnableHook(TRUE))
            return FALSE;
        break;

    case DLL_PROCESS_DETACH:
        DoEnableHook(FALSE);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}
