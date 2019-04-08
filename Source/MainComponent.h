/*
  ==============================================================================

	MainComponent.h
	Created: 24 Dec 2018 12:44:39pm
	Author:  Daniel Reiter
	Description:Main Gui Component. Parent component for all Gui components. 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FeqWindow.h"
#include "smallTime.h"
#include "bigTime.h"
#include "GuiFunc.h"
#include "envelope.h"
#include "Converter.h"
#include "PluginProcessor.h"

//==============================================================================
/*
	Struct for burger Menu component
*/
//==============================================================================
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
//==============================================================================



//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public Component, 
					  public Slider::Listener, 
					  public Button::Listener, 
					  public ApplicationCommandTarget, 
					  public MenuBarModel,
					  public ComboBox::Listener
{
public:
	//==============================================================================
	MainComponent(Soundshape_pluginAudioProcessor& p, AudioProcessorValueTreeState& _valueTreeState);
	~MainComponent();

	//==============================================================================
	//  Command Id for menu options
	//==============================================================================
	enum CommandIDs
	{
		menuPositionInsideWindow = 1,
		menuPositionBurgerMenu,
		DefaultTheme,
		TestTheme,
		Keyboard,
		PresetPath,
		ToolTips,
		Tutorial,
		Developers,
		Licence
	};

	enum class MenuBarPosition
	{
		window,
		burger
	};

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void sliderValueChanged(Slider* slider) override;
	void buttonClicked(Button* button) override;
	void setConverter(Converter* _converter);
	void loadSound();
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override;
	void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
	ApplicationCommandTarget* getNextCommandTarget() override;
	void getCommandInfo(CommandID _commandID, ApplicationCommandInfo& _result) override;
	void getAllCommands(Array<CommandID>& c) override;
	bool perform(const InvocationInfo& info) override;
	void setMenuBarPosition(MenuBarPosition newPosition);
	void setTheme(CommandID newTheme);
	void setPresetPath();
	void loadPresetPath();
	void saveFilePrompt();
	void promptSaveOptions();
	void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
	bool save();
	bool saveAs();

private:
	//==============================================================================
	// Your private member variables go here...
	FeqWindow fWindow;  // Window displays the selected frequency profile element
	smallTime sTWindow; // Window displays a zoomed in view of the time domain 
	bigTime bTWindow;	// Window displays the time domain
	fundFreq fund;		// Component handles seeting for the fundmental frequency for redering the time domain
	GuiFunc volComp;	// Component handles volume settings
	envelope enve;		// Component handles Envelope settings
    TooltipWindow ttp;  // Component handles tooltip messages

	TextButton *harmonicButton;			// Toggles harmonic constraint for frequency profile
	TextButton *addButton;				// Toggles buttons to add frequency spikes to frequency profile
	Slider *zoomSlider;					// Handles magnification settings for frequency profile window
	ComboBox cb;						// Toggles files for quick access 
	float amp;							// Currently not used
	TextButton *writeButton;			// Currently not used
	int freq, harm, add, timeSize;		// Varibles for tracking software and gui state
	int timeBlock, selectedProfile, currentProfile;		// Varible for tracking current selected time and frequency
	double zoom;						// Varible for tracking current degree of zoom of frquency profile
	
	MidiKeyboardState keyboardState;           
	MidiKeyboardComponent keyboardComponent;

	// Varible for notes in hz
    static float notes[12];
    float currentNoteFreq = 27.5;

    Converter *converterPtr;
    Soundshape_pluginAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState; // back-end parameters

	ApplicationCommandManager commandManager;
	std::unique_ptr<MenuBarComponent> menuBar;
	MenuBarPosition menuBarPosition = MenuBarPosition::burger;

	SidePanel sidePanel{ "Menu", 150, true };

	BurgerMenuComponent burgerMenu;
	BurgerMenuHeader menuHeader{ sidePanel };
	CommandID currentTheme = CommandIDs::DefaultTheme;
	bool tips = true;
	bool showKeyboard = false;

	//-----------File handling-----------------------------------------------
	File presetPath;
	File selectedFile;
	const File newFile = File();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
//==============================================================================

