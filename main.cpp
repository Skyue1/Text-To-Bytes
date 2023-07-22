#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

std::vector<unsigned char> ReadFileBytes(const std::wstring& filePath);
std::vector<size_t> FindTextPositions(const std::vector<unsigned char>& fileBytes, const std::wstring& searchText);
void PrintTextBytes(const std::wstring& filePath, const std::vector<unsigned char>& fileBytes, const std::vector<size_t>& textPositions, const std::wstring& searchText);
void PrintBaseAddressAndOffset(const std::vector<unsigned char>& fileBytes, size_t position);

int main() {
    std::wcout << L"Enter the path to the file to search for text: ";
    std::wstring filePath;
    std::getline(std::wcin, filePath);

    std::wcout << L"Enter the text to be found in the file: ";
    std::wstring searchText;
    std::getline(std::wcin, searchText);

    std::vector<unsigned char> fileBytes = ReadFileBytes(filePath);
    if (fileBytes.empty()) {
        std::wcout << L"Failed to read file: " << filePath << std::endl;
        std::wcout << L"\nPress any button to exit the program" << std::endl;
        std::wcin.get();
        return 1;
    }

    std::vector<size_t> textPositions = FindTextPositions(fileBytes, searchText);
    if (textPositions.empty()) {
        std::wcout << L"Text \"" << searchText << L"\" not found in file: " << filePath << std::endl;
    }
    else {
        PrintTextBytes(filePath, fileBytes, textPositions, searchText);
    }

    std::wcout << L"\nPress any button to exit the program" << std::endl;
    std::wcin.get();

    return 0;
}

std::vector<unsigned char> ReadFileBytes(const std::wstring& filePath) {
    std::vector<unsigned char> fileBytes;
    std::ifstream file(filePath, std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        fileBytes.resize(fileSize);
        file.read(reinterpret_cast<char*>(fileBytes.data()), fileSize);

        file.close();
    }
    return fileBytes;
}

std::vector<size_t> FindTextPositions(const std::vector<unsigned char>& fileBytes, const std::wstring& searchText) {
    std::vector<size_t> positions;
    size_t foundPos = 0;

    while (foundPos < fileBytes.size()) {
        foundPos = std::search(fileBytes.begin() + foundPos, fileBytes.end(), searchText.begin(), searchText.end()) - fileBytes.begin();

        if (foundPos != fileBytes.size()) {
            positions.push_back(foundPos);
            foundPos += searchText.size();
        }
    }

    return positions;
}

void PrintBaseAddressAndOffset(const std::vector<unsigned char>& fileBytes, size_t position)
{
    const unsigned char* baseAddress = fileBytes.data();

    size_t offset = position;

    uintptr_t address = reinterpret_cast<uintptr_t>(baseAddress) + offset;

    std::wcout << L"address: 0x"<< std::hex << address << L", base address: 0x" << reinterpret_cast<uintptr_t>(baseAddress) << L", offset: " << offset << std::endl;
}

void PrintTextBytes(const std::wstring& filePath, const std::vector<unsigned char>& fileBytes, const std::vector<size_t>& textPositions, const std::wstring& searchText) {
    std::wcout << L"Text \"" << searchText << L"\" found in file: " << filePath << L"\n" << std::endl;

    for (size_t pos : textPositions) {
        PrintBaseAddressAndOffset(fileBytes, pos);

        for (size_t i = pos; i < pos + searchText.size(); ++i) {
            std::wcout << std::hex << static_cast<int>(fileBytes[i]) << L" ";
        }
        std::wcout << std::endl;
        std::wcout << L"\nPress any button to exit the program" << std::endl;
        std::wcin.get();
    }
}
