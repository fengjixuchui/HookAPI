typedef int (WINAPI *FN_MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);

static FN_MessageBoxW fn_MessageBoxW = NULL;

INT WINAPI
DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    return (*fn_MessageBoxW)(hWnd, L"Hooked!", lpCaption, uType);
}

BOOL DoHook(BOOL bHook)
{
    if (bHook)
    {
        return (MH_CreateHookCast(&MessageBoxW, &DetourMessageBoxW, &fn_MessageBoxW) == MH_OK);
    }
    else
    {
        return (MH_RemoveHookCast(&MessageBoxW) == MH_OK);
    }
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
