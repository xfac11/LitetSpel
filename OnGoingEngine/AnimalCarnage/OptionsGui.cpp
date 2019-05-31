#include "OptionsGui.h"
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

std::string OptionsGui::getResolutionText(Resolution res)
{
	switch (res)
	{
	case _3840x2160:
		return "3840x2160 16:9";
	case _2560x1440:
		return "2560x1440 16:9";
	case _1920x1080:
		return "1920x1080 16:9";
	case _1280x720:
		return "1280x720 16:9";
	case _854x480:
		return "854x480 16:9";
	case _640x360:
		return "640x360 16:9";
	default:
		return "Default 16:9";
	}
}

void OptionsGui::setResolution()
{
	System::resolution = this->resolution;
	
	switch (this->resolution)
	{
	case _3840x2160:
		System::resizeWindow(3840, 2160);
		break;
	case _2560x1440:
		System::resizeWindow(2560, 1440);
		break;
	case _1920x1080:
		System::resizeWindow(1920, 1080);
		break;
	case _1280x720:
		System::resizeWindow(1280, 720);
		break;
	case _854x480:
		System::resizeWindow(854, 480);
		break;
	case _640x360:
		System::resizeWindow(640, 360);
		break;
	default:
		System::resizeWindow(1280, 720);
		break;
	}
}

void OptionsGui::changeResolution()
{
	int id = static_cast<int>(this->resolution) + 1;

	if (id >= 7)
	{
		id = 0;
	}

	this->resolution = static_cast<Resolution>(id);
}

OptionsGui::OptionsGui(State * myState) : GuiBase(myState)
{
	this->selectedElement = nullptr;

	this->sliderMusic = nullptr;
	this->buttonConfirm = nullptr;
	this->buttonResolution = nullptr;
	this->checkBoxRumble1 = nullptr;
	this->checkBoxRumble2 = nullptr;
	this->checkBoxRumble3 = nullptr;
	this->checkBoxRumble4 = nullptr;

	this->sliderSelected = false;
	this->pressedLastFrame = false;
	this->changedLastFrame = false;
	this->timeSinceChanged = 0.0f;
}

OptionsGui::~OptionsGui()
{
	delete this->sliderMusic;
	delete this->buttonConfirm;
	delete this->buttonResolution;
	delete this->checkBoxRumble1;
	delete this->checkBoxRumble2;
	delete this->checkBoxRumble3;
	delete this->checkBoxRumble4;
}

bool OptionsGui::initialize()
{
	this->resolution = System::resolution;

	bool rumble[4];
	dynamic_cast<MainMenu*>(this->myState)->getRumble(rumble);

	this->sliderMusic = new Slider(100, 100, 5, SimpleMath::Vector2(1920 / 2.0F - 610 / 2, 1080 / 2.0F - 200));
	this->buttonConfirm = new Button("Apply", SimpleMath::Vector2(1920 / 2.0F + 10, 1080 / 2.0F - 50));
	this->buttonResolution = new Button(this->getResolutionText(System::resolution), SimpleMath::Vector2(1920 / 2.0F - 610, 1080 / 2.0F - 50));
	this->checkBoxRumble1 = new CheckBox(rumble[0], SimpleMath::Vector2(1920 / 2.0F - 350, 1080 / 2.0F + 100));
	this->checkBoxRumble2 = new CheckBox(rumble[1], SimpleMath::Vector2(1920 / 2.0F - 150, 1080 / 2.0F + 100));
	this->checkBoxRumble3 = new CheckBox(rumble[2], SimpleMath::Vector2(1920 / 2.0F + 50, 1080 / 2.0F + 100));
	this->checkBoxRumble4 = new CheckBox(rumble[3], SimpleMath::Vector2(1920 / 2.0F + 250, 1080 / 2.0F + 100));

	this->selectedElement = this->sliderMusic;
	this->sliderMusic->setConnectedElements(nullptr, nullptr, checkBoxRumble1, buttonResolution);
	this->buttonConfirm->setConnectedElements(buttonResolution, buttonResolution, sliderMusic, checkBoxRumble3);
	this->buttonResolution->setConnectedElements(buttonConfirm, buttonConfirm, sliderMusic, checkBoxRumble1);
	this->checkBoxRumble1->setConnectedElements(checkBoxRumble4, checkBoxRumble2, buttonResolution, sliderMusic);
	this->checkBoxRumble2->setConnectedElements(checkBoxRumble1, checkBoxRumble3, buttonResolution, sliderMusic);
	this->checkBoxRumble3->setConnectedElements(checkBoxRumble2, checkBoxRumble4, buttonConfirm, sliderMusic);
	this->checkBoxRumble4->setConnectedElements(checkBoxRumble3, checkBoxRumble1, buttonConfirm, sliderMusic);

	this->sliderMusic->setValue(static_cast<int>(System::getSoundManager()->getMasterVolume() * this->sliderMusic->getMaxValue()));

	return true;
}

