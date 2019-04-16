#include "MainGui.h"
#include "System.h"

MainGui::MainGui()
{
	this->spriteBatch = nullptr;
	this->spriteFont = nullptr;
	this->playButton = nullptr;
	this->quitButton = nullptr;
}

MainGui::~MainGui()
{
	delete this->spriteBatch;
	delete this->spriteFont;
	delete this->playButton;
	delete this->quitButton;
}

bool MainGui::initialize()
{
	this->spriteBatch = new SpriteBatch(System::getDeviceContext());
	this->spriteFont = new SpriteFont(System::getDevice(), L"./Fonts/comic_sans.spritefont");

	this->playButton = new Button("cat.tga", "Start", Vector2(100, 0), Vector2(1000, 100));
	this->quitButton = new Button("cat2.tga", "Quit", Vector2(100, 200), Vector2(1000, 100));

	return true;
}

void MainGui::shutDown()
{
	delete this->spriteBatch;
	delete this->spriteFont;
	delete this->playButton;
	delete this->quitButton;
}

bool MainGui::update(float deltaTime)
{
	return true;
}

bool MainGui::render()
{
	spriteBatch->Begin();
	spriteBatch->Draw(this->playButton->getTexture(), this->playButton->getRect(), DirectX::Colors::Aqua);
	spriteFont->DrawString(this->spriteBatch, this->playButton->getText().c_str(), this->playButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	spriteBatch->Draw(this->quitButton->getTexture(), this->quitButton->getRect(), DirectX::Colors::Red);
	spriteFont->DrawString(this->spriteBatch, this->quitButton->getText().c_str(), this->quitButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	spriteBatch->End();

	return true;
}
