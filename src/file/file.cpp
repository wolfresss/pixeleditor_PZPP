//
// Created by izakr on 25/05/2026.
//
#include "file.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>


#include <windows.h>
#include <shobjidl.h>

const uint32_t PPA_MAGIC_VALUE = 0x505045;

void SaveDocumentPPE(const char* filename, const Document& doc) {
    std::ofstream out(filename, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }


    ppe_header header = {};
    header.MagicValue = PPA_MAGIC_VALUE;
    header.w = doc.width;
    header.h = doc.height;
    header.LayerCount = static_cast<uint32_t>(doc.layers.size());
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));


    for (const auto& layer : doc.layers) {
        ppe_layer_header layer_hdr = {};
        layer_hdr.Id = layer.Id;
        layer_hdr.visible = layer.visible ? 1 : 0;
        layer_hdr.name_length = static_cast<uint32_t>(layer.name.size());


        out.write(reinterpret_cast<const char*>(&layer_hdr), sizeof(layer_hdr));


        out.write(layer.name.data(), layer_hdr.name_length);


        uint32_t pixels_bytes = static_cast<uint32_t>(layer.pixels.size() * sizeof(Color));
        out.write(reinterpret_cast<const char*>(layer.pixels.data()), pixels_bytes);
    }

    out.close();
    std::cout << "[SUCCESS] Saved project with " << doc.layers.size() << " layers.\n";
}

Document* LoadDocumentPPE(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return nullptr;
    }


    ppe_header header = {};
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.MagicValue != PPA_MAGIC_VALUE) {
        std::cerr << "Invalid format magic value!\n";
        in.close();
        return nullptr;
    }


    std::vector<Color> dummy_pixels(header.w * header.h, Color{0,0,0,0});
    Document* doc = new Document(header.w, header.h, "TemporaryBase", dummy_pixels);
    doc->layers.clear();


    for (uint32_t i = 0; i < header.LayerCount; ++i) {
        ppe_layer_header layer_hdr = {};
        in.read(reinterpret_cast<char*>(&layer_hdr), sizeof(layer_hdr));


        std::string layer_name(layer_hdr.name_length, '\0');
        in.read(&layer_name[0], layer_hdr.name_length);


        std::vector<Color> layer_pixels(header.w * header.h);
        in.read(reinterpret_cast<char*>(layer_pixels.data()), header.w * header.h * sizeof(Color));


        doc->addLayer(layer_name, header.w, header.h, layer_pixels);
        doc->layers.back().Id = layer_hdr.Id;
        doc->layers.back().visible = (layer_hdr.visible == 1);
    }

    doc->activeLayerIndex = 0;
    in.close();

    std::cout << "[SUCCESS] Loaded project: " << header.w << "x" << header.h << " with " << header.LayerCount << " layers.\n";
    return doc;
}


std::string open_file_dialog_windows() {
    std::string file_path = "";


    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileOpenDialog *pFileOpen;


        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                              IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC fileTypes[] = {
                { L"Pixel Project Files (*.ppe)", L"*.ppe" },
                { L"All Files (*.*)", L"*.*" }
            };
            pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr)) {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr)) {
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

std::string save_file_dialog_windows() {
    std::string file_path = "";


    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileSaveDialog *pFileSave;


        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                              IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC fileTypes[] = {
                { L"Pixel Project Files (*.ppe)", L"*.ppe" },
                { L"All Files (*.*)", L"*.*" }
            };
            pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

            pFileSave->SetDefaultExtension(L"ppe");

            hr = pFileSave->Show(NULL);

            if (SUCCEEDED(hr)) {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr)) {
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                        file_path.resize(size_needed - 1);
                        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &file_path[0], size_needed, NULL, NULL);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileSave->Release();
        }
        CoUninitialize();
    }
    return file_path;
}