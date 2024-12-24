#include "ctower.h"
#include <iostream>

ctower::ctower() : _tpos(0, 0, 0), _treach({}) {}

ctower::ctower(cpoint pos) : _tpos(pos) {}

cpoint ctower::getCurr() const
{
    return _tpos;
}

void ctower::setCurr(const cpoint& pos)
{
    _tpos = pos;
}

vector<Treach> ctower::getTreach() const
{
    return _treach;
}

void ctower::createTreach(const vector<cenemy>& enemies)
{
    _treach.clear();

    if (enemies.empty())
        return;

    cpoint towerPoint = cpoint::fromXYToRowCol(_tpos);
    int rowTower = towerPoint.getX();
    int colTower = towerPoint.getY();

    for (const auto& enemy : enemies) {
        const auto& enemyPath = enemy.getPath();

        for (int i = 0; i < enemyPath.size(); ++i) {
            cpoint pathPoint = cpoint::fromXYToRowCol(enemyPath[i]);

            int rowPath = pathPoint.getX();
            int colPath = pathPoint.getY();

            if (rowTower == rowPath || colTower == colPath || abs(rowTower - rowPath) == abs(colTower - colPath)) {
                bool exists = false;
                for (const auto& target : _treach)
                {
                    if (target.pos == enemyPath[i])
                    {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    _treach.push_back({ enemyPath[i], i, towerPoint.countSteps(pathPoint) });
                }
            }
        }
    }
}

int ctower::calShootDirection(const vector<cenemy>& enemiesList, int& treachIndex) const
{
    // Lấy tọa độ của tháp trong hệ lưới
    cpoint towerPoint = cpoint::fromXYToRowCol(_tpos);
    int rowTower = towerPoint.getX();
    int colTower = towerPoint.getY();

    treachIndex = -1;

    // Duyệt qua các ô ảnh hưởng trong danh sách
    for (const Treach& treachPoint : _treach)
    {
        treachIndex++;

        int pathIndex = treachPoint.index;

        // Lấy tọa độ của ô ảnh hưởng trong danh sách các ô ảnh hưởng (trong hệ lưới)
        cpoint reachPoint = cpoint::fromXYToRowCol(treachPoint.pos);
        int rowReach = reachPoint.getX();
        int colReach = reachPoint.getY();

        int bulletStep = treachPoint.step;

        for (const auto& enemy : enemiesList)
        {
            int enemyIndex = enemy.getIndex();  // Lấy thứ tự của ô mà kẻ thù đang đứng

            int enemyStep = pathIndex - enemyIndex + 1;     // +1 vì đây là tính khoảng cách giữa 2 ô trên lộ trình của kẻ thù

            // Kiểm tra xem số bước từ kẻ thù đến ô ảnh hưởng có bằng từ trụ đến ô ảnh hưởng ko
            if (enemyStep == bulletStep)
            {

                if (rowReach < rowTower && colReach < colTower)
                    return UP_LEFT;
                if (rowReach < rowTower && colReach == colTower)
                    return UP;
                if (rowReach < rowTower && colReach > colTower)
                    return UP_RIGHT;
                if (rowReach == rowTower && colReach > colTower)
                    return RIGHT;
                if (rowReach > rowTower && colReach > colTower)
                    return DOWN_RIGHT;
                if (rowReach > rowTower && colReach == colTower)
                    return DOWN;
                if (rowReach > rowTower && colReach < colTower)
                    return DOWN_LEFT;
                if (rowReach == rowTower && colReach < colTower)
                    return LEFT;
            }
        }
    }

    // Nếu không có hướng bắn nào thì trả về -1 cho 2 giá trị (treachIndex và shootDirection)

    treachIndex = -1;

    return -1;
}





