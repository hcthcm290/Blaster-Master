#include "Rock.h"
#include "InteriorScene.h"
#include <time.h>
#include "PlayerItem.h"
Rock::Rock()
{
	//set HP
	HP = 1;
	SetSpriteID(11601);
}

FRECT Rock::GetCollisionBox()
{
	FRECT colRect;
	
	colRect.left = x - 16 / 2;
	colRect.right = x + 16 / 2;
	colRect.top = y - 16 / 2;
	colRect.bottom = y + 16 / 2;
	return colRect;
}

void Rock::OnCollisionEnter(CollisionEvent e)
{
}

void Rock::Update(float dt)
{

}

void Rock::TakeDamage(int dmg)
{
    this->HP -= dmg;
    if (HP < 0)
    {
        HP = 0;
    }
    if (HP == 0)
    {
        srand((int)time(0));
        int temp = rand() % 8;
        if (temp == 0)
        {
            PlayerItem* newPlayerItem = new PlayerItem(Power);
            newPlayerItem->SetPosition(x, y);
            dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
        }
        else
        {
            if (temp == 1)
            {
                PlayerItem* newPlayerItem = new PlayerItem(Gun);
                newPlayerItem->SetPosition(x, y);
                dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(newPlayerItem);
            }
        }
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->RemoveGameObjectFromScene(this);
    }
}