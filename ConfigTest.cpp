#include "Config.h"
#include <iostream>
#include <fstream>
using namespace std;

int testsPassed = 0;
int testsFailed = 0;

void report(bool condition, const string &testName)
{
    if (condition)
    {
        cout << testName << ": PASS\n";
        testsPassed++;
    }
    else
    {
        cout << testName << ": FAIL\n";
        testsFailed++;
    }
}

/*
 Helper function to create temporary config files.
*/
void writeFile(const string &name, const string &content)
{
    ofstream out(name);
    out << content;
    out.close();
}

// --------------------------------------
// TESTS
// --------------------------------------

void testDefaultConstructor()
{
    Config c;
    report(c.getHitDuration() == 20, "Default HIT_DURATION");
    report(c.getPaintBlobLimit() == 30, "Default PAINTBLOB_LIMIT");
}

void testBadFilename()
{
    try
    {
        Config c("does_not_exist.txt");
        report(false, "Bad filename should throw");
    }
    catch (...)
    {
        report(true, "Bad filename throws correctly");
    }
}

void testValidAllParams()
{
    writeFile("config1.txt",
              "HIT_DURATION = 10\n"
              "PAINTBLOB_LIMIT = 40\n"
              "ROCK_LOWER_BOUND = 2\n"
              "ROCK_UPPER_BOUND = 9\n"
              "FOG_LOWER_BOUND = 3\n"
              "FOG_UPPER_BOUND = 7\n"
              "LONG_RANGE_LIMIT = 15\n");

    Config c("config1.txt");
    bool ok =
        c.getHitDuration() == 10 &&
        c.getPaintBlobLimit() == 40 &&
        c.getRockLowerBound() == 2 &&
        c.getRockUpperBound() == 9 &&
        c.getFogLowerBound() == 3 &&
        c.getFogUpperBound() == 7 &&
        c.getLongRangeLimit() == 15;

    report(ok, "Valid config w/ all params");
}

void testMissingParams()
{
    writeFile("config2.txt",
              "HIT_DURATION = 8\n"
              "# missing rest\n");

    Config c("config2.txt"); // should use defaults for missing ones

    bool ok =
        c.getHitDuration() == 8 &&
        c.getPaintBlobLimit() == 30 && // default
        c.getRockLowerBound() == 10 && // default
        c.getRockUpperBound() == 20 && // default
        c.getFogLowerBound() == 5 &&   // default
        c.getFogUpperBound() == 10 &&  // default
        c.getLongRangeLimit() == 30;   // default

    report(ok, "Missing params fall back to defaults");
}

void testCaseInsensitivity()
{
    writeFile("config3.txt",
              "hit_duration = 12\n"
              "paintblob_Limit = 33\n");

    Config c("config3.txt");
    bool ok =
        c.getHitDuration() == 12 &&
        c.getPaintBlobLimit() == 33;

    report(ok, "Case insensitivity");
}

void testWhitespaceIgnored()
{
    writeFile("config4.txt",
              "   HIT_DURATION     =    25   \n"
              "\n");

    Config c("config4.txt");
    report(c.getHitDuration() == 25, "Whitespace ignored");
}

void testCommentsIgnored()
{
    writeFile("config5.txt",
              "# comment\n"
              "HIT_DURATION=9\n"
              "# another comment\n");

    Config c("config5.txt");
    report(c.getHitDuration() == 9, "Comments ignored");
}

void testInvalidLineMissingEquals()
{
    writeFile("config6.txt",
              "HIT_DURATION 20\n" // invalid
    );

    try
    {
        Config c("config6.txt");
        report(false, "Missing '=' should throw");
    }
    catch (...)
    {
        report(true, "Invalid missing '=' throws");
    }
}

void testInvalidUnknownKeyword()
{
    writeFile("config7.txt",
              "HIT_DURATION = 17\n"
              "BAD_PARAMETER = 99\n");

    try
    {
        Config c("config7.txt");
        report(false, "Unknown keyword should throw");
    }
    catch (...)
    {
        report(true, "Unknown keyword throws");
    }
}

void testNonIntegerValue()
{
    writeFile("config8.txt",
              "HIT_DURATION = abc\n");

    try
    {
        Config c("config8.txt");
        report(false, "Non-integer value should throw");
    }
    catch (...)
    {
        report(true, "Non-integer throws");
    }
}

// --------------------------------------
// MAIN
// --------------------------------------
int main()
{
    cout << "Config class unit tests\n\n";

    testDefaultConstructor();
    testBadFilename();
    testValidAllParams();
    testMissingParams();
    testCaseInsensitivity();
    testWhitespaceIgnored();
    testCommentsIgnored();
    testInvalidLineMissingEquals();
    testInvalidUnknownKeyword();
    testNonIntegerValue();

    cout << "\nTests Passed: " << testsPassed << endl;
    cout << "Tests Failed: " << testsFailed << endl;

    return 0;
}
