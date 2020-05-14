#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <iostream>
#include <string>

namespace FileHandler
{
	static std::string ShowOpenFileDialog()
	{
        std::string filePath = "";
		IFileDialog* dialog = NULL;
		HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog, (void**)&dialog);
        if (SUCCEEDED(result))
        {
            result = dialog->Show(NULL);
            IShellItem* psiResult;
            result = dialog->GetResult(&psiResult);

            if (SUCCEEDED(result))
            {
                // We are just going to print out the 
                // name of the file for sample sake.
                PWSTR pszFilePath = NULL;
                result = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                    &pszFilePath);
                if (SUCCEEDED(result))
                {
                    std::wstring ws(pszFilePath);
                    filePath = std::string(ws.begin(), ws.end());
                }
                psiResult->Release();
            }
            dialog->Release();
            return filePath;
        }
        /*
            if (SUCCEEDED(result))
            {
                // Obtain the result once the user clicks 
                // the 'Open' button.
                // The result is an IShellItem object.
                IShellItem* psiResult;
                result = dialog->GetResult(&psiResult);
                if (SUCCEEDED(result))
                {
                    // We are just going to print out the 
                    // name of the file for sample sake.
                    PWSTR pszFilePath = NULL;
                    result = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                        &pszFilePath);
                    if (SUCCEEDED(result))
                    {
                        TaskDialog(NULL,
                            NULL,
                            L"CommonFileDialogApp",
                            pszFilePath,
                            NULL,
                            TDCBF_OK_BUTTON,
                            TD_INFORMATION_ICON,
                            NULL);
                        CoTaskMemFree(pszFilePath);
                    }
                    psiResult->Release();
                }
            }
			dialog->Release();
		}*/
	}
};
