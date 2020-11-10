#pragma once

class State {
public:
#pragma region Jason
	static const int _JASON_IDLE_	=	29901;
	static const int _JASON_WALK_	=	29902;
	static const int _JASON_JUMP_	=	29903;
	static const int _JASON_CLIMB_	=	29904;
	static const int _JASON_CRAWL_	=	29905;
	static const int _JASON_CMOVE_	=	29906;
	static const int _JASON_DIE_	=	29907;
	static const int _JASON_SWIM_	=	29908;
#pragma endregion

#pragma region Jumper
	static const int _JUMPER_IDLE_	=	20101;
	static const int _JUMPER_WALK_	=	20102;
#pragma endregion

#pragma region Dome
	static const int _DOME_FLYING_	=	20401;
	static const int _DOME_WALKING_ =	20402;
	
#pragma endregion

#pragma region Floater
	static const int _FLOATER_IDLE_ = 20201;
	static const int _FLOATER_FLY_ = 20202;
	static const int _FLOATER_SPEED_ = 40;
	static const int _FLOATER_BULLET_ = 22203;
#pragma endregion

#pragma region Jason_Bullet
	static const int _BULLET_JASON_	=	26901;
#pragma endregion

#pragma region Insect
	static const int _INSECT_		=	20602;
#pragma endregion

#pragma region PlayScene
	static const int _PLAYSCENE_FREE_PLAYING_ = 27001;
	static const int _PLAYSCENE_SWITCH_SECTION = 27002;
#pragma endregion

};