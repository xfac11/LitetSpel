#include "RulesGui.h"
#include "System.h"
#include "MainMenu.h"

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
		//this->changedLastFrame = true;
		this->selectedElement = newSelected;
	}
	//else
	//{
		//this->changedLastFrame = false;
	//}
}

RulesGui::RulesGui(State* myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->testCheckBox = nullptr;
	this->confirmButton = nullptr;
}

RulesGui::~RulesGui()
{
	delete this->testCheckBox;
	delete this->confirmButton;
}

bool RulesGui::initialize()
{
	this->testCheckBox = new CheckBox("checkBG.tga", "check.tga", false, Vector2(400, 200));
	this->confirmButton = new Button("cat.tga", "Confirm", Vector2(200, 400), Vector2(1000, 100));

	this->selectedElement = testCheckBox;
	this->testCheckBox->setConnectedElements(nullptr, nullptr, confirmButton, confirmButton);
	this->confirmButton->setConnectedElements(nullptr, nullptr, testCheckBox, testCheckBox);

	return true;
}

void RulesGui::shutDown()
{
	delete this->testCheckBox;
	delete this->confirmButton;
}

bool RulesGui::update(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		DirectX::GamePad::State gamepadState = System::theGamePad->GetState(i);

		if (gamepadState.IsConnected())
		{
			System::theTracker->Update(gamepadState);
			this->changeSelected();


			DirectX::GamePad::ButtonStateTracker temp;
			temp.a = System::theTracker->a;
			temp.b = System::theTracker->b;
			temp.x = System::theTracker->x;
			temp.y = System::theTracker->y;

			if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
			{
				if (this->selectedElement == this->testCheckBox)
				{
					this->testCheckBox->setChecked(!this->testCheckBox->isChecked());
				}
				else
				{
					System::setState(GUNGAME);
				}

				System::theRumble[i].rumble.x = 0.2f;
				System::theRumble[i].rumble.y = 0.2f;
				System::theRumble[i].rumbleTime = 0.2f;
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
	System::getSpriteBatch()->Begin();
	System::getSpriteBatch()->Draw(this->testCheckBox->getTexture(), this->testCheckBox->getPosition(), nullptr, this->testCheckBox == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);

	if (this->testCheckBox->isChecked())
	{
		System::getSpriteBatch()->Draw(this->testCheckBox->getCheckTexture(), this->testCheckBox->getPosition(), nullptr, this->testCheckBox == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	}

	System::getSpriteBatch()->Draw(this->confirmButton->getTexture(), this->confirmButton->getRect(), this->confirmButton == this->selectedElement ? DirectX::Colors::Red : DirectX::Colors::White);
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), this->confirmButton->getText().c_str(), this->confirmButton->getPosition(), DirectX::Colors::Black, 0.0f, Vector2::Zero, Vector2::One * 3);
	System::getSpriteBatch()->End();
	return true;
}
