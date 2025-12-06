#pragma once

enum Color
{
    Red,
    Blue,
    White,
    None
};

struct Location // note everything in a struct is public, and inheritance is also public (by default)
{
    int row;
    int col;

    // other members that are needed
    Location() : row(0), col(0) {};
    Location(int r, int c) : row(r), col(c) {};

    Location &operator+=(const Location &other);
    Location operator+(const Location &other) const;
    Location &operator-=(const Location &other);
    Location operator-(const Location &other) const;
    bool operator==(const Location &other) const;
    bool operator!=(const Location &other) const;
};

struct Direction
{
    int dRow;
    int dCol;

    static const Direction North;
    static const Direction East;
    static const Direction South;
    static const Direction West;

    // TODO - documentation (P8) - above pt.4, it says Direction should not have a constructor?
    // should i delete this then?
    Direction() : dRow(-1), dCol(0) {} // default North
    Direction(int dr, int dc) : dRow(dr), dCol(dc) {}
    Direction(const Direction &o) = default;

    Direction RotateLeft90() const;
    Direction RotateRight90() const;
    Direction Rotate180() const;
};
