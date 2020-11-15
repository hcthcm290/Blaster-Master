#include "DynamicObject.h"
#include "Explosive.h"
#include "PlayScene.h"

D3DVECTOR DynamicObject::GetVelocity()
{
    D3DVECTOR velocity;
    velocity.x = this->vx;
    velocity.y = this->vy;
    velocity.z = 0;
    return  velocity;
}

void DynamicObject::TakeDamage(int dmg)
{
    this->HP -= dmg;
    if (HP < 0)
    {
        HP = 0;
    }
    if (HP == 0)
    {
        auto explode = new Explosive();
        explode->SetPosition(x, y);
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
    }
}