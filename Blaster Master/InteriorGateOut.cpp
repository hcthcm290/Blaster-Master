#include "InteriorGateOut.h"
#include "PlayScene.h"
#include "Playable.h"

FRECT InteriorGateOut::GetCollisionBox()
{
    float width = 8;
    float height = 8;

    return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void InteriorGateOut::OnCollisionEnter(CollisionEvent e)
{
    if (dynamic_cast<Playable*>(e.pGameObject) != NULL)
    {
        CGame::GetInstance()->SwitchScene(id_target_scene);
    }
}
