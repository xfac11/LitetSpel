#include "Bear.h"
#include "GamePad.h"
enum CHARACTER{
	BEAR,FOX
};
class Player:public GameObject
{
private:
	//ska den h�lla i en animal eller flera
	Animal animal[4];
	//GamePad* gamePad;
	int current;
public:
	Player();
	~Player();
	void SelectCharacter(CHARACTER b = BEAR);
	void update();
	AABB GetBody();
};

