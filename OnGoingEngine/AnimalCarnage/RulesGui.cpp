#include "RulesGui.h"
#include "System.h"
#include "MainMenu.h"

void RulesGui::changeSelected_Keyboard()
{
	GuiElement* newSelected = nullptr;

	if (System::theKeyboard->KeyIsPressed('W'))
	{
		newSelected = this->selectedElement->getUp();
	}
	if (System::theKeyboard->KeyIsPressed('S'))
	{
		newSelected = this->selectedElement->getDown();
	}
	if (System::theKeyboard->KeyIsPressed('A'))
	{
		newSelected = this->selectedElement->getLeft();
	}
	if (System::theKeyboard->KeyIsPressed('D'))
	{
		newSelected = this->selectedElement->getRight();
	}

	if (newSelected != nullptr)
	{
		this->changedLastFrame = true;
		this->selectedElement = newSelected;
	}
	else
	{
		this->changedLastFrame = false;
	}
}

void RulesGui::changeSelected()
{
	GuiElement* newSelected = nullptr;

	if (System::theTracker->dpadUp == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getUp();
	}
	if (System::theTracker->dpadDown == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getDown();
	}
	if (System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getLeft();
	}
	if (System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
	{
		newSelected = this->selectedElement->getRight();
	}

	if (newSelected != nullptr)
	{
		this->selectedElement = newSelected;
	}
}

RulesGui::RulesGui(State* myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->buttonGunGame = nullptr;
	this->buttonVersus = nullptr;
	this->buttonFalling = nullptr;
	this->buttonTagTeam = nullptr;

	this->changedLastFrame = false;
	this->timeSinceChanged = 0.0F;
}

RulesGui::~RulesGui()
{
	delete this->buttonGunGame;
	delete this->buttonVersus;
	delete this->buttonFalling;
	delete this->buttonTagTeam;
}

bool RulesGui::initialize()
{
	this->buttonGunGame = new Button("Gun Game", Vector2(WIDTH / 2 - 300, HEIGHT / 2 - 210));
	this->buttonVersus = new Button("Versus", Vector2(WIDTH / 2 - 300, HEIGHT / 2 - 70));
	this->buttonFalling = new Button("Falling", Vector2(WIDTH / 2 - 300, HEIGHT / 2 + 70));
	this->buttonTagTeam = new Button("Tag Team", Vector2(WIDTH / 2 - 300, HEIGHT / 2 + 210));

	this->selectedElement = buttonGunGame;
	this->buttonGunGame->setConnectedElements(nullptr, nullptr, buttonTagTeam, buttonVersus);
	this->buttonVersus->setConnectedElements(nullptr, nullptr, buttonGunGame, buttonFalling);
	this->buttonFalling->setConnectedElements(nullptr, nullptr, buttonVersus, buttonTagTeam);
	this->buttonTagTeam->setConnectedElements(nullptr, nullptr, buttonFalling, buttonGunGame);

	return true;
}

void RulesGui::shutDown()
{
	delete this->buttonGunGame;
	delete this->buttonVersus;
	delete this->buttonFalling;
	delete this->buttonTagTeam;
}

bool RulesGui::update(float deltaTime)
{
	if (this->keyboardDelay <= 0.0F)
	{
		if (this->changedLastFrame)
		{
			if (this->timeSinceChanged > 0.2F)
			{
				this->timeSinceChanged -= 0.2F;
				this->changeSelected_Keyboard();
			}

			this->timeSinceChanged += deltaTime;
		}
		else
		{
			this->timeSinceChanged = 0.0F;
			this->changeSelected_Keyboard();
		}

		if (System::theKeyboard->KeyIsPressed('E'))
		{
			if (this->selectedElement == buttonGunGame)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(SELECT);
			}
			else if (this->selectedElement == buttonVersus)
			{

			}
			else if (this->selectedElement == buttonFalling)
			{

			}
			else
			{

			}
		}
		else if (System::theKeyboard->KeyIsPressed('Q'))
		{
			MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
			state->setCurrentMenu(MAIN);
		}
	}
	else
	{
		this->keyboardDelay -= deltaTime;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);
			this->changeSelected();

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->selectedElement == buttonGunGame)
				{
					MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
					state->setCurrentMenu(SELECT);
				}
				else if (this->selectedElement == buttonVersus)
				{

				}
				else if (this->selectedElement == buttonFalling)
				{

				}
				else
				{

				}
			}
			else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN);
			}

			break;
		}
	}

	return true;
}

bool RulesGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());

	Vector2 textWidth = System::getFontArial()->MeasureString("Select Gamemode");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Select Gamemode", Vector2(WIDTH / 2, HEIGHT / 2 - 400), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);
	this->buttonGunGame->render(this->selectedElement == buttonGunGame);
	this->buttonVersus->render(this->selectedElement == buttonVersus);
	this->buttonFalling->render(this->selectedElement == buttonFalling);
	this->buttonTagTeam->render(this->selectedElement == buttonTagTeam);

	System::getSpriteBatch()->End();
	return true;
}
