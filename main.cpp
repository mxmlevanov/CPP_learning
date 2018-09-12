#include "main.h";
#include "resource.h";

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
}


INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	wstring line;
	wifstream file("movie_list_uni.txt", std::ios::binary);
	HWND CSSCombo = GetDlgItem(hwnd, IDC_COMBO1);
	UINT_PTR uIdSubclass = 0;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		file.imbue(locale(file.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, consume_header>));
		if (!file) {
			cout << "unable to open file";
			return false;
		}
		while (getline(file, line))
		{
			SendMessage(CSSCombo, CB_ADDSTRING, 0, (LPARAM)line.c_str());
		}
		// subclass ComboBox
		if (!SetWindowSubclass(CSSCombo, CSSComboProc, uIdSubclass, 0))
		{
			break;
		}
		RECT rc;
		if (GetClientRect(CSSCombo, &rc))
			InvalidateRect(CSSCombo, &rc, TRUE);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			EndDialog(hwnd, NULL);
			return TRUE;
			break;
		}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
		break;
	}
	return FALSE;
}

// check if a string include substring
bool IsInclude(vector<wchar_t>& row, vector<wchar_t> input)
{
	vector <wchar_t>::iterator it = search(row.begin(), row.end(), input.begin(), input.end() - 1);
	if (it == row.end())
	{
		return false;
	}
	return true;
}
LRESULT CALLBACK CSSComboProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_COMBO1:
			{
				if (HIWORD(wParam) == CBN_EDITUPDATE)
				{
					HWND CSSCombo = GetDlgItem(hwnd, IDC_COMBO1);
					vector<wchar_t> text_in;
					vector<vector<wchar_t>> c_list;
					vector<vector<wchar_t>>::iterator c_list_it;

					// get user input					
					int n_text_in = SendMessage(CSSCombo, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
					text_in.resize(n_text_in + 1);
					SendMessage(CSSCombo, WM_GETTEXT, n_text_in + 1, (LPARAM)&text_in[0]);
					if (n_text_in > 0)
					{
						// store listbox content into vector
						int n_list_row = SendMessage(CSSCombo, CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
						for (int i = 0; i < n_list_row; i++)
						{
							int n = SendMessage(CSSCombo, CB_GETLBTEXTLEN, (WPARAM)i, (LPARAM)0);
							if (n != CB_ERR)
							{
								vector<wchar_t> list_row(n + 1);
								SendMessage(CSSCombo, CB_GETLBTEXT, (WPARAM)i, (LPARAM)&list_row[0]);
								c_list.push_back(list_row);
							}
						}
						// clear listbox
						SendMessage(CSSCombo, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
						SendMessage(CSSCombo, WM_SETTEXT, 0, (LPARAM)&text_in[0]);
						// process vector and reload vector to listbox
						for (c_list_it = c_list.begin(); c_list_it != c_list.end();)
						{
							vector<wchar_t> temp = *c_list_it;
							if (!IsInclude(*c_list_it, text_in))
							{
								//									c_list_it = c_list.erase(c_list_it);	
								//									 SendMessage(CSSCombo, CB_DELETESTRING, 0, (LPARAM)&temp[0]);
								++c_list_it;
							}
							else
							{

								SendMessage(CSSCombo, CB_ADDSTRING, 0, (LPARAM)&temp[0]);
								++c_list_it;
							}
						}
					}
					SendMessage(CSSCombo, CB_SHOWDROPDOWN, (WPARAM)true, (LPARAM)0);
					//						SendMessage(CSSCombo, CB_SETTOPINDEX, (WPARAM)0, (LPARAM)0);
					SetCursor(LoadCursor(NULL, IDC_ARROW));
					//						SendMessage(CSSCombo, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				}
			}
			default:
				break;
			}
		}
		default:
			return DefSubclassProc(hwnd, uMsg, wParam, lParam);
			break;
	}
}