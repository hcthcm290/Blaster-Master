#include "ForegroundTile.h"

FRECT ForegroundTile::GetCollisionBox()
{
    float width = 16;
    float height = 16;

    return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}
