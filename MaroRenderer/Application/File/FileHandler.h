#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <iostream>
#include <string>

namespace FileHandler
{
    const COMDLG_FILTERSPEC meshFileTypes[] =
    {
        {L"Mesh file (*.obj, *.fbx)",       L"*.obj;.fbx"}
    };

    const COMDLG_FILTERSPEC textureFileTypes[] =
    {
        {L"Texture file (*.png, *.jpg, *.jpeg)",       L"*.png;*.jpg;*.jpeg"}
    };

    enum class FileType
    {
        MESH_FILE, TEXTURE_FILE
    };

	static std::string ShowOpenFileDialog(FileType fileType)
	{
        std::string filePath = "";
		IFileDialog* dialog = NULL;
		HRESULT result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog, (void**)&dialog);
        if (SUCCEEDED(result))
        {
            DWORD dwFlags;
            result = dialog->GetOptions(&dwFlags);

            if (SUCCEEDED(result))
            {
                if (fileType == FileType::MESH_FILE)
                    result = dialog->SetFileTypes(ARRAYSIZE(meshFileTypes), meshFileTypes);
                else if (fileType == FileType::TEXTURE_FILE)
                    result = dialog->SetFileTypes(ARRAYSIZE(textureFileTypes), textureFileTypes);

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
                }
            }
            dialog->Release();
            return filePath;
        }
	}
};
