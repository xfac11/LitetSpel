#include "StartGui.h"
#include "System.h"
#include "MainMenu.h"

StartGui::StartGui(State* myState) : GuiBase(myState)
{
	System::assetMananger->LoadTexture("menuBG", "menuBG.tga");
	this->texture = System::assetMananger->GetTexture("menuBG");
	this->timer = 0.0F;
}

StartGui::~StartGui()
{
}

bool StartGui::initialize()
{
	return true;
}

void StartGui::shutDown()
{
	this->timer = 0.0F;
}

bool StartGui::update(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);
		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);
			if (System::theTracker->start)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN);
				return true;
			}
		}
	}

	this->timer += deltaTime;

	while (this->timer > 1.5F)
	{
		this->timer -= 1.5F;
	}

	return true;
}

bool StartGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

	System::getSpriteBatch()->Draw(this->texture->getTexture(), SimpleMath::Rectangle(0, 0, 1920, 1080));
	float animate = this->timer / 1.5F;
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Press Start", SimpleMath::Vector2(1920, 1080) / 2.0F, SimpleMath::Color(0, 0, 0, animate), 0, System::getFontArial()->MeasureString("Press Start") / 2.0F, SimpleMath::Vector2::One);

	System::getSpriteBatch()->End();
	return true;
}
