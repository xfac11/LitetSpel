#include "ResultGui.h"
#include "System.h"

ResultGui::ResultGui(State * myState) : GuiBase(myState)
{
	this->buttonMenu = nullptr;
	this->winnerView = nullptr;
	this->looserView[0] = nullptr;
	this->looserView[1] = nullptr;
	this->looserView[2] = nullptr;

	this->buttonDelay = 0.0f;
}

ResultGui::~ResultGui()
{
	delete this->buttonMenu;
	delete this->winnerView;
	delete this->looserView[0];
	delete this->looserView[1];
	delete this->looserView[2];
}

bool ResultGui::initialize()
{
	this->buttonMenu = new Button("Exit to menu", Vector2(1920 / 2.0f - 300, 1080 - 125));
	this->winnerView = new WinnerView(Vector2(1920 / 2.0F - 775, 1080 / 2.0F - 405));
	this->looserView[0] = new LooserView(Vector2(1920 / 2.0F - 375, 1080 / 2.0F - 300));;
	this->looserView[1] = new LooserView(Vector2(1920 / 2.0F + 25, 1080 / 2.0F - 300));;
	this->looserView[2] = new LooserView(Vector2(1920 / 2.0F + 425, 1080 / 2.0F - 300));;

	return true;
}

void ResultGui::shutDown()
{
	delete this->buttonMenu;
	delete this->winnerView;
	delete this->looserView[0];
	delete this->looserView[1];
	delete this->looserView[2];

	this->buttonMenu = nullptr;
	this->winnerView = nullptr;
	this->looserView[0] = nullptr;
	this->looserView[1] = nullptr;
	this->looserView[2] = nullptr;

	this->buttonDelay = 0.0f;
}

bool ResultGui::update(float deltaTime)
{
	if (this->keyboardDelay <= 0.0F)
	{
		if (System::theKeyboard->KeyIsPressed('E'))
		{
			System::setState(MAINMENU);
		}
	}
	else
	{
		this->keyboardDelay -= deltaTime;
	}

	if (this->buttonDelay < 2.0F)
	{
		this->buttonDelay += deltaTime;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED && this->buttonDelay >= 2.0F)
			{
				System::setState(MAINMENU);
			}
		}
	}

	return true;
}

bool ResultGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());

	Vector2 textWidth = System::getFontArial()->MeasureString("Results");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Results", Vector2(1920 / 2.0F, 100), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);

	this->buttonMenu->render(this->buttonDelay >= 2.0F);
	this->winnerView->render(false);
	this->looserView[0]->render(false);
	this->looserView[1]->render(false);
	this->looserView[2]->render(false);

	System::getSpriteBatch()->End();

	return true;
}
