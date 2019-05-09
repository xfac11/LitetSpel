#include "MainMenu.h"
#include "MainGui.h"
#include "OptionsGui.h"
#include "RulesGui.h"
#include "SelectGui.h"
#include"System.h"
MainMenu::MainMenu()
{
	this->main = nullptr;
	this->options = nullptr;
	this->rules = nullptr;
	this->select = nullptr;
}

MainMenu::~MainMenu()
{
	delete this->main;
	delete this->options;
	delete this->rules;
	delete this->select;
}

bool MainMenu::initailize()
{
	this->main = new MainGui(this);
	this->options = new OptionsGui(this);
	this->rules = new RulesGui(this);
	this->select = new SelectGui(this);

	this->gui = main;
	this->gui->initialize();

	return true;
}

bool MainMenu::render()
{
	this->renderImgui();
	this->gui->render();

	return true;
}

void MainMenu::renderImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	std::string textUse;
	ImGui::Begin("Main menu");


	//ImGui::EndChild();
	ImGui::CaptureKeyboardFromApp(true);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

bool MainMenu::update(float deltaTime)
{
	this->gui->update(deltaTime);
	return true;
}

void MainMenu::shutDown()
{
	delete this->main;
	delete this->options;
	delete this->rules;
	delete this->select;
}

void MainMenu::setCurrentMenu(MainMenuGui menu, bool alwaysActivateDelay)
{
	GuiBase* oldGui = this->gui;

	switch (menu)
	{
	case MAIN:
		this->gui = main;
		break;
	case OPTIONS:
		this->gui = options;
		break;
	case RULES:
		this->gui = rules;
		break;
	case SELECT:
		this->gui = select;
		break;
	}

	if (this->gui != oldGui)
	{
		oldGui->shutDown();

		this->gui->initialize();
		this->gui->activateDelay();
	}
	else if (alwaysActivateDelay)
	{
		this->gui->activateDelay();
	}
}
