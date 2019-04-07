/*
  ==============================================================================

    MenuBar.cpp
    Created: 5 Apr 2019 7:22:36pm
    Author:  danre

  ==============================================================================
*/

#include "MenuBar.h"

	BurgerMenuHeader::BurgerMenuHeader(SidePanel& sp)
		: sidePanel(sp)
	{
		static const unsigned char burgerMenuPathData[]
			= { 110,109,0,0,128,64,0,0,32,65,108,0,0,224,65,0,0,32,65,98,254,212,232,65,0,0,32,65,0,0,240,65,252,
				169,17,65,0,0,240,65,0,0,0,65,98,0,0,240,65,8,172,220,64,254,212,232,65,0,0,192,64,0,0,224,65,0,0,
				192,64,108,0,0,128,64,0,0,192,64,98,16,88,57,64,0,0,192,64,0,0,0,64,8,172,220,64,0,0,0,64,0,0,0,65,
				98,0,0,0,64,252,169,17,65,16,88,57,64,0,0,32,65,0,0,128,64,0,0,32,65,99,109,0,0,224,65,0,0,96,65,108,
				0,0,128,64,0,0,96,65,98,16,88,57,64,0,0,96,65,0,0,0,64,4,86,110,65,0,0,0,64,0,0,128,65,98,0,0,0,64,
				254,212,136,65,16,88,57,64,0,0,144,65,0,0,128,64,0,0,144,65,108,0,0,224,65,0,0,144,65,98,254,212,232,
				65,0,0,144,65,0,0,240,65,254,212,136,65,0,0,240,65,0,0,128,65,98,0,0,240,65,4,86,110,65,254,212,232,
				65,0,0,96,65,0,0,224,65,0,0,96,65,99,109,0,0,224,65,0,0,176,65,108,0,0,128,64,0,0,176,65,98,16,88,57,
				64,0,0,176,65,0,0,0,64,2,43,183,65,0,0,0,64,0,0,192,65,98,0,0,0,64,254,212,200,65,16,88,57,64,0,0,208,
				65,0,0,128,64,0,0,208,65,108,0,0,224,65,0,0,208,65,98,254,212,232,65,0,0,208,65,0,0,240,65,254,212,
				200,65,0,0,240,65,0,0,192,65,98,0,0,240,65,2,43,183,65,254,212,232,65,0,0,176,65,0,0,224,65,0,0,176,
				65,99,101,0,0 };

		Path p;
		p.loadPathFromData(burgerMenuPathData, sizeof(burgerMenuPathData));
		burgerButton.setShape(p, true, true, false);

		burgerButton.onClick = [this] { showOrHide(); };
		addAndMakeVisible(burgerButton);
	}

	BurgerMenuHeader::~BurgerMenuHeader()
	{
		sidePanel.showOrHide(false);
	}

	void BurgerMenuHeader::paint(Graphics& g)
	{
		auto titleBarBackgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId)
			.darker();

		g.setColour(titleBarBackgroundColour);
		g.fillRect(getLocalBounds());
	}

	void BurgerMenuHeader::resized()
	{
		auto r = getLocalBounds();

		burgerButton.setBounds(r.removeFromRight(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()).withSizeKeepingCentre(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()/2, LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()/2));

		titleLabel.setFont(Font(getHeight() * 0.5f, Font::plain));
		titleLabel.setBounds(r);
	}

	void BurgerMenuHeader::showOrHide()
	{
		sidePanel.showOrHide(!sidePanel.isPanelShowing());
	}


MenuBar::MenuBar()
{
	menuBar.reset(new MenuBarComponent(this));
	addAndMakeVisible(menuBar.get());
	setApplicationCommandManagerToWatch(&commandManager);
	commandManager.registerAllCommandsForTarget(this);
	addKeyListener(commandManager.getKeyMappings());
	addChildComponent(menuHeader);
	addAndMakeVisible(outerCommandTarget);
	addAndMakeVisible(sidePanel);

	if (menuBarPosition != MenuBarPosition::burger)
		sidePanel.showOrHide(false);

	menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
	burgerMenu.setModel(menuBarPosition == MenuBarPosition::burger ? this : nullptr);
	menuHeader.setVisible(menuBarPosition == MenuBarPosition::burger);

	sidePanel.setContent(menuBarPosition == MenuBarPosition::burger ? &burgerMenu : nullptr, false);
	menuItemsChanged();
	//setSize(500, 500);
}

MenuBar::~MenuBar()
{
}

