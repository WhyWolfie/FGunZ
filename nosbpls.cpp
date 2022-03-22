#include "StdAfx.h"
#include "nosbpls.h"

HHOOK MouseHook;
HHOOK KeyboardHook;

std::list<MTD_KeyParams*> keyList;

template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if(wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONDOWN || wParam == WM_MBUTTONDOWN || wParam == WM_XBUTTONDOWN) //Added middle mouse and the side buttons. 
		{
			MSLLHOOKSTRUCT *info=(MSLLHOOKSTRUCT*)lParam;     

			if((info->flags & LLMHF_INJECTED) == LLMHF_INJECTED)
			{
				ZPostBanMe(WHT_MOUSE);
				return 1;
				//ExitProcess(-1); //Can be converted into anything, hack detected, or whatnot. 
			}
		}
	}
	return CallNextHookEx(MouseHook,nCode,wParam,lParam);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			KBDLLHOOKSTRUCT *info=(KBDLLHOOKSTRUCT*)lParam;

			// ignore system reserved keys <=0x00, >=0xE0
			// http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
			// http://referencesource.microsoft.com/#WindowsBase/src/Base/System/Windows/Input/Key.cs#37907789825923bd
			// oem specific (0xE9), /*lmenu, rmenu,*/ lwin, rwin, undefined, back, enter, browser back to reserved, caps lock, scroll lock, escape, 
			// packet[unicode], clear key, reserved-unassigned, ime stuff-undefined, ime stuff
			if (info->vkCode <= 0x00 || info->vkCode >= 0xE0 || 
				info->vkCode == 0xE9 || /*info->vkCode == 0xA4 || info->vkCode == 0xA5 || */
				info->vkCode == 0x5B || info->vkCode == 0x5C || info->vkCode == 0x07 || info->vkCode == 0x08 ||
				info->vkCode == 0x09 || info->vkCode == 0x0D || (info->vkCode >= 0xA6 && info->vkCode <= 0xB9) ||
				info->vkCode == 0x14 || info->vkCode == 0x91 || info->vkCode == 0x1B || info->vkCode == 0xE7 ||
				info->vkCode == 0x0C || (info->vkCode >= 0xC1 && info->vkCode <= 0xDA) || 
				(info->vkCode >= VK_KANA && info->vkCode <= 0x1A) || (info->vkCode >= 0x1C && info->vkCode <= 0x1F))
			{
				return CallNextHookEx(KeyboardHook,nCode,wParam,lParam);
			}

			if (keyList.size() == 5)
			{
				MTD_KeyParams* frontParam = keyList.front();
				delete frontParam;
				keyList.pop_front();
			}

			MTD_KeyParams* params = new MTD_KeyParams();
			params->vkCode = info->vkCode;
			params->scanCode = info->scanCode;
			params->flags = info->flags;
			params->time = info->time;
			params->extraInfo = info->dwExtraInfo;
			keyList.push_back(params);

			if ((info->flags & LLKHF_INJECTED) == LLKHF_INJECTED && !((info->flags & LLKHF_EXTENDED) == LLKHF_EXTENDED) && info->dwExtraInfo == 0)
			{
				ZPostBanMe(WHT_KEYBOARD, keyList);
				return 1;
			}
		}
	}
	return CallNextHookEx(KeyboardHook,nCode,wParam,lParam);
}


void AntiShotbotLogger()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
	//This can't really be touched but to patch a sig search I made a useless variable.
    MouseHook = SetWindowsHookEx( WH_MOUSE_LL, MouseHookProc, hInstance, NULL );
	KeyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardHookProc, hInstance, NULL );

	int sigsearch = 0;
    MSG message;
	//sigsearch++; // don't use this, bugs the entire thing
    while (GetMessage(&message,NULL,sigsearch,sigsearch) && ( ZGetApplication()->m_RunThread )) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

	UnhookWindowsHookEx(KeyboardHook);
    UnhookWindowsHookEx(MouseHook);
}