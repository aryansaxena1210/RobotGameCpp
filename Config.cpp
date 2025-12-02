#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void Config::setDefaults()
{
    hitDuration = 20;
    paintBlobLimit = 30;
    rockLower = 10;
    rockUpper = 20;
    fogLower = 5;
    fogUpper = 10;
    longRangeLimit = 30;
}

Config::Config()
{
    setDefaults();
}

static bool isComment(const string &s)
{
    return s.size() > 0 && s[0] == '#';
}

void Config::parseLine(const string &line)
{
    if (line.find('=') == string::npos)
        throw std::runtime_error("Invalid config line: missing '='");

    string left = line.substr(0, line.find('='));
    string right = line.substr(line.find('=') + 1);

    auto trim = [&](string &x) // making trim a lambda cuz we dont need it to be in global scope, very unecessary optimization but why not?
    {
        x.erase(remove_if(x.begin(), x.end(), ::isspace), x.end());
        transform(x.begin(), x.end(), x.begin(), ::toupper);
    };

    trim(left);
    trim(right);

    int value = stoi(right);

    if (left == "HIT_DURATION")
        hitDuration = value;
    else if (left == "PAINTBLOB_LIMIT")
        paintBlobLimit = value;
    else if (left == "ROCK_LOWER_BOUND")
        rockLower = value;
    else if (left == "ROCK_UPPER_BOUND")
        rockUpper = value;
    else if (left == "FOG_LOWER_BOUND")
        fogLower = value;
    else if (left == "FOG_UPPER_BOUND")
        fogUpper = value;
    else if (left == "LONG_RANGE_LIMIT")
        longRangeLimit = value;
    else
        throw std::runtime_error("Unknown config parameter: " + left);
}

Config::Config(const string &filename)
{
    setDefaults();

    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    string line;
    while (std::getline(file, line))
    {
        if (line.empty() || isComment(line))
            continue;
        parseLine(line);
    }
}

int Config::getHitDuration() const { return hitDuration; }
int Config::getPaintBlobLimit() const { return paintBlobLimit; }
int Config::getRockLowerBound() const { return rockLower; }
int Config::getRockUpperBound() const { return rockUpper; }
int Config::getFogLowerBound() const { return fogLower; }
int Config::getFogUpperBound() const { return fogUpper; }
int Config::getLongRangeLimit() const { return longRangeLimit; }
