#pragma once
#include "cpoint.h"
#include "cenemy.h"
#include <vector>

using namespace std;

class cbullet
{
private:
    cpoint _bpos;   // Tọa độ của đạn
    double _damage; // Sát thương
    vector<cpoint> _bpath;  // Đường đi
    bool _isActive; // Kiểm tra xem đạn đã đi hết đường chưa (đi hết thì active là false)

public:
    cbullet();
    cbullet(cpoint pos, double damage, vector<cpoint> path);

    cpoint getCurr() const;
    double getDamage() const;
    bool isActive() const;

    void setCurr(cpoint pos);
    void setDamage(double damage);
    void setActive(bool active);

    vector<cpoint>& getPath();
    void setPath(const vector<cpoint>& path);
};
