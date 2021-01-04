#include "CreditController.h"
#include "Camera.h"

CreditController::CreditController()
{
	textRenderer.LoadFont("Font\\CreditFont.txt");
	textRenderer.LoadContent("CreditContent.txt");
}

void CreditController::Update(float dt)
{
	CountTime += dt;

	if (CountTime <= textLivingTime)
	{
		textPosition.y = textPosition.y - textMovingSpeed * dt;
	}
}

void CreditController::Render()
{
	if (CountTime <= textLivingTime)
	{
		textRenderer.Render(textPosition.x, int(textPosition.y));
	}
	else
	{
		CSprites::GetInstance()->Get(IDTheEndText)->Draw(TheEndTextPosition.x, TheEndTextPosition.y);
		textRenderer.Render(-20, 20, "thanks for playing");
	}

}
