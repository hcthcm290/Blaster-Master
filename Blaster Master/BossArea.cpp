#include "BossArea.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include "Boss.h"
#include "InteriorScene.h"
#include "Debug.h"

FRECT BossArea::GetCollisionBox()
{
    return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

void BossArea::Update(float dt)
{
    if (CollisionSystem::CheckOverlap(this, dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()))
    {
        Countime += dt;

        if (Countime >= delayFadding && Countime < delayFadding + faddingTime / 2)
        {
            // fadding black
            float percentAlpha = (Countime - delayFadding) / (faddingTime / 2);
            blackscreenAlphaValue = 255 * percentAlpha;
        }
        else if (Countime >= delayFadding && Countime < delayFadding + faddingTime)
        {
            dynamic_cast<InteriorScene*>(CGame::GetInstance()->GetCurrentScene())->SetFightingBoss(true);

            // fadding white
            float percentAlpha = 1 - (Countime - delayFadding - faddingTime/2) / (faddingTime / 2);
            blackscreenAlphaValue = 255 * percentAlpha;
            DebugOut(L"percent black: %d\n", blackscreenAlphaValue);
        }
        else if (Countime > MaxCountime && !bossSpawned)
        {

            CGameObject* boss = new Boss(BossInitPosition.x, BossInitPosition.y);
            dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->AddGameObjectToScene(boss);

            bossSpawned = true;
        }
    }
    else
    {
        Countime = 0;
    }

    
}

void BossArea::Render()
{
    if (!bossSpawned && Countime > delayFlashing && Countime < delayFadding)
    {
        backgroundAnimator.Draw(backgroundAnimationID, backgroundPosition.x, backgroundPosition.y, false, 0, D3DCOLOR_XRGB(255, 255, 255), 0.4);
        foregroundAnimator.Draw(foregroundAnimationID, foregroundPosition.x, foregroundPosition.y, false, 0, D3DCOLOR_XRGB(255, 255, 255), 0.8);
    }
    else if (Countime >= delayFadding && Countime < delayFadding + faddingTime)
    {
        // fadding white
        CSprites::GetInstance()->Get(blackscreenSpriteID)->Draw(x, y, false, 0, D3DCOLOR_ARGB(blackscreenAlphaValue, 255, 255, 255), 1);
    }
}
