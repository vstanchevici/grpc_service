#include <version.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <MemoryBlock.h>

namespace fs = std::filesystem;

void test_filesystem()
{
    fs::path p1 = "/usr/lib/sendmail.cf"; // portable format
    fs::path p2 = "C:\\users\\abcdef\\AppData\\Local\\Temp\\"; // native format

    fs::path p4 = "var";
    p4 = p4 / "car" / "smar";


    std::cout << "p1 = " << p1 << '\n'
        << "p2 = " << p2 << '\n'
        << "p4 = " << p4 << '\n';

    std::cout << "pn = " << p1.string().c_str() << '\n';
}


int main_test(int argc, char* argv[])
{
    std::cout << "!Project Name: " << GRPC_SERVICE_PROJECT_DESCRIPTION << std::endl << std::endl;

    // Create a vector object and add a few elements to it.
    std::vector<MemoryBlock> v;
    v.push_back(MemoryBlock(25));
    v.push_back(MemoryBlock(75));

    // Insert a new element into the second position of the vector.
    v.insert(v.begin() + 1, MemoryBlock(50));

    return 0;
}
