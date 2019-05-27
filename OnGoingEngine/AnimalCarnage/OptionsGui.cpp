#include "OptionsGui.h"
#include "System.h"
#include "MainMenu.h"

void OptionsGui::changeSelected_Keyboard()
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

void OptionsGui::changeSelected()
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

OptionsGui::OptionsGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->sliderMusic = nullptr;
	this->sliderSound = nullptr;
	this->checkBoxRumble1 = nullptr;
	this->checkBoxRumble2 = nullptr;
	this->checkBoxRumble3 = nullptr;
	this->checkBoxRumble4 = nullptr;

	this->pressedLastFrame = false;
}

OptionsGui::~OptionsGui()
{
	delete this->sliderMusic;
	delete this->sliderSound;
	delete this->checkBoxRumble1;
	delete this->checkBoxRumble2;
	delete this->checkBoxRumble3;
	delete this->checkBoxRumble4;
}

bool OptionsGui::initialize()
{
	this->sliderMusic = new Slider(100, 100, 5, SimpleMath::Vector2(1920 / 2.0F - 610 / 2, 1080 / 2.0F - 200));
	this->sliderSound = new Slider(100, 100, 5, SimpleMath::Vector2(1920 / 2.0F - 610 / 2, 1080 / 2.0F - 100));
	this->checkBoxRumble1 = new CheckBox(true, SimpleMath::Vector2(1920 / 2.0F - 350, 1080 / 2.0F + 100));
	this->checkBoxRumble2 = new CheckBox(true, SimpleMath::Vector2(1920 / 2.0F - 150, 1080 / 2.0F + 100));
	this->checkBoxRumble3 = new CheckBox(true, SimpleMath::Vector2(1920 / 2.0F + 50, 1080 / 2.0F + 100));
	this->checkBoxRumble4 = new CheckBox(true, SimpleMath::Vector2(1920 / 2.0F + 250, 1080 / 2.0F + 100));

	this->selectedElement = this->sliderMusic;
	this->sliderMusic->setConnectedElements(nullptr, nullptr, checkBoxRumble1, sliderSound);
	this->sliderSound->setConnectedElements(nullptr, nullptr, sliderMusic, checkBoxRumble1);
	this->checkBoxRumble1->setConnectedElements(checkBoxRumble4, checkBoxRumble2, sliderSound, sliderMusic);
	this->checkBoxRumble2->setConnectedElements(checkBoxRumble1, checkBoxRumble3, sliderSound, sliderMusic);
	this->checkBoxRumble3->setConnectedElements(checkBoxRumble2, checkBoxRumble4, sliderSound, sliderMusic);
	this->checkBoxRumble4->setConnectedElements(checkBoxRumble3, checkBoxRumble1, sliderSound, sliderMusic);

	this->sliderMusic->setValue(static_cast<int>(System::getSoundManager()->getMasterVolume() * this->sliderMusic->getMaxValue()));

	return true;
}

void OptionsGui::shutDown()
{
	delete this->sliderMusic;
	delete this->sliderSound;
	delete this->checkBoxRumble1;
	delete this->checkBoxRumble2;
	delete this->checkBoxRumble3;
	delete this->checkBoxRumble4;

	this->selectedElement = nullptr;

	this->sliderMusic = nullptr;
	this->sliderSound = nullptr;
	this->checkBoxRumble1 = nullptr;
	this->checkBoxRumble2 = nullptr;
	this->checkBoxRumble3 = nullptr;
	this->checkBoxRumble4 = nullptr;

	this->pressedLastFrame = false;
}

bool OptionsGui::update(float deltaTime)
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
			if (!this->pressedLastFrame)
			{
				if (this->selectedElement == this->sliderMusic)
				{
					if (this->sliderMusic->getValue() < this->sliderMusic->getMaxValue())
					{
						this->sliderMusic->changeValueWithStep(false);
					}
					else
					{
						this->sliderMusic->setValue(0);
					}

					System::getSoundManager()->setMasterVolume(this->sliderMusic->getValue() / static_cast<float>(this->sliderMusic->getMaxValue()));
				}
				else if (this->selectedElement == this->sliderSound)
				{
					if (this->sliderSound->getValue() < this->sliderSound->getMaxValue())
					{
						this->sliderSound->changeValueWithStep(false);
					}
					else
					{
						this->sliderSound->setValue(0);
					}
				}
				else if (this->selectedElement == this->checkBoxRumble1)
				{
					this->checkBoxRumble1->setChecked(!this->checkBoxRumble1->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble2)
				{
					this->checkBoxRumble2->setChecked(!this->checkBoxRumble2->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble3)
				{
					this->checkBoxRumble3->setChecked(!this->checkBoxRumble3->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble4)
				{
					this->checkBoxRumble4->setChecked(!this->checkBoxRumble4->isChecked());
				}
			}

			this->pressedLastFrame = true;
		}
		else
		{
			this->pressedLastFrame = false;

			if (System::theKeyboard->KeyIsPressed('Q'))
			{
				MainMenu* state = dynamic_cast<MainMenu*>(this->myState);
				state->setCurrentMenu(MAIN);
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
				if (this->selectedElement == this->sliderMusic)
				{
					if (this->sliderMusic->getValue() < this->sliderMusic->getMaxValue())
					{
						this->sliderMusic->changeValueWithStep(false);
					}
					else
					{
						this->sliderMusic->setValue(0);
					}
				}
				else if (this->selectedElement == this->sliderSound)
				{
					if (this->sliderSound->getValue() < this->sliderSound->getMaxValue())
					{
						this->sliderSound->changeValueWithStep(false);
					}
					else
					{
						this->sliderSound->setValue(0);
					}
				}
				else if (this->selectedElement == this->checkBoxRumble1)
				{
					this->checkBoxRumble1->setChecked(!this->checkBoxRumble1->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble2)
				{
					this->checkBoxRumble2->setChecked(!this->checkBoxRumble2->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble3)
				{
					this->checkBoxRumble3->setChecked(!this->checkBoxRumble3->isChecked());
				}
				else if (this->selectedElement == this->checkBoxRumble4)
				{
					this->checkBoxRumble4->setChecked(!this->checkBoxRumble4->isChecked());
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

bool OptionsGui::render()
{
	System::getSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, System::getCommonStates()->NonPremultiplied(), nullptr, nullptr, nullptr, nullptr, System::getSpritebatchMatrix());
	
	System::getFontComicSans()->DrawString(System::getSpriteBatch(), "Options", DirectX::SimpleMath::Vector2(200, 200), DirectX::Colors::Black, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2::One * 3);
	this->sliderMusic->render(this->sliderMusic == this->selectedElement);
	this->sliderSound->render(this->sliderSound == this->selectedElement);
	this->checkBoxRumble1->render(this->checkBoxRumble1 == this->selectedElement);
	this->checkBoxRumble2->render(this->checkBoxRumble2 == this->selectedElement);
	this->checkBoxRumble3->render(this->checkBoxRumble3 == this->selectedElement);
	this->checkBoxRumble4->render(this->checkBoxRumble4 == this->selectedElement);

	System::getSpriteBatch()->End();

	return true;
}
