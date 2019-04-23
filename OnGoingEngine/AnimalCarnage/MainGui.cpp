#include "MainGui.h"
#include "System.h"
#include "MainMenu.h"

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
		//this->changedLastFrame = true;
		this->selectedElement = newSelected;
	}
	//else
	//{
		//this->changedLastFrame = false;
	//}
}

MainGui::MainGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->playButton = nullptr;
	this->optionsButton = nullptr;
	this->quitButton = nullptr;

	//this->timeSinceChanged = 0.0F;
	//this->changedLastFrame = false;
}

MainGui::~MainGui()
{
	delete this->playButton;
	delete this->optionsButton;
	delete this->quitButton;
}

bool MainGui::initialize()
{
	this->playButton = new Button("cat.tga", "Start", Vector2(100, 0), Vector2(1000, 100));
	this->optionsButton = new Button("cat.tga", "Options", Vector2(100, 200), Vector2(1000, 100));
	this->quitButton = new Button("cat2.tga", "Quit", Vector2(100, 400), Vector2(1000, 100));

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
}

bool MainGui::update(float deltaTime)
{
	//if (this->changedLastFrame)
	//{
	//	if (this->timeSinceChanged > 0.2F)
	//	{
	//		this->timeSinceChanged -= 0.2F;
	//		this->changeSelected();
	//	}

	//	this->timeSinceChanged += deltaTime;
	//}
	//else
	//{
	//	this->timeSinceChanged = 0.0F;
	//	
	//}

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
					state->setCurrentMenu(RULES);
				}
				else
				{
					MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
					state->setCurrentMenu(OPTIONS);
				}

				System::theRumble[i].rumble.x = 0.2f;
				System::theRumble[i].rumble.y = 0.2f;
				System::theRumble[i].rumbleTime = 0.2f;
			}

			break;
		}
	}

	return true;
}

bool MainGui::render()
{
	System::getSpriteBatch()->Begin();
	System::getSpriteBatch()->Draw(this->playButton->getTexture(), this->playButton->getRect(), this->playButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->playButton->getText().c_str(), this->playButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->Draw(this->optionsButton->getTexture(), this->optionsButton->getRect(), this->optionsButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->optionsButton->getText().c_str(), this->optionsButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->Draw(this->quitButton->getTexture(), this->quitButton->getRect(), this->quitButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->quitButton->getText().c_str(), this->quitButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->End();

	return true;
}