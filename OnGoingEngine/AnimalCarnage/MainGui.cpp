#include "MainGui.h"
#include "System.h"
#include "MainMenu.h"

void MainGui::changeSelected(int i)
{
	DirectX::GamePad::State state;
	GuiElement* newSelected = nullptr;

	state = System::theGamePad->GetState(i);
	if (state.IsConnected())
	{
		System::theTracker->Update(state);
		//L+R+A+Start
		if (this->reset(state))
		{

			this->theRumble[i].rumble.x = 0.2f;
			this->theRumble[i].rumble.y = 0.2f;
			this->theRumble[i].rumbleTime = 0.2f;
			//goes back to start screen
		}

		//move
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

void MainGui::updateRumble(float deltaTime, int id)
{
	if (theRumble[id].rumbleTime > theRumble[id].rumbleClock)
	{
		theRumble[id].rumbleClock += deltaTime;
		System::theGamePad->SetVibration(0, theRumble[id].rumble.x, theRumble[id].rumble.y);
	}
	else
	{
		theRumble[id].rumbleClock = 0.f;
		theRumble[id].rumbleTime = 0.f;
		System::theGamePad->SetVibration(0, 0, 0);
	}
}

MainGui::MainGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->playButton = nullptr;
	this->quitButton = nullptr;

	/*this->timeSinceChanged = 0.0F;*/
	this->changedLastFrame = false;
}

MainGui::~MainGui()
{
	delete this->playButton;
	delete this->quitButton;
}

bool MainGui::initialize()
{
	this->playButton = new Button("cat.tga", "Start", Vector2(100, 0), Vector2(1000, 100));
	this->quitButton = new Button("cat2.tga", "Quit", Vector2(100, 200), Vector2(1000, 100));

	this->selectedElement = playButton;
	this->playButton->setConnectedElements(nullptr, nullptr, quitButton, quitButton);
	this->quitButton->setConnectedElements(nullptr, nullptr, playButton, playButton);

	return true;
}

void MainGui::shutDown()
{
	delete this->playButton;
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
		this->changeSelected(i);
		//select/comfirm
		if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
		{
			if (this->selectedElement == this->quitButton)
			{
				System::closeWindow();
			}
			else if (this->selectedElement == this->playButton)
			{
				//MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				//state->setCurrentMenu(OPTIONS);
				this->theRumble[i].rumble.x = 0.4f;
				this->theRumble[i].rumble.y = 0.4f;
				this->theRumble[i].rumbleTime = 0.2f;
				System::setState(GUNGAME);
			}
		}
		//back	
		if (System::theTracker->b == DirectX::GamePad::ButtonStateTracker::PRESSED) //and
		{

			//System::setState(START);
		}
		this->updateRumble(deltaTime, i);
	}
	return true;
}

bool MainGui::render()
{
	System::getSpriteBatch()->Begin();
	System::getSpriteBatch()->Draw(this->playButton->getTexture(), this->playButton->getRect(), this->playButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->playButton->getText().c_str(), this->playButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->Draw(this->quitButton->getTexture(), this->quitButton->getRect(), this->quitButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->quitButton->getText().c_str(), this->quitButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->End();

	return true;
}

bool MainGui::reset(DirectX::GamePad::State state)
{
	bool result = false;
	if (state.IsConnected())
	{
		//tracker.Update(state);
		if (state.IsLeftTriggerPressed() &&
			state.IsRightTriggerPressed() &&
			state.buttons.a &&
			(state.buttons.back || state.buttons.menu))
		{
			result = true;
		}
	}
	return result;
}
