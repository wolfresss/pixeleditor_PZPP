#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <sstream>

using namespace std;

//NOTE(Iza): warning - make sure you have excluded this source file from cmakelist otherwise you will get error for
//two main fucntion defined in seperate files : list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Resources/TEMPORARY_tool.cpp")
//If pixel_editor.exe still fails, remove build folder and remove cashe from cmake

//This exist purely to automate inserting new icons, it should add new enums and new bit array to the binary_icons.h file
//as well as update  PBM_SourceMeta generated_icons_meta[] array that is needed for init.

string get_base_name(const string& filename) {
    size_t last_slash = filename.find_last_of("/\\");
    string base = (last_slash == string::npos) ? filename : filename.substr(last_slash + 1);
    size_t last_dot = base.find_last_of(".");
    if (last_dot != string::npos) {
        base = base.substr(0, last_dot);
    }
    return base;
}

string get_file_with_extension(const string& filename) {
    size_t last_slash = filename.find_last_of("/\\");
    return (last_slash == string::npos) ? filename : filename.substr(last_slash + 1);
}

string to_uppercase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

int main(int argc, char* argv[])
{
    string input_filename;

    if (argc > 1) {
        input_filename = argv[1];
    }
    else {
        cout << "Enter the FULL path to the PBM file (or drag & drop it here): ";
        getline(cin, input_filename);
    }

    input_filename.erase(remove(input_filename.begin(), input_filename.end(), '\"'), input_filename.end());

    ifstream in(input_filename, ios::in | ios::binary);
    if (!in.is_open()) {
        cerr << "Error: Could not open file at path: " << input_filename << endl;
        return 1;
    }

    string format;
    int width = 0, height = 0;
    in >> format >> width >> height;
    in.get();

    if (format != "P4") {
        cerr << "Error: Format must be P4 (binary PBM). Detected: " << format << endl;
        return 1;
    }

    if (width > 64) {
        cerr << "Error: Image width (" << width << " px) exceeds the 64-bit limit!" << endl;
        return 1;
    }

    string clean_name = get_base_name(input_filename);
    string file_with_ext = get_file_with_extension(input_filename);
    string enum_name = "ICON_PBM_" + to_uppercase(clean_name);

    string output_filename = "binary_icons.h";

    ifstream old_file(output_filename);
    string file_content = "";
    if (old_file.is_open()) {
        stringstream buffer;
        buffer << old_file.rdbuf();
        file_content = buffer.str();
        old_file.close();
    } else {
        cerr << "Error: Could not find or open " << output_filename << " to inject enum!" << endl;
        return 1;
    }

    if (file_content.find(enum_name + ",") != string::npos) {
        cout << "[WARNING] Enum " << enum_name << " already exists in file. Skipping enum injection.\n";
    } else {
        size_t enum_pos = file_content.find("ICON_PBM_TOTAL_COUNT");
        if (enum_pos != string::npos) {
            string injection = enum_name + ",\n    ";
            file_content.insert(enum_pos, injection);
        } else {
            cerr << "Error: Could not find 'ICON_PBM_TOTAL_COUNT' inside " << output_filename << "!" << endl;
            return 1;
        }
    }

    stringstream new_array;
    new_array << "// Automatically generated from file: " << file_with_ext << "\n";
    new_array << "// Dimensions: " << width << "x" << height << "\n";
    new_array << "inline constexpr uint64_t " << clean_name << "_icon_bits[" << height << "] = {\n";

    int bytes_per_line = (width + 7) / 8;

    for (int y = 0; y < height; ++y) {
        vector<char> row_bytes(bytes_per_line);
        in.read(row_bytes.data(), bytes_per_line);

        uint64_t row_mask = 0;

        for (int x = 0; x < width; ++x) {
            int byte_idx = x / 8;
            int bit_idx = 7 - (x % 8);

            bool is_pbm_black = (row_bytes[byte_idx] >> bit_idx) & 1;
            bool active_pixel = !is_pbm_black;

            if (active_pixel) {
                row_mask |= (1ULL << (63 - x));
            }
        }

        new_array << "    0x" << hex << setw(16) << setfill('0') << row_mask;
        if (y < height - 1) new_array << ",";
        new_array << " // Row " << dec << y << "\n";
    }
    new_array << "};\n\n";


    size_t data_section_pos = file_content.find("// SEKCJA DANYCH BINARNYCH (TABLICE BITOWE)");
    if (data_section_pos != string::npos) {

        size_t insert_pos = file_content.find("\n", data_section_pos);
        if (insert_pos != string::npos) {
            file_content.insert(insert_pos + 1, new_array.str());
        }
    } else {
        cerr << "Error: Could not find data section marker in " << output_filename << "!" << endl;
        return 1;
    }

    if (file_content.find(enum_name + ", " + clean_name + "_icon_bits") != string::npos) {
        cout << "[WARNING] Icon " << clean_name << " already registered in metadata. Skipping.\n";
    } else {
        size_t meta_list_pos = file_content.find("}; // END_OF_REGISTRATION_LIST");
        if (meta_list_pos != string::npos) {
            stringstream meta_entry;
            meta_entry << "    { " << enum_name << ", " << clean_name << "_icon_bits, " << width << ", " << height << " },\n";
            file_content.insert(meta_list_pos, meta_entry.str());
        } else {
            cerr << "Error: Could not find 'END_OF_REGISTRATION_LIST' marker in " << output_filename << "!" << endl;
            return 1;
        }
    }

    ofstream out_file(output_filename, ios::out | ios::trunc);
    if (!out_file.is_open()) {
        cerr << "Error: Could not save modifications to " << output_filename << endl;
        return 1;
    }
    out_file << file_content;

    in.close();
    out_file.close();

    cout << "\n[SUCCESS] Injected " << enum_name << " into enum list!\n";
    cout << "[SUCCESS] Injected matrix data into Binary Section!\n";
    cout << "[SUCCESS] Registered " << clean_name << " in automatic metadata list!\n";

    return 0;
}