#pragma once
#include <string>
#include <stdexcept>
using std::string;

class Config
{
private:
    int hitDuration;
    int paintBlobLimit;
    int rockLower;
    int rockUpper;
    int fogLower;
    int fogUpper;
    int longRangeLimit;

    void setDefaults();
    void parseLine(const string &line);

public:
    Config();
    Config(const string &filename);

    int getHitDuration() const;
    int getPaintBlobLimit() const;
    int getRockLowerBound() const;
    int getRockUpperBound() const;
    int getFogLowerBound() const;
    int getFogUpperBound() const;
    int getLongRangeLimit() const;
};
