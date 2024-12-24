#include "cenemy.h"

cenemy::cenemy() :_speed(0), _health(0.0), _epos(cpoint()), _epath({}), _index(0), _isAlive(true) {}

cenemy::cenemy(int speed, double health, cpoint pos, vector <cpoint> path)
    : _speed(speed), _health(health), _epos(pos), _epath(path)
{
    _index = 0;
    _isAlive = true;
}

// Getters
int cenemy::getSpeed() const { return _speed; }
double cenemy::getHealth() const { return _health; }
cpoint cenemy::getCurr() const { return _epos; }
vector<cpoint> cenemy::getPath() const { return _epath; }
int cenemy::getIndex() const { return _index; }
bool cenemy::isAlive() const { return _isAlive; }

void cenemy::setSpeed(int speed)
{
    _speed = speed;
}
void cenemy::setHealth(double health)
{
    _health = health;
}
void cenemy::setCurr(cpoint pos)
{
    _epos = pos;
}
void cenemy::setPath(const vector<cpoint>& path)
{
    _epath = path;
}
void cenemy::setIndex(int index)
{
    _index = index;
}
void cenemy::setAlive(bool alive)
{
    _isAlive = alive;
}

void cenemy::decreaseHealth(double damage)
{
    _health -= damage;
}



