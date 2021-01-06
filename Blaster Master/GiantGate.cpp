#include "GiantGate.h"
#include "PlayScene.h"
#include "CollisionSystem.h"

FRECT GiantGate::GetCollisionBox()
{
    int width = 16;
    int height = 16;
    return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void GiantGate::Update(float dt)
{
    CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    if (CollisionSystem::CheckOverlap(player, this) && DInput::KeyPress(DIK_DOWN))
    {
        dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SwitchScene(idtargetScene);
    }
}

void GiantGate::Render()
{
}
