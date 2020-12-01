#pragma once

class State {
public:
#pragma region Object 
	static const int _P_ITEM_POWER_ = 20001;
	static const int _P_ITEM_HOVER_ = 20002;
	static const int _P_ITEM_GUN_ = 20003;
	static const int _P_ITEM_HOMING_MISSLE_ = 20004;
	static const int _P_ITEM_MULTI_WARHEAD_ = 20005;
	static const int _P_ITEM_THUNDER_BREAK_ = 20006;
#pragma endregion

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
	static const int _FLOATER_SHOT_ = 20201;
	static const int _FLOATER_FLY_ = 20202;
	static const int _FLOATER_SPEED_ = 40;
	static const int _FLOATER_BULLET_ = 22203;
#pragma endregion
#pragma region Ship

	static const int _SHIP_FLY_ = 22702;
	static const int _SHIP_IDLE_ = 22701;

#pragma endregion
	
#pragma region Canon
	static const int _CANON_ = 21101;
	static const int _CANON_HOR_ = 21102;
	static const int _CANON_VER_ = 21103;
	static const int _CANON_BULLET_ = 211101;
#pragma endregion

#pragma region Orb
	static const int _ORB_FLY_ = 20901;
	static const int _ORB_UOD_ = 20902;
	static const int _AGR_ORB_IDLE_ = 20911;
	static const int _AGR_ORB_FLY_ = 20912;
#pragma endregion

#pragma region Spike
	static const int _SPIKE_ = 12301;
#pragma endregion

	static const int _BULLET_JASON_	=	26901;
#pragma endregion

#pragma region Insect
	static const int _INSECT_		=	20602;
#pragma endregion

#pragma region Lava
	static const int _LAVA_			=	23001;
#pragma endregion

#pragma region Ladder
	static const int _LADDER_		=	23101;
#pragma endregion

#pragma region PlayScene
	static const int _PLAYSCENE_FREE_PLAYING_ = 27001;
	static const int _PLAYSCENE_SWITCH_SECTION = 27002;
#pragma endregion

};