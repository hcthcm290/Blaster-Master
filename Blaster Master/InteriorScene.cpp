#include "InteriorScene.h"
#include "Utils.h"
#include "BigJason.h"
#include "Camera.h"
#include "CameraBoundaryLib.h"
#include "Eyeball_Spawner.h"
#include "Eyeball_Curved.h"

InteriorScene::InteriorScene(int id, LPCWSTR filePath)
	:
	CPlayScene(id, filePath)
{
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
	case 9: {
		int l = atoi(tokens[3].c_str());
		int t = atoi(tokens[4].c_str());
		int r = atoi(tokens[5].c_str());
		int b = atoi(tokens[6].c_str());
		obj = new EyeballSpawner((float)l, (float)t, (float)r, (float)b);
		break;
	}
	case 10: {
		obj = new EyeballCurved();
	}
	}

	obj->SetPosition(x, y);
	AddGameObjectToScene(obj);

}
