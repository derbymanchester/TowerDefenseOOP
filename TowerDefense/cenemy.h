#pragma once
#include "cpoint.h"
#include <vector>
#include <mutex>

using namespace std;

class cenemy
{
private:
    int _speed;                   // Tốc độ của kẻ thù
    double _health;               // Máu
    cpoint _epos;                 // Tọa độ
    vector<cpoint> _epath;        // Đường đi
    int _index;                   // Thứ tự trên đường đi
    bool _isAlive;        // Trạng thái sống/chết của kẻ thù     

public:
    // Constructors
    cenemy();
    cenemy(int speed, double health, cpoint pos, vector<cpoint> path);

    int getSpeed() const;
    double getHealth() const;
    cpoint getCurr() const;
    vector<cpoint> getPath() const;
    int getIndex() const;
    bool isAlive() const;

    void setSpeed(int speed);
    void setHealth(double health);
    void setCurr(cpoint pos);
    void setPath(const vector<cpoint>& path);
    void setIndex(int index);
    void setAlive(bool alive);

    void decreaseHealth(double damage); // Trừ máu kẻ thù với damage của đạn
};