void OptionsGui::shutDown()
{
	delete this->sliderMusic;
	delete this->buttonConfirm;
	delete this->buttonResolution;
	delete this->checkBoxRumble1;
	delete this->checkBoxRumble2;
	delete this->checkBoxRumble3;
	delete this->checkBoxRumble4;

	this->selectedElement = nullptr;

	this->sliderMusic = nullptr;
	this->buttonConfirm = nullptr;
	this->buttonResolution = nullptr;
	this->checkBoxRumble1 = nullptr;
	this->checkBoxRumble2 = nullptr;
	this->checkBoxRumble3 = nullptr;
	this->checkBoxRumble4 = nullptr;

	this->sliderSelected = false;
	this->pressedLastFrame = false;
	this->changedLastFrame = false;
	this->timeSinceChanged = 0.0f;
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
				
				if (this->sliderSelected)
				{
					if (System::theKeyboard->KeyIsPressed('A'))
					{
						this->sliderMusic->changeValueWithStep(true);
					}
					else if (System::theKeyboard->KeyIsPressed('D'))
					{
						this->sliderMusic->changeValueWithStep(false);
					}

					System::getSoundManager()->setMasterVolume(this->sliderMusic->getValue() / static_cast<float>(this->sliderMusic->getMaxValue()));
				}
				else
				{
					this->changeSelected_Keyboard();
				}
			}

			this->timeSinceChanged += deltaTime;
		}
		else
		{
			this->timeSinceChanged = 0.0F;

			if (this->sliderSelected)
			{
				if (System::theKeyboard->KeyIsPressed('A'))
				{
					this->sliderMusic->changeValueWithStep(true);
				}
				else if (System::theKeyboard->KeyIsPressed('D'))
				{
					this->sliderMusic->changeValueWithStep(false);
				}

				System::getSoundManager()->setMasterVolume(this->sliderMusic->getValue() / static_cast<float>(this->sliderMusic->getMaxValue()));
			}
			else
			{
				this->changeSelected_Keyboard();
			}
		}

		if (System::theKeyboard->KeyIsPressed('E'))
		{
			if (!this->pressedLastFrame)
			{
				if (this->sliderSelected)
				{
					this->sliderSelected = false;
				}
				else
				{
					if (this->selectedElement == this->sliderMusic)
					{
						this->sliderSelected = true;
					}
					else if (this->selectedElement == this->buttonConfirm)
					{
						this->setResolution();
					}
					else if (this->selectedElement == this->buttonResolution)
					{
						this->changeResolution();
						this->buttonResolution->setText(this->getResolutionText(this->resolution));
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

			if (this->sliderSelected)
			{
				if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
				{
					this->sliderSelected = false;
				}
				else if (System::theTracker->dpadLeft == DirectX::GamePad::ButtonStateTracker::PRESSED)
				{
					this->sliderMusic->changeValueWithStep(true);
				}
				else if (System::theTracker->dpadRight == DirectX::GamePad::ButtonStateTracker::PRESSED)
				{
					this->sliderMusic->changeValueWithStep(false);
				}

				System::getSoundManager()->setMasterVolume(this->sliderMusic->getValue() / static_cast<float>(this->sliderMusic->getMaxValue()));
			}
			else
			{
				this->changeSelected();

				if (System::theTracker->a == DirectX::GamePad::ButtonStateTracker::PRESSED)
				{
					if (this->selectedElement == this->sliderMusic)
					{
						this->sliderSelected = true;
					}
					else if (this->selectedElement == this->buttonConfirm)
					{
						this->setResolution();
					}
					else if (this->selectedElement == this->buttonResolution)
					{
						this->changeResolution();
						this->buttonResolution->setText(this->getResolutionText(this->resolution));
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
					bool rumble[4] = {
						this->checkBoxRumble1->isChecked(),
						this->checkBoxRumble2->isChecked(),
						this->checkBoxRumble3->isChecked(),
						this->checkBoxRumble4->isChecked()
					};
					state->setRumble(rumble);
					state->setCurrentMenu(MAIN);
				}
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
	this->buttonConfirm->render(this->buttonConfirm == this->selectedElement);
	this->buttonResolution->render(this->buttonResolution == this->selectedElement);
	this->checkBoxRumble1->render(this->checkBoxRumble1 == this->selectedElement);
	this->checkBoxRumble2->render(this->checkBoxRumble2 == this->selectedElement);
	this->checkBoxRumble3->render(this->checkBoxRumble3 == this->selectedElement);
	this->checkBoxRumble4->render(this->checkBoxRumble4 == this->selectedElement);

	System::getSpriteBatch()->End();

	return true;
}
