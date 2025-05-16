/*
    sample.cpp
*/
#include "archive.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    /*
    CFile file("test.txt", CFile::modeWrite);
    char buffer[] = "CMakeFiles/miniArchive.dir/build.make:90: CMakeFiles/miniArchive.dir/archive.cpp.o";
    file.Write(buffer,sizeof(buffer)-1);
    file.Close();    
    */
    CFile file("test.txt", CFile::modeRead);
    char buffer[1024] = {0};
    std::cout << "File size:" << file.GetLength() << std::endl;
    file.SetPosition(10);
    file.Read(buffer,sizeof(buffer));
    file.Close();    
    std::cout << buffer << std::endl;
    return 0;
}
