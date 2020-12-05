#include "InteriorScene.h"
#include "Utils.h"
#include "BigJason.h"
#include "Camera.h"
#include "Canon.h"
#include "Spike.h"
#include "CameraBoundaryLib.h"
#include "Teleporter.h"
#include "Rock.h"
#include  "SoundManager.h"

InteriorScene::InteriorScene(int id, LPCWSTR filePath)
	:
	CPlayScene(id, filePath)
{
}
InteriorScene::~InteriorScene()
{
	SoundManager::GetInstance()->Release();
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
	case 16:
	{
		obj = new Rock();
		break;
	}
	case 99:
		obj = new BigJason();
		player = dynamic_cast<DynamicObject*>(obj);
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
		int length = atoi(tokens[3].c_str());
		obj = new Spike(length);
		break;
	}
	}

	obj->SetPosition(x, y);
	AddGameObjectToScene(obj);

}
