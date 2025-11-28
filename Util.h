#pragma once

enum Color
{
    Red,
    Blue,
    White,
    None
};

struct Location // note everything in a struct is public, and inheritance is also public
{
    int row;
    int col;

    // other members
    Location() : row(0), col(0) {};
    Location(int r, int c) : row(r), col(c) {};

    Location &operator+=(const Location &other);
    // {
    //     row += other.row;
    //     col += other.col;
    //     return *this;
    // }
    Location operator+(const Location &other) const;
    // {
    //     return Location(row + other.row, col + other.col);
    // }
    Location &operator-=(const Location &other);
    // {
    //     row -= other.row;
    //     col -= other.col;
    //     return *this;
    // }
    Location operator-(const Location &other) const;
    // {
    //     return Location(row - other.row, col - other.col);
    // }
    bool operator==(const Location &other) const;
    // {
    //     return row == other.row && col == other.col;
    // }
    bool operator!=(const Location &other) const;
    // {
    //     return !(*this == other);
    // }
};

struct Direction
{
    int dRow;
    int dCol;

    static const Direction North;
    static const Direction East;
    static const Direction South;
    static const Direction West;

    Direction() : dRow(-1), dCol(0) {} // default North
    Direction(int dr, int dc) : dRow(dr), dCol(dc) {}
    Direction(const Direction &o) = default;

    Direction RotateLeft90() const;
    Direction RotateRight90() const;
    Direction Rotate180() const;
};