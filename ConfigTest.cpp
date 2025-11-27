#include "Config.h"
#include <iostream>
using namespace std;

void testDefault()
{
    Config c;
    cout << "Checking default constructor: PASS\n";
}

void testBadFile()
{
    try
    {
        Config c("no_such_file.txt");
        cout << "Checking bad file name: FAIL\n";
    }
    catch (...)
    {
        cout << "Checking bad file name: PASS\n";
    }
}

int main()
{
    cout << "Config class unit tests\n\n";
    testDefault();
    testBadFile();
    return 0;
}
