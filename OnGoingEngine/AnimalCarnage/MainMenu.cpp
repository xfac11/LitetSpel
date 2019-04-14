#include "MainMenu.h"
#include "System.h"

MainMenu::MainMenu()
{
	this->spritebatch = nullptr;
	this->sprite = nullptr;
}

MainMenu::~MainMenu()
{
	delete this->spritebatch;
	delete this->sprite;
}

bool MainMenu::initailize()
{
	this->spritebatch = new DirectX::SpriteBatch(System::getDeviceContext());
	this->sprite = new Sprite("cat.tga");
	return true;
}

bool MainMenu::render()
{
	this->spritebatch->Begin();
	
	this->spritebatch->Draw(this->sprite->getTexture()->getTexture(), this->sprite->getPosition(), nullptr, Colors::White, 0, this->sprite->getOrigin(), 0.1F);
	this->spritebatch->End();
	return true;
}

bool MainMenu::update(float deltaTime)
{

	return true;
}

void MainMenu::shutDown()
{
	delete this->spritebatch;
	delete this->sprite;
}
