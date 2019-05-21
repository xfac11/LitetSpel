#include "MainGui.h"
#include "System.h"
#include "MainMenu.h"

void MainGui::changeSelected_Keyboard()
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

void MainGui::changeSelected()
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

MainGui::MainGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->playButton = nullptr;
	this->optionsButton = nullptr;
	this->quitButton = nullptr;

	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

MainGui::~MainGui()
{
	delete this->playButton;
	delete this->optionsButton;
	delete this->quitButton;
}

bool MainGui::initialize()
{
	this->playButton = new Button("Start", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F - 140));
	this->optionsButton = new Button("Options", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F));
	this->quitButton = new Button("Quit", Vector2(System::theWindow.width / 2.0F - 300, System::theWindow.height / 2.0F + 140));

	this->selectedElement = playButton;
	this->playButton->setConnectedElements(nullptr, nullptr, quitButton, optionsButton);
	this->optionsButton->setConnectedElements(nullptr, nullptr, playButton, quitButton);
	this->quitButton->setConnectedElements(nullptr, nullptr, optionsButton, playButton);

	return true;
}

void MainGui::shutDown()
{
	delete this->playButton;
	delete this->optionsButton;
	delete this->quitButton;

	this->selectedElement = nullptr;

	this->playButton = nullptr;
	this->optionsButton = nullptr;
	this->quitButton = nullptr;

	this->timeSinceChanged = 0.0F;
	this->changedLastFrame = false;
}

bool MainGui::update(float deltaTime)
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
			if (this->selectedElement == this->quitButton)
			{
				System::closeWindow();
			}
			else if (this->selectedElement == this->playButton)
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(SELECT/*RULES*/);
			}
			else
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(OPTIONS);
			}
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
				if (this->selectedElement == this->quitButton)
				{
					System::closeWindow();
				}
				else if (this->selectedElement == this->playButton)
				{
					MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
					state->setCurrentMenu(SELECT/*RULES*/);
				}
				else
				{
					MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
					state->setCurrentMenu(OPTIONS);
				}
			}

			break;
		}
	}

	return true;
}

bool MainGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied());

	this->playButton->render(this->playButton == this->selectedElement);
	this->optionsButton->render(this->optionsButton == this->selectedElement);
	this->quitButton->render(this->quitButton == this->selectedElement);

	System::getSpriteBatch()->End();
	return true;
}