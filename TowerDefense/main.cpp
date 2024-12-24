#include "cgame.h"
#include "ctool.h"

int main() 
{
    ctool::printMenu();

    cgame cg;
    cg.startGame();
    cg.processGame();


    return 0;
}
