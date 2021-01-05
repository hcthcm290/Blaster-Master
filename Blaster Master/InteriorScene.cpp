#include "InteriorScene.h"
#include "Utils.h"
#include "BigJason.h"
#include "Camera.h"
#include "Canon.h"
#include "Spike_Interior.h"
#include "CameraBoundaryLib.h"
#include "Teleporter.h"
#include "CollisionSystem.h"
#include "PInput.h"
#include "InteriorGateOut.h"
#include "Eyeball_Spawner.h"
#include "TheEye.h"
#include "HealthBarGUI.h"
#include "GunBarGUI.h"
#include "Rock.h"
#include "Eyeball_Curved.h"
#include "Boss.h"
#include "Debug.h"


InteriorScene::InteriorScene(int id, LPCWSTR filePath)
	:
	CPlayScene(id, filePath)
{
}
InteriorScene::~InteriorScene()
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
		//DebugOut(L"%d", zone.right);
		break;
	}
	case 16:
	{
		obj = new Rock();
		break;
	}
	case 99:
		obj = new BigJason();
		player = dynamic_cast<DynamicObject*>(obj);
		
		if (AdaptJasonHealth)
		{
			DynamicObject* Jason = TheEye::GetInstance()->GetJason();
			float JasonHealthPercent;
			if (Jason == NULL)
			{
				JasonHealthPercent = 1;
			}
			else 
			{
				JasonHealthPercent = (float)Jason->GetCurrentHP() / Jason->GetMaxHP();
			}
			player->SetCurrentHP(player->GetMaxHP()*JasonHealthPercent);
		}
		else if (ResetHealth)
		{
			player->SetCurrentHP(player->GetMaxHP());
		}

		break;
	case 11:
		obj = new Canon();
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
	case 23:
	{
		int w = atoi(tokens[3].c_str());
		int h = atoi(tokens[4].c_str());
		obj = new Spike_Interior(w,h);
		break;
	}
	case 77:
	{
		BigGate* bg = new BigGate();

		x = (float)atof(tokens[1].c_str()) * 16;
		y = (float)atof(tokens[2].c_str()) * 16;

		bg->shift_direction = D3DXVECTOR2(atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));

		bg->shift_time1 = atof(tokens[5].c_str());
		bg->shift_time2 = atof(tokens[6].c_str());

		bg->pre_teleport_delta = D3DXVECTOR2(atof(tokens[7].c_str()), atof(tokens[8].c_str()));

		bg->teleport_delta = D3DXVECTOR2(atof(tokens[10].c_str()), atof(tokens[11].c_str()));


		obj = bg;
		break;
	}
	case 29: {
		int l = atoi(tokens[3].c_str());
		int t = atoi(tokens[4].c_str());
		int r = atoi(tokens[5].c_str());
		int b = atoi(tokens[6].c_str());
		obj = new EyeballSpawner((float)l, (float)t, (float)r, (float)b);
		break;
	}
	case 30: {
		obj = new EyeballCurved();
		break;
	}
	case 79:
	{
		FRECT freeMovingAreaInterior;
		freeMovingAreaInterior.left = atof(tokens[1].c_str());
		freeMovingAreaInterior.top = atof(tokens[2].c_str());
		freeMovingAreaInterior.right = atof(tokens[3].c_str());
		freeMovingAreaInterior.bottom = atof(tokens[4].c_str());

		Camera::GetInstance()->SetFreeInteriorMovingArea(freeMovingAreaInterior);

		return;
	}
	case 81:
	{
		obj = new InteriorGateOut();
		x = atof(tokens[1].c_str()) * 16;
		y = atof(tokens[2].c_str()) * 16;
		dynamic_cast<InteriorGateOut*>(obj)->SetIDTargetScene(atoi(tokens[3].c_str()));
		break;
	}
	case 82: 
	{
		obj = new Boss(x, y);
		break;
	}
	default:
	{
		return;
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


	// at the first of shifting we snap the feet of player to correct position of gate
	if (countingTime1 == 0)
	{
		// magic number 10: distance from center of player to its feet
		if (gate->shift_direction.x != 0)
		{
			player->SetPosition(player->GetPosition().x,
								gate->GetPosition().y - 10);
		}

		if (gate->shift_direction.y != 0)
		{
			player->SetPosition(gate->GetPosition().x,
								player->GetPosition().y);
		}
	}

	countingTime1 += dt;

	RemoveGameObjectFromScene(player);

	float baseMovingSpeed = dynamic_cast<Playable*>(player)->GetEnterGateSpeed();

	if (countingTime1 < gate->shift_time1)
	{
		// shifting time 1
		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, 
							player->GetPosition().y + baseMovingSpeed * dt * gate->shift_direction.y);
	}

	if (countingTime1 > gate->shift_time1 && countingTime2 == 0)
	{
		// shift camera
		shiftingCamera = true;


		// we set the maximum shift distance for the camera
		if (deltaShiftX == 0 && gate->shift_direction.x != 0) // that's mean we've not set it yet
		{
			if (gate->shift_direction.x > 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().right - Camera::GetInstance()->GetCollisionBox().left;
			}

			if (gate->shift_direction.x < 0)
			{
				deltaShiftX = Camera::GetInstance()->GetCollisionBox().left - Camera::GetInstance()->GetCollisionBox().right;
			}

			// before shift the camera, we do the pre_tele for the player
			auto playerPosition = player->GetPosition();

			player->SetPosition(playerPosition.x + gate->pre_teleport_delta.x, playerPosition.y + gate->pre_teleport_delta.y);
		}
		else if(deltaShiftY == 0 && gate->shift_direction.y != 0)
		{
			// magic number 32: 2x16 we shift extra 2 tile
			if (gate->shift_direction.y > 0)
			{
				deltaShiftY = Camera::GetInstance()->GetCollisionBox().bottom - Camera::GetInstance()->GetCollisionBox().top + 32;
			}
			if(gate->shift_direction.y < 0)
			{
				deltaShiftY = Camera::GetInstance()->GetCollisionBox().top - Camera::GetInstance()->GetCollisionBox().bottom - 32;
			}

			// before shift the camera, we do the pre_tele for the player
			auto playerPosition = player->GetPosition();

			player->SetPosition(playerPosition.x + gate->pre_teleport_delta.x, playerPosition.y + gate->pre_teleport_delta.y);
		}

		auto cameraRECT = Camera::GetInstance()->GetCollisionBox();
		auto cameraPosition = Camera::GetInstance()->GetPosition();

		// magic number 150: speed of camera when shifting
		Camera::GetInstance()->SetPosition(cameraPosition.x + gate->shift_direction.x * 150 * dt, 
										   cameraPosition.y + gate->shift_direction.y * 150 * dt);

		if (gate->shift_direction.x != 0)
		{
			totalShifting += gate->shift_direction.x * 150 * dt;
		}
		else if (gate->shift_direction.y != 0)
		{
			totalShifting += gate->shift_direction.y * 150 * dt;
		}

		// if the total distance camera shifted over the maximum the distance, we swith to shift time 2 for the player
		if (abs(totalShifting) >= abs(deltaShiftX) && gate->shift_direction.x != 0)
		{
			shiftingCamera = false;
		}
		else if (abs(totalShifting) >= abs(deltaShiftY) && gate->shift_direction.y != 0)
		{
			shiftingCamera = false;
		}
	}

	if (countingTime1 > gate->shift_time1 && shiftingCamera == false)
	{
		// shifting time 2
		countingTime2 += dt;

		player->SetPosition(player->GetPosition().x + baseMovingSpeed * dt * gate->shift_direction.x, 
							player->GetPosition().y + baseMovingSpeed * dt * gate->shift_direction.y);
	}

	// we finish shifttime 2 for the player
	if (countingTime2 >= gate->shift_time2)
	{

		auto playerPosition = player->GetPosition();
		auto cameraPosition = Camera::GetInstance()->GetPosition();

		player->SetPosition(playerPosition.x + gate->teleport_delta.x, playerPosition.y + gate->teleport_delta.y);

		Camera::GetInstance()->SetCameraBoundary(CameraBoundaryLib::GetCameraBoundary(player));


		state = State::_PLAYSCENE_FREE_PLAYING_;
	}

	AddGameObjectToScene(player);
}

void InteriorScene::UpdateFaddingOut(float dt)
{
	totalFaded += dt;
	if (totalFaded >= maxFading)
	{
		// Notify the big-jason to sync with jason before switching scene
		dynamic_cast<BigJason*>(player)->NotifySwitchSceneOut();

		CGame::GetInstance()->ToggleOverrideColorOff();
		CGame::GetInstance()->SwitchScene(id_target_scene);
		return;
	}

	int color = 255 * (1 - totalFaded / maxFading);

	if (color < 70) color = 0;

	CGame::GetInstance()->ToggleOverrideColorOn();
	CGame::GetInstance()->SetOverrideColor(D3DCOLOR_ARGB(255, color, color, color));
}

void InteriorScene::InitGUI()
{
	HealthBarGUI* hpGUI = new HealthBarGUI();
	GUIObjects.emplace_back(hpGUI);

	GunBarGUI* gunGUI = new GunBarGUI();
	GUIObjects.emplace_back(gunGUI);
}
