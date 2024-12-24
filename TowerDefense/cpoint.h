#pragma once
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class cpoint {
public:
    // Constants and data
    const static int MAP_HEIGHT = 11; // Map matrix size
    const static int MAP_WIDTH = 18; // Map matrix size

    const static int MAP_LEFT = 3;      // Left-coordinates of the map screen
    const static int MAP_TOP = 0;       // Top-coordinates of the map screen

    // Constants for grid calculations
    const static int CELL_WIDTH = 4;
    const static int CELL_HEIGHT = 2;  // Vertical distance between rows
    const static int X_OFFSET = 1;      // Adjustment for X coordinate in grid
    const static int Y_OFFSET = 1;      // Adjustment for Y coordinate in grid

private:
    int x, y, c;     

public:
    cpoint();       
    cpoint(int tx, int ty, int tc);

    int getX() const;
    int getY() const;
    int getC() const;

    void setX(int tx);
    void setY(int ty);
    void setC(int tc);

    cpoint& operator=(const cpoint& other);


    static cpoint fromXYToRowCol(const cpoint& v);
    static cpoint fromRowColToXY(const cpoint& s);

    int countSteps(cpoint other);

    bool operator==(const cpoint& other) const;  // Compare two points
    bool operator!=(const cpoint& other) const;  // Check inequality of two points
};