void MenuBar::resized()
{
	if (menuBarPosition == MenuBarPosition::window)
	{
		menuBar->setBounds(getParentComponent()->getX(), getParentComponent()->getY(), getParentWidth(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
	}
	else
	{
		menuHeader.setBounds(getParentComponent()->getX(), getParentComponent()->getY(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
	}
}

StringArray MenuBar::getMenuBarNames()
{
	return { "Menu Position", "Outer Colour", "Inner Colour", "", "Themes", "Tooltips", "Test" };
}

PopupMenu MenuBar::getMenuForIndex(int menuIndex, const String& /*menuName*/)
{
	PopupMenu menu;
	if (menuIndex == 0)
	{
		menu.addCommandItem(&commandManager, CommandIDs::menuPositionInsideWindow);
		menu.addCommandItem(&commandManager, CommandIDs::menuPositionBurgerMenu);
	}
	else if (menuIndex == 1)
	{
		menu.addCommandItem(&commandManager, CommandIDs::outerColourRed);
		menu.addCommandItem(&commandManager, CommandIDs::outerColourGreen);
		menu.addCommandItem(&commandManager, CommandIDs::outerColourBlue);
	}
	else if (menuIndex == 2)
	{
		menu.addCommandItem(&commandManager, CommandIDs::innerColourRed);
		menu.addCommandItem(&commandManager, CommandIDs::innerColourGreen);
		menu.addCommandItem(&commandManager, CommandIDs::innerColourBlue);
	}
	else if (menuIndex == 3)
	{
		menu.addCommandItem(&commandManager, CommandIDs::DefaultTheme);
		menu.addCommandItem(&commandManager, CommandIDs::TestTheme);
	}
	else if (menuIndex == 4)
	{
		menu.addCommandItem(&commandManager, CommandIDs::ToolTips);
	}
	else if (menuIndex == 5)
	{
		menu.addCommandItem(&commandManager, CommandIDs::test1);
		menu.addCommandItem(&commandManager, CommandIDs::test2);

	}

	return menu;
}

void MenuBar::menuItemSelected(int, int) 
{

}

ApplicationCommandTarget * MenuBar::getNextCommandTarget()
{
	return &outerCommandTarget;
	//return nullptr;
}

void MenuBar::getCommandInfo(CommandID _commandID, ApplicationCommandInfo & _result)
{
	switch (_commandID)
	{
	case CommandIDs::menuPositionInsideWindow:
		_result.setInfo("Inside Window", "Places the menu bar inside the application window", "Menu", 0);
		_result.setTicked(menuBarPosition == MenuBarPosition::window);
		_result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
		break;
	case CommandIDs::menuPositionGlobalMenuBar:
		_result.setInfo("Global", "Uses a global menu bar", "Menu", 0);
		_result.setTicked(menuBarPosition == MenuBarPosition::global);
		_result.addDefaultKeypress('g', ModifierKeys::shiftModifier);
		break;
	case CommandIDs::menuPositionBurgerMenu:
		_result.setInfo("Burger Menu", "Uses a burger menu", "Menu", 0);
		_result.setTicked(menuBarPosition == MenuBarPosition::burger);
		_result.addDefaultKeypress('b', ModifierKeys::shiftModifier);
		break;
	//case CommandIDs::DefaultTheme:
	//	_result.setInfo("Theme Default", "Set theme to the default theme", "Themes", 0);
	//	break;
	//case CommandIDs::TestTheme:
	//	_result.setInfo("Theme Test", "Set theme to the test theme", "Themes", 0);
	//	break;
	default:
		break;
	}
	
}

void MenuBar::getAllCommands(Array<CommandID>& c)
{
	Array<CommandID> commands{ CommandIDs::menuPositionInsideWindow,
							CommandIDs::menuPositionGlobalMenuBar,
							CommandIDs::menuPositionBurgerMenu };
	/*Array<CommandID> commands{ CommandIDs::DefaultTheme,
							   CommandIDs::TestTheme }*/;
	c.addArray(commands);
}

bool MenuBar::perform(const InvocationInfo & info)
{
	switch (info.commandID)
	{
	case CommandIDs::menuPositionInsideWindow:
		setMenuBarPosition(MenuBarPosition::window);
		break;
	case CommandIDs::menuPositionGlobalMenuBar:
		setMenuBarPosition(MenuBarPosition::global);
		break;
	case CommandIDs::menuPositionBurgerMenu:
		setMenuBarPosition(MenuBarPosition::burger);
		break;
	//case CommandIDs::DefaultTheme:
	//	break;
	//case CommandIDs::TestTheme:
	//	break;
	default:
		return false;
	}

	return true;
}

void MenuBar::setMenuBarPosition(MenuBarPosition newPosition)
{
	if (menuBarPosition != newPosition)
	{
		menuBarPosition = newPosition;

		if (menuBarPosition != MenuBarPosition::burger)
			sidePanel.showOrHide(false);

		menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
		burgerMenu.setModel(menuBarPosition == MenuBarPosition::burger ? this : nullptr);
		menuHeader.setVisible(menuBarPosition == MenuBarPosition::burger);

		sidePanel.setContent(menuBarPosition == MenuBarPosition::burger ? &burgerMenu : nullptr, false);
		menuItemsChanged();

		resized();
	}
}
