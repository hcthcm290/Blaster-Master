class State {
public:
	//P2_JASON
	static const int _JASON_IDLE_	=	29901;
	static const int _JASON_WALK_	=	29902;
	static const int _JASON_JUMP_	=	29903;
	static const int _JASON_CLIMB_	=	29904;
	static const int _JASON_CRAWL_	=	29905;
	static const int _JASON_CMOVE_	=	29906;
	static const int _JASON_DIE_	=	29907;
	static const int _JASON_SWIM_	=	29908;
};

class Environment {
public:
	static const int GRAVITY = 2;
};