#include "libxl.h"
#include <iostream>
#include <string>
#include <filesystem>

int main() {
    // Create a new .xlsx workbook
    libxl::Book* book = xlCreateXMLBook();
    if (!book) {
        std::cout << "Failed to create book." << std::endl;
        return 1;
    }

    // Add a new sheet and write data
    libxl::Sheet* sheet = book->addSheet("Sheet1");
    if (!sheet) {
        std::cout << "Failed to add sheet." << std::endl;
        book->release();
        return 1;
    }
    sheet->writeStr(0, 0, "Name");
    sheet->writeStr(0, 1, "Score");
    sheet->writeStr(1, 0, "Rishav");
    sheet->writeNum(1, 1, 99);

    // Save the file
    std::string main_filepath = std::filesystem::current_path().string() + "/"; // Get the current working directory
    std::string name = "op"; // Name of the file without extension
    std::string filepath = main_filepath+ name + ".xlsx";
    if (book->save(filepath.c_str())) {
        std::cout << "File saved as " << filepath << std::endl;
    } else {
        std::cout << "Failed to save file: " << book->errorMessage() << std::endl;
        book->release();
        return 1;
    }
    book->release();

    // Now, read the file back
    libxl::Book* readBook = xlCreateXMLBook();
    if (readBook && readBook->load(filepath.c_str())) {
        libxl::Sheet* readSheet = readBook->getSheet(0);
        if (readSheet) {
            const char* name = readSheet->readStr(1, 0);
            double score = readSheet->readNum(1, 1);
            std::cout << "Read from file: " << (name ? name : "null") << ", " << score << std::endl;
        }
        readBook->release();
    } else {
        std::cout << "Failed to load file for reading." << std::endl;
    }

    return 0;
}