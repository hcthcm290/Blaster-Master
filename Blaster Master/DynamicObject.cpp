#include "DynamicObject.h"
#include "Explosive.h"
#include "PlayScene.h"
#include "Debug.h"
#include "SoundManager.h"
#include "PlayerItem.h"

DynamicObject::DynamicObject()
{
    last_blink = GetTickCount();
    startTakeDamage = GetTickCount();
}
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
    startTakeDamage = GetTickCount();
    last_blink = GetTickCount();
    inv = 1;
    if (HP < 0)
    {
        HP = 0;
    }
    if (HP == 0)
    {
        srand((int)time(0));
        if ((rand() % 3) == 0)
        {
            PlayerItem* newPlayerItem = new PlayerItem(Power);
            newPlayerItem->SetPosition(x, y);
            dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
        }

        auto explode = new Explosive();
        explode->SetPosition(x, y);
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(explode);
        if (this != dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer())
            SoundManager::GetInstance()->PlaySoundW("EnemyDie.wav");
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
    }
}