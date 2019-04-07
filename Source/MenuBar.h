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
struct BurgerMenuHeader : public Component
{
	BurgerMenuHeader(SidePanel& sp);

	~BurgerMenuHeader();

private:
	void paint(Graphics& g) override;
	void resized() override;
	void showOrHide();

	SidePanel& sidePanel;
	Label titleLabel{ "titleLabel", "JUCE Demo" };
	ShapeButton burgerButton{ "burgerButton", Colours::lightgrey, Colours::lightgrey, Colours::white };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BurgerMenuHeader)
};

class MenuBar : public Component, public ApplicationCommandTarget, public MenuBarModel
{

	public:

		enum CommandIDs
		{
			menuPositionInsideWindow = 1,
			menuPositionGlobalMenuBar,
			menuPositionBurgerMenu,
			outerColourRed,
			outerColourGreen,
			outerColourBlue,
			innerColourRed,
			innerColourGreen,
			innerColourBlue,
			DefaultTheme,
			TestTheme,
			ToolTips,
			test1, 
			test2
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
		PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override;
		void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
		ApplicationCommandTarget* getNextCommandTarget() override;
		void getCommandInfo(CommandID _commandID, ApplicationCommandInfo& _result) override;
		void getAllCommands(Array<CommandID>& c) override;
		bool perform(const InvocationInfo& info) override;
		void setMenuBarPosition(MenuBarPosition newPosition);


	private:
		ApplicationCommandManager commandManager;
		std::unique_ptr<MenuBarComponent> menuBar;
		MenuBarPosition menuBarPosition = MenuBarPosition::burger;

		SidePanel sidePanel{ "Menu", 150, true };

		BurgerMenuComponent burgerMenu;
		BurgerMenuHeader menuHeader{ sidePanel };

		//==============================================================================
	/**
		Command messages that aren't handled in the main component will be passed
		to this class to respond to.
	*/
		class OuterCommandTarget : public Component,
			public ApplicationCommandTarget
		{
		public:
			OuterCommandTarget(ApplicationCommandManager& m)
				: commandManager(m),
				innerCommandTarget(commandManager)
			{

				commandManager.registerAllCommandsForTarget(this);

				addAndMakeVisible(innerCommandTarget);
			}

			void resized() override
			{
				innerCommandTarget.setBounds(getLocalBounds().reduced(50));
			}

			void paint(Graphics& g) override
			{
				g.fillAll(currentColour);
			}

			//==============================================================================
			ApplicationCommandTarget* getNextCommandTarget() override
			{
				return &innerCommandTarget;
			}

			void getAllCommands(Array<CommandID>& c) override
			{
				Array<CommandID> commands{ CommandIDs::outerColourRed,
											CommandIDs::outerColourGreen,
											CommandIDs::outerColourBlue };

				c.addArray(commands);
			}

			void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override
			{
				switch (commandID)
				{
				case CommandIDs::outerColourRed:
					result.setInfo("Red", "Sets the outer colour to red", "Outer", 0);
					result.setTicked(currentColour == Colours::red);
					result.addDefaultKeypress('r', ModifierKeys::commandModifier);
					break;
				case CommandIDs::outerColourGreen:
					result.setInfo("Green", "Sets the outer colour to green", "Outer", 0);
					result.setTicked(currentColour == Colours::green);
					result.addDefaultKeypress('g', ModifierKeys::commandModifier);
					break;
				case CommandIDs::outerColourBlue:
					result.setInfo("Blue", "Sets the outer colour to blue", "Outer", 0);
					result.setTicked(currentColour == Colours::blue);
					result.addDefaultKeypress('b', ModifierKeys::commandModifier);
					break;
				default:
					break;
				}
			}

			bool perform(const InvocationInfo& info) override
			{
				switch (info.commandID)
				{
				case CommandIDs::outerColourRed:
					currentColour = Colours::red;
					break;
				case CommandIDs::outerColourGreen:
					currentColour = Colours::green;
					break;
				case CommandIDs::outerColourBlue:
					currentColour = Colours::blue;
					break;
				default:
					return false;
				}

				repaint();
				return true;
			}

		private:
			//==============================================================================
			/**
				Command messages that aren't handled in the OuterCommandTarget will be passed
				to this class to respond to.
			*/
			struct InnerCommandTarget : public Component,
				public ApplicationCommandTarget
			{
				InnerCommandTarget(ApplicationCommandManager& m)
					: commandManager(m)
				{
					commandManager.registerAllCommandsForTarget(this);
				}

				void paint(Graphics& g) override
				{
					g.fillAll(currentColour);
				}

				//==============================================================================
				ApplicationCommandTarget* getNextCommandTarget() override
				{
					// this will return the next parent component that is an ApplicationCommandTarget
					return findFirstTargetParentComponent();
				}

				void getAllCommands(Array<CommandID>& c) override
				{
					Array<CommandID> commands{ CommandIDs::innerColourRed,
												CommandIDs::innerColourGreen,
												CommandIDs::innerColourBlue };

					c.addArray(commands);
				}

				void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override
				{
					switch (commandID)
					{
					case CommandIDs::innerColourRed:
						result.setInfo("Red", "Sets the inner colour to red", "Inner", 0);
						result.setTicked(currentColour == Colours::red);
						result.addDefaultKeypress('r', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
						break;
					case CommandIDs::innerColourGreen:
						result.setInfo("Green", "Sets the inner colour to green", "Inner", 0);
						result.setTicked(currentColour == Colours::green);
						result.addDefaultKeypress('g', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
						break;
					case CommandIDs::innerColourBlue:
						result.setInfo("Blue", "Sets the inner colour to blue", "Inner", 0);
						result.setTicked(currentColour == Colours::blue);
						result.addDefaultKeypress('b', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
						break;
					default:
						break;
					}
				}

				bool perform(const InvocationInfo& info) override
				{
					switch (info.commandID)
					{
					case CommandIDs::innerColourRed:
						currentColour = Colours::red;
						break;
					case CommandIDs::innerColourGreen:
						currentColour = Colours::green;
						break;
					case CommandIDs::innerColourBlue:
						currentColour = Colours::blue;
						break;
					default:
						return false;
					}

					repaint();
					return true;
				}

				ApplicationCommandManager& commandManager;

				Colour currentColour{ Colours::blue };
			};

			ApplicationCommandManager& commandManager;
			InnerCommandTarget innerCommandTarget;

			Colour currentColour{ Colours::red };
		};

		OuterCommandTarget outerCommandTarget{ commandManager };

		//==============================================================================

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBar)
};