/*
  ==============================================================================

    MenuBar.h
    Created: 5 Apr 2019 7:22:36pm
    Author:  Daniel Reiter
	Description: Menu Bar Component handles menu bar components for GUI

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MenuBar : public Component, public ApplicationCommandTarget, public MenuBarModel
{

	public:

		enum CommandIDs
		{
			DefaultTheme = 1,
			TestTheme,
			ToolTips
		};

		enum class MenuBarPosition
		{
			window,
			global,
			burger
		};

		MenuBar();
		~MenuBar();

		void resized() override;
		StringArray getMenuBarNames() override;
		void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
		ApplicationCommandTarget* getNextCommandTarget() override;
		void getCommandInfo(CommandID _commandID, ApplicationCommandInfo& _result) override;
		bool perform(const InvocationInfo& info) override;
		void setMenuBarPosition();


	private:
		ApplicationCommandManager commandListener;
		std::unique_ptr<MenuBarComponent> menuBar;
		MenuBarPosition menuBarPosition = MenuBarPosition::window;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};