#include "cbullet.h"

cbullet::cbullet()
    : _bpos(cpoint()), _damage(0.0), _bpath({}), _isActive(true) {
}

cbullet::cbullet(cpoint pos, double damage, vector<cpoint> path)
    : _bpos(pos), _damage(damage), _bpath(path)
{
    _isActive = true;
}

cpoint cbullet::getCurr() const { return _bpos; }
double cbullet::getDamage() const { return _damage; }
bool cbullet::isActive() const { return _isActive; }

void cbullet::setCurr(cpoint pos) { _bpos = pos; }
void cbullet::setDamage(double damage) { _damage = damage; }
void cbullet::setActive(bool active) { _isActive = active; }

vector<cpoint>& cbullet::getPath() { return _bpath; }
void cbullet::setPath(const vector<cpoint>& path) { _bpath = path; }

