#pragma once

#include <gtk/gtk.h>
#include <string>

namespace FileHandler
{
    #ifdef WIN32
    const COMDLG_FILTERSPEC meshFileTypes[] =
    {
        {L"Mesh file (*.obj, *.fbx)",       L"*.obj;*.fbx"}
    };

    const COMDLG_FILTERSPEC textureFileTypes[] =
    {
        {L"Texture file (*.png, *.jpg, *.jpeg, *.tga)",       L"*.png;*.jpg;*.jpeg;*.tga"}
    };
    #endif

    enum class FileType
    {
        MESH_FILE, TEXTURE_FILE
    };

	static std::string ShowOpenFileDialog(FileType fileType)
	{
        #ifdef UNIX
        GtkWidget *dialog;
        if ( !gtk_init_check( NULL, NULL ) )
        {
            return "";
        }

        dialog = gtk_file_chooser_dialog_new( "Open File",
                                            NULL,
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            "_Cancel", GTK_RESPONSE_CANCEL,
                                            "_Open", GTK_RESPONSE_ACCEPT,
                                            NULL );

        /* Build the filter list */
        //AddFiltersToDialog(dialog, filterList);

        /* Set the default path */
        //SetDefaultPath(dialog, defaultPath);
        char *filename;
        if ( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT )
        {
            filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dialog) );
        }

        while (gtk_events_pending())
            gtk_main_iteration();
        gtk_widget_destroy(dialog);
        while (gtk_events_pending())
            gtk_main_iteration();

        return filename;

        #elif WIN32
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
        #else
        return nullptr;
        #endif
    }

};
