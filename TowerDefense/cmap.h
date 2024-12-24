#pragma once
#include "ctool.h"
#include "cenemy.h"
#include "ctower.h"
#include "cbullet.h"
#include <algorithm>
#include <chrono>

class cmap
{
private:
    vector<cenemy> _ce; // Danh sách kẻ thù
    vector<ctower> _ctw;    // Danh sách trụ
    vector<cbullet> _cb;    // Danh sách đạn

    int _height;    // Chiều cao
    int _width;     // Chiều rộng
    vector<vector<cpoint>> _m;      // Map để vẽ
    int _level;     // Lvel của map

public:
    cmap(int);
    void resetMapData();    // Khởi tạo giá trị từng ô trong map
    void makeMapData();
    void drawMap();     // Vẽ map

    int getHeight() const;
    int getWidth() const;
    int getLevel() const;

    void setHeight(int height);
    void setWidth(int width);
    void setLevel(int level);

    void addEnemy(const cenemy& enemy);
    void addTower(const ctower& tower);
    void addBullet(const cbullet& bullet);

    vector<cenemy>& getEnemies();
    vector<ctower>& getTowers();
    vector<cbullet>& getBullets();

    // Tạo đường đi cho đạn dựa trên trụ và danh sách kẻ thù
    vector<cpoint> createBulletPath(const ctower& tower, const vector<cenemy>& enemiesList);
};
