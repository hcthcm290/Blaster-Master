#include "DynamicObject.h"

D3DVECTOR DynamicObject::GetVelocity()
{
    D3DVECTOR velocity;
    velocity.x = this->vx;
    velocity.y = this->vy;
    velocity.z = 0;
    return  velocity;
}
