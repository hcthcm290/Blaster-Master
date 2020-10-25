#include "ColliableBrick.h"

FRECT ColliableBrick::GetCollisionBox()
{
    FRECT ColRECT;
    ColRECT.left = x - 8;
    ColRECT.right = x + 8;
    ColRECT.top = y - 8;
    ColRECT.bottom = y + 8;

    return ColRECT;
}
