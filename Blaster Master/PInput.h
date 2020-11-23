#include "DInput.h"
#include <vector>

using namespace std;

#define PINPUT_SIZE 8

//the keys' states is stored in arrays via these indexes
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SHOOT 4
#define JUMP 5
#define SHIFT 6
#define ENTER 7

//Key customization here
static int Key[] = {
	DIK_UP, //Up
	DIK_DOWN, //Down
	DIK_LEFT, //Left
	DIK_RIGHT,  //Right
	DIK_Z, //Shoot
	DIK_X, //Jump
	DIK_LSHIFT, //Shift
	DIK_RETURN, //Enter
};

class PInput {
private:
	PInput();

	static PInput* __instance;

	//this illustrate whether the key is being pressed
	vector<bool> isPressed;
	//this illustrate whether the key is pressed for the first time since lastest keyup
	vector<bool> firstPress;

	//simulate press action
	void Press(int key);
	
	//simulate release action 
	void Release(int key);
public:
	static PInput* GetInstance();

	//call this before updating Sophia and Jason
	static void Update();

	//return true if the key is being pressed
	static bool KeyPressed(int key);

	//return true if the key was pressed in the this frame
	static bool KeyDown(int key);
};