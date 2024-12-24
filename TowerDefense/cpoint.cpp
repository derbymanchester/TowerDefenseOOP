#include "cpoint.h"

cpoint::cpoint() : x(0), y(0), c(0) {}
cpoint::cpoint(int tx, int ty, int tc) : x(tx), y(ty), c(tc) {}

int cpoint::getX() const { return x; }
int cpoint::getY() const { return y; }
int cpoint::getC() const { return c; }

void cpoint::setX(int tx) { x = tx; }
void cpoint::setY(int ty) { y = ty; }
void cpoint::setC(int tc) { c = tc; }

cpoint& cpoint::operator=(const cpoint& other) 
{
    if (this != &other) {
        x = other.x;
        y = other.y;
        c = other.c;
    }
    return *this;
}


// Convert screen coordinates (x, y) to grid indices (row, col)
cpoint cpoint::fromXYToRowCol(const cpoint& xyPoint) {
    return {
        (xyPoint.y - Y_OFFSET - MAP_TOP) / CELL_HEIGHT,
        (xyPoint.x - X_OFFSET - MAP_LEFT) / CELL_WIDTH,
        xyPoint.c
    };
}

// Convert grid indices (row, col) back to screen coordinates (x, y)
cpoint cpoint::fromRowColToXY(const cpoint& gridPoint) {
    return {
        CELL_WIDTH * gridPoint.y + X_OFFSET + MAP_LEFT,
        CELL_HEIGHT * gridPoint.x + Y_OFFSET + MAP_TOP,
        gridPoint.c
    };
}

int cpoint::countSteps(cpoint other) 
{
    if (this->y == other.y) 
    {
        return abs(this->x - other.x);
    }

    if (this->x == other.x) 
    {
        return abs(this->y - other.y);
    }

    if (abs(this->x - other.x) == abs(this->y - other.y)) 
    {
        return max(abs(this->x - other.x), abs(this->y - other.y));
    }


    return -1;
}


bool cpoint::operator==(const cpoint& other) const {
    return x == other.x && y == other.y;
}

bool cpoint::operator!=(const cpoint& other) const {
    return !(*this == other);
}
