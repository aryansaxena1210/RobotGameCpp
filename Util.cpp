#include "Util.h"

Location &Location::operator+=(const Location &other)
{
    row += other.row;
    col += other.col;
    return *this;
}
Location Location::operator+(const Location &other) const
{
    return Location(row + other.row, col + other.col);
}
Location &Location::operator-=(const Location &other)
{
    row -= other.row;
    col -= other.col;
    return *this;
}
Location Location::operator-(const Location &other) const
{
    return Location(row - other.row, col - other.col);
}
bool Location::operator==(const Location &other) const
{
    return row == other.row && col == other.col;
}
bool Location::operator!=(const Location &other) const
{
    return !(*this == other);
}
Location operator+(const Location& lhs, const Direction& rhs)
{
    return Location(lhs.row + rhs.dRow, lhs.col + rhs.dCol);
}
const Direction Direction::North = Direction(-1, 0);
const Direction Direction::East = Direction(0, 1);
const Direction Direction::South = Direction(1, 0);
const Direction Direction::West = Direction(0, -1);

Direction Direction::RotateLeft90() const
{
    // North -> West, East -> North, South -> East, West -> South
    // North -> West -> South -> East
    return Direction(-dCol, dRow);
}

Direction Direction::RotateRight90() const
{
    // North -> East, East -> South, South -> West, West -> North
    return Direction(dCol, -dRow);
}

Direction Direction::Rotate180() const
{
    return Direction(-dRow, -dCol);
}


//left rotate == (-dCol, dRow)
//right rotate == (dCol, -dRow)