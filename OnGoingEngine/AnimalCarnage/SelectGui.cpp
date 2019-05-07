#include "SelectGui.h"
#include "System.h"
#include "MainMenu.h"

void SelectGui::changeSelected_Keyboard()
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

void SelectGui::changeSelected()
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

SelectGui::SelectGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;
	this->buttonTest = nullptr;
	this->playerSelector0 = nullptr;

	this->changedLastFrame = false;
	this->timeSinceChanged = 0.0F;
}

SelectGui::~SelectGui()
{
	delete this->playerSelector0;
	delete this->playerSelector1;
	delete this->playerSelector2;
	delete this->playerSelector3;
	delete this->buttonTest;
}

bool SelectGui::initialize()
{
	this->buttonTest = new Button("Start Game", Vector2(WIDTH / 2 - 300, HEIGHT / 2 + 300));
	this->playerSelector0 = new PlayerSelector(WOLF, Vector2(WIDTH / 2 - 775, HEIGHT / 2 - 300));
	this->playerSelector1 = new PlayerSelector(WOLF, Vector2(WIDTH / 2 - 375, HEIGHT / 2 - 300));
	this->playerSelector2 = new PlayerSelector(WOLF, Vector2(WIDTH / 2 + 25, HEIGHT / 2 - 300));
	this->playerSelector3 = new PlayerSelector(WOLF, Vector2(WIDTH / 2 + 425, HEIGHT / 2 - 300));

	this->selectedElement = buttonTest;
	this->buttonTest->setConnectedElements(nullptr, nullptr, nullptr, nullptr);
	return true;
}

void SelectGui::shutDown()
{
	delete this->playerSelector0;
	delete this->playerSelector1;
	delete this->playerSelector2;
	delete this->playerSelector3;
	delete this->buttonTest;

	this->selectedElement = nullptr;
	this->buttonTest = nullptr;
	this->playerSelector0 = nullptr;

	this->changedLastFrame = false;
	this->timeSinceChanged = 0.0F;
}

bool SelectGui::update(float deltaTime)
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
			if (this->selectedElement == buttonTest)
			{
				System::setState(GUNGAME);
			}
		}
		else if (System::theKeyboard->KeyIsPressed('Q'))
		{
			MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
			state->setCurrentMenu(RULES);
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
				if (this->selectedElement == buttonTest)
				{
					System::setState(GUNGAME);
				}
			}
			else if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(RULES);
			}

			break;
		}
	}

	return true;
}

bool SelectGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());

	Vector2 textWidth = System::getFontArial()->MeasureString("Player Select");
	System::getFontArial()->DrawString(System::getSpriteBatch(), "Player Select", Vector2(WIDTH / 2, HEIGHT / 2 - 400), DirectX::Colors::Black, 0.0f, textWidth / 2.f, Vector2::One);
	this->buttonTest->render(this->selectedElement == buttonTest);
	this->playerSelector0->render(false);
	this->playerSelector1->render(false);
	this->playerSelector2->render(false);
	this->playerSelector3->render(false);

	System::getSpriteBatch()->End();
	return true;
}