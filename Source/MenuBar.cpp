/*
  ==============================================================================

    MenuBar.cpp
    Created: 5 Apr 2019 7:22:36pm
    Author:  danre

  ==============================================================================
*/

#include "MenuBar.h"

MenuBar::MenuBar()
{
	//menuBar.reset(new BurgerMenuComponent(this));
	addAndMakeVisible(menuBar.get());
}

MenuBar::~MenuBar()
{
}

void MenuBar::resized()
{
}

StringArray MenuBar::getMenuBarNames()
{
	return StringArray();
}

void MenuBar::menuItemSelected(int, int)
{
}

ApplicationCommandTarget * MenuBar::getNextCommandTarget()
{
	return nullptr;
}

void MenuBar::getCommandInfo(CommandID _commandID, ApplicationCommandInfo & _result)
{
}

bool MenuBar::perform(const InvocationInfo & info)
{
	return false;
}

void MenuBar::setMenuBarPosition()
{
}
