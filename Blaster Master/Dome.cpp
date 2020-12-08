#include "Dome.h"
#include "ColliableBrick.h"
#include "CollisionSystem.h"
#include "PlayScene.h"
#include <time.h>
#include "Debug.h"
#include "SoundManager.h"

Dome::Dome(int id_gravity, int id_direction)
{
	//set HP
	HP = 60;

	animator->AddAnimation(20401);
	animator->AddAnimation(20402);

	currentState = State::_DOME_WALKING_;

	switch (id_gravity)
	{
	case 1:
		this->gravity = D3DXVECTOR2(0, -1);
		break;
	case 2:
		this->gravity = D3DXVECTOR2(-1, 0);
		break;
	case 3:
		this->gravity = D3DXVECTOR2(0, 1);
		break;
	case 4:
		this->gravity = D3DXVECTOR2(1, 0);
		break;
	}

	switch (id_direction)
	{
	case 1:
		this->direction = D3DXVECTOR2(0, -1);
		break;
	case 2:
		this->direction = D3DXVECTOR2(-1, 0);
		break;
	case 3:
		this->direction = D3DXVECTOR2(0, 1);
		break;
	case 4:
		this->direction = D3DXVECTOR2(1, 0);
		break;
	}
}

void Dome::Update(float dt)
{
	//DebugOut(L"d %f	%f\n", direction.x, direction.y);
	CGameObject* player = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (CollisionSystem::CheckOverlap(this, player))
	{
		dynamic_cast<DynamicObject*>(player)->TakeDamage(7);
	}

	if (currentState == State::_DOME_WALKING_)
	{
		flyReloadTime -= dt;
		soundCount += dt;//Count to play sound : Trung Nguyễn
		if (soundCount >= 1)
		{
			soundCount = 0;
			SoundManager::GetInstance()->PlaySoundW("DomeWalking.wav");
		}
		int width = 15;
		int height = 15;

		DynamicObject* target = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		D3DXVECTOR2 vectorToTarget = D3DXVECTOR2(target->GetPosition().x - x, target->GetPosition().y - y);

		if (std::abs(vectorToTarget.x) < width / 2) vectorToTarget.x = 0;
		if (std::abs(vectorToTarget.y) < height / 2) vectorToTarget.y = 0;

		if (!(vectorToTarget.x != 0 && vectorToTarget.y != 0) && flyReloadTime <= 0)
		{
			bool startFlying = false;

			if (gravity.x != 0 && vectorToTarget.x != 0 && (gravity.x / vectorToTarget.x) < 0)
			{
				currentState = State::_DOME_FLYING_;
				direction.x = std::abs(vectorToTarget.x)/vectorToTarget.x;
				direction.y = 0;
				SoundManager::GetInstance()->PlaySoundW("DomeFLying.wav");// Trung Nguyễn
				startFlying = true;
			}

			if (gravity.y != 0 && vectorToTarget.y != 0 && (gravity.y / vectorToTarget.y) < 0)
			{
				currentState = State::_DOME_FLYING_;
				direction.x = 0;
				direction.y = std::abs(vectorToTarget.y) / vectorToTarget.y;
				SoundManager::GetInstance()->PlaySoundW("DomeFLying.wav");// Trung Nguyễn
				startFlying = true;
			}

			if (startFlying)
			{
				vx = 0;
				vy = 0;
				ground.clear();

				waitToFly = 0.3;
				flyReloadTime = 1;

				return;
			}
		}

		// change direction and gravity when at the cliff //
		CGameObject* fakeObject = new Dome();
		fakeObject->SetPosition(x + 3 * gravity.x + direction.x, y + 3 * gravity.y + direction.y);

		bool onTheGround = false;

		for (auto brick : ground)
		{
			if (CollisionSystem::CheckOverlap(fakeObject, brick))
			{
				onTheGround = true;
				break;
			}
		}

		if (ground.size() == 0) onTheGround = true;

		if (!onTheGround)
		{
			x = fakeObject->GetPosition().x;
			y = fakeObject->GetPosition().y;

			auto temp = gravity;
			gravity = -direction;
			direction = temp;
		}

		vx = movingSpeed * (direction.x + gravity.x);
		vy = movingSpeed * (direction.y + gravity.y);

		ground.clear();
		delete fakeObject;
	}
	else if (currentState == State::_DOME_FLYING_)
	{
		waitToFly -= dt;
		if (waitToFly <= 0)
		{
			vx = flyingSpeed * direction.x;
			vy = flyingSpeed * direction.y;
		}
	}
}

void Dome::Render()
{
	float rotation = 0;

	if (this->gravity.y == 1)
	{
		rotation = 0;
	}
	if (this->gravity.y == -1)
	{
		rotation = 180;
	}
	if (this->gravity.x == -1)
	{
		rotation = 90;
	}
	if (this->gravity.x == 1)
	{
		rotation = -90;
	}
	if (inv != -1) {
		animator->Draw(20402, x, y, false, rotation, Color[inv]);
		if (GetTickCount64() - last_blink >= 50) {
			if (GetTickCount64() > startTakeDamage + 150)
			{
				inv = -1;
			}
			else
			{
				last_blink = GetTickCount64();
				switch (inv)
				{
				case 1: inv = 0; break;
				case 0: inv = 1; break;
				}
			}
		}
	}
	else
	{
		animator->Draw(20402, x, y, false, rotation);
	}
	
}

void Dome::OnCollisionEnter(CollisionEvent e)
{
	if (dynamic_cast<ColliableBrick*>(e.pGameObject) != NULL)
	{
		if (currentState == State::_DOME_WALKING_)
		{
			if (direction.x != 0 && e.nx != 0 && (direction.x / e.nx) < 0)
			{
				auto temp = gravity;
				gravity = direction;
				direction = -temp;
			}

			if (direction.y != 0 && e.ny != 0 && (direction.y / e.ny) < 0)
			{
				auto temp = gravity;
				gravity = direction;
				direction = -temp;
			}

			if (gravity.x != 0 && e.nx != 0 && (gravity.x / e.nx) < 0)
			{
				ground.emplace_back(dynamic_cast<ColliableBrick*>(e.pGameObject));
			}

			if (gravity.y != 0 && e.ny != 0 && (gravity.y / e.ny) < 0)
			{
				ground.emplace_back(dynamic_cast<ColliableBrick*>(e.pGameObject));
			}
		}
		else if (currentState == State::_DOME_FLYING_)
		{
			srand(time(NULL));
			int randomDirection = rand() % 2;
			if (randomDirection == 0) randomDirection = -1;

			gravity = direction;

			if (gravity.x == 0)
			{
				direction = D3DXVECTOR2(randomDirection, 0);
			}
			else if (gravity.y == 0)
			{
				direction = D3DXVECTOR2(0, randomDirection);
			}

			currentState = State::_DOME_WALKING_;
		}
	}
}

FRECT Dome::GetCollisionBox()
{
	int width = 15;
	int height = 15;
	return FRECT(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}
