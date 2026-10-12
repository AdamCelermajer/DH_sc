// CUnzipReadFile keeps the entry name it was opened with; the folder path is the file name.
#include <string>

#include "glitch-core-io/io/unzip_read_file.hpp"

int main() {
    const std::string folder = "/nonexistent/folder/archive";
    glitch::io::CUnzipReadFile file(folder, "entry.bin");
    if (file.getFileName() != "entry.bin") return 1;
    glitch::io::CUnzipReadFile empty(folder, nullptr);
    if (!empty.getFileName().empty()) return 2;
    return 0;
}
