//
// Created by izakr on 25/05/2026.
//
//#include "../core/structs.h"
#include <fstream>
#include "file.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <windows.h>
#include <shobjidl.h>
#include <string>

FILE * out;
u32 PPA_MAGIC_VALUE = 0x505045;

void WriteFileType(const char* filename, u32 h, u32 w, const std::vector<Color>& image) {
// Open an empty binary file for writing.
// If the wb mode is specified for a ddname that has DISP=MOD,
// the behavior is the same as if ab had been specified. Otherwise,
// if the file already exists, its contents are destroyed.
    out = fopen(filename, "wb");
    if (out) {
        u32 PixelArraySize = h * w * sizeof(Color);

        ppe_header header = {};
        header.MagicValue = PPA_MAGIC_VALUE;
        header.h = h;
        header.w = w;
        header.PixelsPerLayer = h*w;
        header.PixelOffset = sizeof(header);

        fwrite(&header, sizeof(header) ,1,out);
        fwrite(image.data(), PixelArraySize, 1, out);
        fclose(out);
    }
    else {
        std::cout << "Failed to open file :(\n" << std::endl;
    }
}
std::vector<Color> ReadFileType(const char* filename, u32& out_w, u32& out_h) {
    std::vector<Color> loaded_image;
    FILE* in = fopen(filename, "rb");
    if (!in) {
        std::cout << "Failed to open file for reading :(\n";
        return loaded_image;
    }

    ppe_header header = {};
    if (fread(&header, sizeof(header), 1, in) != 1) {
        fclose(in);
        return loaded_image;
    }

    if (header.MagicValue != PPA_MAGIC_VALUE) {
        std::cout << "Invalid format!\n";
        fclose(in);
        return loaded_image;
    }

    out_w = header.w;
    out_h = header.h;
    u32 total_pixels = out_w * out_h;
    loaded_image.resize(total_pixels);

    fseek(in, header.PixelOffset, SEEK_SET);
    fread(loaded_image.data(), total_pixels * sizeof(Color), 1, in);
    fclose(in);

    return loaded_image;
}


std::string open_file_dialog_windows() {
    std::string file_path = "";

    // Inicjalizacja biblioteki COM (wymagane dla IFileDialog)
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileOpenDialog *pFileOpen;

        // Tworzenie instancji okna dialogowego
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                              IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            // Ustawienie filtra plików (np. tylko pliki .ppe)
            COMDLG_FILTERSPEC fileTypes[] = {
                { L"Pixel Project Files (*.ppe)", L"*.ppe" },
                { L"All Files (*.*)", L"*.*" }
            };
            pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

            // Wyświetlenie okna (kod blokuje się tutaj, dopóki użytkownik nie kliknie OK/Anuluj)
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr)) {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr)) {
                        // Konwersja z Wide Char (Windows) na standardowy std::string
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                        file_path.resize(size_needed - 1);
                        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &file_path[0], size_needed, NULL, NULL);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return file_path;
}
