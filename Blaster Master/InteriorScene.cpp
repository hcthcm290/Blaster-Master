#include "InteriorScene.h"
#include "Utils.h"
#include "BigJason.h"
#include "Camera.h"
#include "CameraBoundaryLib.h"
#include "Teleporter.h"
#include "CollisionSystem.h"
#include "PInput.h"

InteriorScene::InteriorScene(int id, LPCWSTR filePath)
	:
	CPlayScene(id, filePath)
{
}

void InteriorScene::Unload()
{
	unordered_map<CGameObject*, int> listObject;

	for (auto& block : sceneObjects)
	{
		for (auto obj : block.second)
		{
			listObject[obj] = 1;
		}
		block.second.clear();
	}
	sceneObjects.clear();

	for (auto& block : playableObjects)
	{
		for (auto obj : block.second)
		{
			listObject[obj] = 1;
		}
		block.second.clear();
	}
	sceneObjects.clear();

	for (auto& obj : listObject)
	{
		delete obj.first;
	}

	onScreenObjs.clear();
}

void InteriorScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());

	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type) 
	{
	case -1:
		// MAP BACKGROUND //
		obj = new StaticObject();
		obj->SetPosition(x, y);
		dynamic_cast<StaticObject*>(obj)->SetSpriteID(-1);
		mapBackground = dynamic_cast<StaticObject*>(obj);
		return;
	case 10:
	{
		FRECT zone;
		zone.left = atoi(tokens[3].c_str()) * 16 - 8;
		zone.top = atoi(tokens[4].c_str()) * 16 - 8;
		zone.right = atoi(tokens[5].c_str()) * 16 + 8;
		zone.bottom = atoi(tokens[6].c_str()) * 16 + 8;
		obj = new Teleporter(zone);

		break;
	}
	case 99:
		obj = new BigJason();
		player = dynamic_cast<DynamicObject*>(obj);
		break;
	case 76:
	{
		FRECT cameraBoundingBox;
		cameraBoundingBox.left = atoi(tokens[2].c_str()) * 16 - 8;
		cameraBoundingBox.top = atoi(tokens[3].c_str()) * 16 - 8;
		cameraBoundingBox.right = atoi(tokens[4].c_str()) * 16 + 8;
		cameraBoundingBox.bottom = atoi(tokens[5].c_str()) * 16 + 8;

		CameraBoundaryLib::AddCameraBoundary(tokens[1], cameraBoundingBox);
		return;
	}
	case 77:
	{
		BigGate* bg = new BigGate();

		x = atof(tokens[1].c_str()) * 16;
		y = atof(tokens[2].c_str()) * 16;

		bg->shift_direction = D3DXVECTOR2(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));

		bg->shift_time1 = atof(tokens[5].c_str());
		bg->shift_time2 = atof(tokens[6].c_str());

		bg->pre_teleport_delta = D3DXVECTOR2(atof(tokens[7].c_str()), atof(tokens[8].c_str()));

		bg->teleport_delta = D3DXVECTOR2(atof(tokens[10].c_str()), atof(tokens[11].c_str()));


		obj = bg;
		break;
	}
	}

	obj->SetPosition(x, y);
	AddGameObjectToScene(obj);

}

void InteriorScene::UpdateSwitchSection(float dt)
{
	#pragma region Update Enemy Object

	if (dt > 0.1) dt = 0.1;

	if (dt == 0) return;

	PInput::Update();

	// Update for all the game object
	for (int i = 0; i < onScreenObjs.size(); i++)
	{
		if (dynamic_cast<Playable*>(onScreenObjs[i]) != NULL)
			continue;
		onScreenObjs[i]->Update(dt);
	}

	for (int i = 0; i < onScreenObjs.size(); i++)
	{
		if (dynamic_cast<Playable*>(onScreenObjs[i]) != NULL)
			continue;

		if (dynamic_cast<DynamicObject*>(onScreenObjs.at(i)) == NULL)
		{
			continue; // if it not moving, we don't need to docollision for it
		}
		else if (D3DXVECTOR3(dynamic_cast<DynamicObject*>(onScreenObjs.at(i))->GetVelocity()) == D3DXVECTOR3(0, 0, 0))
		{
			continue;
		}
		CollisionSystem::DoCollision(dynamic_cast<DynamicObject*>(onScreenObjs.at(i)), &onScreenObjs, dt);
	}

	ApllyVelocityToGameObjs(dt);

	#pragma endregion

	countingTime1 += dt;

	RemoveGameObjectFromScene(player);

	float baseMovingSpeed = dynamic_cast<Playable*>(player)->GetEnterGateSpeed();

	if (countingTime1 < gate->shift_time1)
	{
		// shifting time 1
		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, player->GetPosition().y);
	}

	if (countingTime1 > gate->shift_time1 && countingTime2 == 0)
	{
		// shift camera
		shiftingCamera = true;

		if (deltaShiftX == 0) // that's mean we've not set it yet
		{
			if (gate->shift_direction.x > 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().right - Camera::GetInstance()->GetCollisionBox().left;
			}

			if (gate->shift_direction.x < 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().left - Camera::GetInstance()->GetCollisionBox().right;
			}
		}

		// shift camera follow y-axis to respect the new camera boundary
		auto cameraRECT = Camera::GetInstance()->GetCollisionBox();
		auto cameraPosition = Camera::GetInstance()->GetPosition();
		Camera::GetInstance()->SetPosition(cameraPosition.x + gate->shift_direction.x * 150 * dt, cameraPosition.y);

		totalShifting += gate->shift_direction.x * 150 * dt;

		if (abs(totalShifting) >= abs(deltaShiftX))
		{
			shiftingCamera = false;

			auto playerPosition = player->GetPosition();

			player->SetPosition(playerPosition.x + gate->pre_teleport_delta.x, playerPosition.y + gate->pre_teleport_delta.y);
		}
	}

	if (countingTime1 > gate->shift_time1 && shiftingCamera == false)
	{
		// shifting time 2
		countingTime2 += dt;

		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, player->GetPosition().y);
	}

	if (countingTime2 >= gate->shift_time2)
	{

		auto playerPosition = player->GetPosition();
		auto cameraPosition = Camera::GetInstance()->GetPosition();

		player->SetPosition(playerPosition.x + gate->teleport_delta.x, playerPosition.y + gate->teleport_delta.y);

		Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));

		Camera::GetInstance()->SetPosition(cameraPosition.x + gate->teleport_delta.x, cameraPosition.y + gate->teleport_delta.y);

		state = State::_PLAYSCENE_FREE_PLAYING_;
	}

	AddGameObjectToScene(player);
	BackupPlayableObject();
}
