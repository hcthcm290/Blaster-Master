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

#pragma region Floater
	static const int _FLOATER_IDLE_ = 20201;
	static const int _FLOATER_FLY_ = 20202;
#pragma endregion
};