#include "ColliableBrick.h"

FRECT ColliableBrick::GetCollisionBox()
{
    FRECT ColRECT;
    ColRECT.left = x - width/2;
    ColRECT.right = x + width/2;
    ColRECT.top = y - height/2;
    ColRECT.bottom = y + height/2;

    return ColRECT;
}
