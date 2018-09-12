#pragma once
 
#include <Windows.h>;
#include <windowsx.h>;
#include <CommCtrl.h>;
#include "resource.h";
#include <stdlib.h> 
#include <string.h>
#include <tchar.h> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <locale>
#include <codecvt>

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CSSComboProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

class Dialog
{
public:
	void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
	{
		switch (id)
		{
		case IDC_BUTTON1:
			EndDialog(hwnd,0);
			break;
		default:
			break;
		}
	}
};

