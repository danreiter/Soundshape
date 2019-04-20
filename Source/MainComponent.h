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
#include "IOHandler.h"
#include "keyboardPopup.h"
#include "otherPopups.h"

#define WRITE_BUTTON 2008  ///< Component ID number for the write button

//==============================================================================
/**
Struct for burger Menu component.

@see
JUCE documentation: BurgerMenu
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
/**
This the main GUI component and is instantiated by the plugin editor on start up. All GUI components are a child of this components.
This Component passes event listener to child componets, paints and repaint the user interface, 
and handles load and save location protocols.
	
Uses a reference to Soundshapes's plugin audio processor to call a reference to the Soundshape's converter to signal and retreive state changes to a sound.
Passes refernences of the a AudioProcessorValueTreeState to components in order to handle changes to Soundshape's sound rendering settings (settings such as envelope and volume setting changes).
Uses two separate time domain references for a zoomed in view and zoomed out view and a single frequency domain view. 
*/
//==============================================================================
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
	/**
	Command Ids for menu options
	*/
	//==============================================================================
	enum CommandIDs
	{
		menuPositionInsideWindow = 1,	/**< Menu CommandID = 1 Drop down window position. */
		menuPositionBurgerMenu,			/**< Menu CommandID = 2 Burger menu postion. */
		DefaultTheme,					/**< Menu CommandID = 3 Theme 1. */
		TestTheme,						/**< Menu CommandID = 4 Theme 2. */
		SchoolTheme,					/**< Menu CommandID = 5 Theme 3. */
		Keyboard,						/**< Menu CommandID = 6 Toggle MIDI keyboard. */
		PresetPath,						/**< Menu CommandID = 7 Set path for saves/Load directory. */
		ToolTips,						/**< Menu CommandID = 8 Toggle tool Tips. */
		Tutorial,						/**< Menu CommandID = 9 Open Tutorial window. */
		Developers,						/**< Menu CommandID = 10 Open window about Soundshape. */
		Licence							/**< Menu CommandID = 11 Open window with licence. */
	};

	/**
	Command Ids for menu types
	*/
	enum class MenuBarPosition
	{
		window,	/**< Menu Bar Postion setting = 1 Drop down window position. */
		burger	/**< Menu Bar Postion setting = 2 Burger menu position. */
	};

	//==============================================================================
	/**
	Paints mainComponent and mainComponent's sub-components.

	Can be recalled using the function call repaint(). See JUCE documentation.
	@see
	JUCE documentation: Component::paint(), Component::repaint()

	@param g Graphical context object
	*/
	void paint(Graphics& g) override;

	/**
	Repaints mainComponent and mainComponent's sub-components on widow resize.

	@see
	JUCE documentation: Component resized().
	*/

	void resized() override;
	/**
	Event listener lisetens for changes slider sub-components.

	@see 
	JUCE documentation: Slider::Listener sliderValueChanged().
	@param slider refenece to the slider that's value changed.
	*/
	void sliderValueChanged(Slider* slider) override;
	/**
	Event listener lisetens for changes button sub-components.

	This will trigger on events including button hover over as well as button clicks.
	@see
	JUCE documentation:
		Button::Listener ButtonStateChanged()

	@param button Refenece to the button that's state changed.
	*/
    void buttonStateChanged(Button* button) override;

	/**
	Renders soundwave path of the current state of the sound.

	Used be the editor to update the time domain sound wave after changes to a sounds state.
	@see Soundshape_pluginAudioProcessor::setStateInformation ()
	*/
    void drawSound();
	/**
	Event listener lisetens for clicked event of button sub-components.

	@see 
	JUCE documentation: Button::Listener ButtonClicked()
	@param button refenece to the button that was clicked.
	*/
	void buttonClicked(Button* button) override;
	/**
	Sets the mainComponent's converter pointer.

	@param _converter reference to a converter from SoundShape's audioProcessor.
	*/
	void setConverter(Converter* _converter);
	/**
	Sets default variable settings for Soundshape's time and frequency domains and passes references of variables to sub0components.

	Sets the profile to the sound profile to first profile in a ten second block.
	@see FeqWindow::setZoom()
	@see smallTime::setTimeDomain()
	@see bigTime::setProfile()
	@see GuiFunc::setListeners()
	@see fundFreq::setListener()
	@see envelope::setListener()
	*/
	void loadSound();
	/**
	Sets the styles and colors for all of Soundshapes GUI components.

	LookAndFeel are set using user selected theme.

	<br>Note: LookAndFeel propogate to sub-components.</br>
	@see 
	JUCE documentation: CustomLookAndFeel

	@param laf Reference to a LookAndFeel object to change to.
	@param comp Reference to component to change the LookAndFeel of. 
	*/
	void setAllLookAndFeels(LookAndFeel* laf, Component* comp);
	/**
	Set the title of each optin in the menu bar.

	@see Juce documentation: MenuBarModel::getMenuBarNames()
	*/
	StringArray getMenuBarNames() override;
	/**
	Get the index of a selected menu index and sets sub menu options to the names and CommandIDs with the commandManger.

	@see 
	JUCE documentation: MenuBarModel::getMenuForIndex()
	@param menuIndex Currently selected menu option. 
	*/
	PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override;
	/**
	@see 
	JUCE documention: MenuBarModel::menuItemSelected()
	*/
	void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
	/**
	Used for selecting menu options returns the Command target of the first parent component.

	@see 
	JUCE documentation: ApplicationCommandTarget::getNextCommandTarget()
	*/
	ApplicationCommandTarget* getNextCommandTarget() override;
	/**
	Get the labels and states of the sub-menu options.

	@see 
	JUCE documentation: ApplicationCommandTarget::getCommandInfo()
	*/
	void getCommandInfo(CommandID _commandID, ApplicationCommandInfo& _result) override;
	/**
	Get a list of all Application Commands for the menu options.

	@see 
	JUCE documetation: ApplicationCommandTarget::getAllCommands()
	*/
	void getAllCommands(Array<CommandID>& c) override;
	/**
	Performs actions per menu option selected.

	Invokes action selected command option per menu selection uses CommandIDs as selection.

	@see 
	JUCE documentation ApplicationCommandTarget::perform().
	@param info Selected CommandID 
	*/
	bool perform(const InvocationInfo& info) override;
	/**
	Set the menu bar postion to either a drop down menu or a burger menu.

	@see perform()
	@param newPosition The selected menu bar position setting.
	*/
	void setMenuBarPosition(MenuBarPosition newPosition);
	/**
	Set the mainComponent's theme per menu bar selection.

	@see perform()
	@param newTheme The selected theme to change to.
	*/
	void setTheme(CommandID newTheme);
	/**
	Sets the Save and Load directory of the preset path. 

	Opens a dialog box for users to choose a directory. Prompts the user to save the current sound before changing 
	directory, loads the Soundshape files to the presets and make the current sound profile a new sound.
	<br>Warning: If used with a DAW the presetPath will not match a path set by the DAW is for use with stand alone mode.</br>

	@see perfom() 
	@see saveFilePrompt()
	@see loadPresetPath()
	@see JUCE documentation: FileChooser::browseForDirectory()
	*/
	void setPresetPath();
	/**
	Loads a file names from a selected path into the preset options.
	*/
	void loadPresetPath();
	/**
	Shows a popup window prompting the user to select to save or not.
	*/
	void saveFilePrompt();
	/**
	Shows a popup window with save options.
	
	Prompts the user to select a saveAs option (save current sound as a new file) or a save option (save sound to currently selected file).

	@see save()
	@see saveAs()
	*/
	void promptSaveOptions();
	/**
	Event listener function that triggers on the event that a comobox sub-component changes.

	@see
	JUCE documentation: ComboBox::Listener
	@param comoBoxThatHasChanged Reference to the comboBox component that changed.
	*/
	void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
	/**
	Overwrites selected preset file with current Soundshape state.

	@see IOHandler::createStateDocument()
	@see IOHandler::writeStateXMLFile()
	*/
	bool save();
	/**
	Writes current Soundshape state to a new state.

	Opens a dialog box to select file to save.
	@see IOHandler::createStateDocument()
	@see IOHandler::writeStateXMLFile()
	@see 
	JUCE documentation: FileChooser
	*/
	bool saveAs();
	/**
	Loads a new Soundshape state (sound profile) from a file. 

	On load all time and frequency profiles are set to the first profile sound.

	@see IOHandler::restoreStateFromXml()
	@see Soundshape_PluginAudioProcessor::restoreNewSoundParams()
	@see Soundshape_PluginAudioProcessor::restoreDefaultProfile()
	@see FeqWindow::setProfile()
	*/
	void loadFile();
	/**
	Import a .ogg, .wav, or .flac file to a Soundshape sound profile.

	@see IOHandler::importConverterAudio()
	@see AlertWindow::showMessageBox()
	*/
	void importFile();
	/**
	Exports Soundshape sound profile to a .ogg, .wav, or .flac file.

	Popup window prompts user to select file type and then a dialog window asks user to select a file.
	The popup window file selection is done as a work around in JUCE's FileChooser class that does not filter
	multiple file type extesions of certain types.

	@see IOHandler::exportConverterAudio()
	@see AlertWindow::showMessageBox()
	*/
	void exportFile();
	/**
	Creates or destroys a popup window containing a MIDI keyboard.

	On creatation of keyboard a new keyboardPopup component is created and passed by reference to the midiKeyboard pointer variable.
	On destruction the midiKeyboard is deleted and set to zero.

	@see keyboardPopup::keyboardPopup()
	@param vis Boolean tells function to create a new keyboard on true and destroy an existing keyboard on false.
	*/
	void showKey(bool vis);
	/**
	Creates a document window containing Soundshapes licence information.
	*/
	void showLic();
	/**
	Creates a document window containing information about Soundshape.
	*/
	void showDevs();

	

private:
	//==============================================================================
	// Your private member variables go here...
	FeqWindow fWindow;  ///< Window displays the selected frequency profile element.
	smallTime sTWindow; ///< Window displays a zoomed in view of the time domain.
	bigTime bTWindow;	///< Window displays the time domain.
	fundFreq fund;		///< Component handles seeting for the fundmental frequency for redering the time domain.
	GuiFunc volComp;	///< Component handles volume settings.
	envelope enve;		///< Component handles Envelope settings.
    TooltipWindow ttp;  ///< Component handles tooltip messages.

	TextButton *harmonicButton;			///< Toggles harmonic constraint for frequency profile.
	TextButton *addButton;				///< Toggles buttons to add frequency spikes to frequency profile.
	Slider *zoomSlider;					///< Handles magnification settings for frequency profile window.
	ComboBox cb;						///< Toggles preset files for quick access.
	TextButton *writeButton;			///< Used to write current Soundshape state to a file.
	int harm;	                        ///< Used to track the state of the harmonic filter for add frequency spikes.
	int add;	                        ///< Used to track the state of the add buttons for adding frequency spikes.
	int timeSize;		                ///< The total time of all sound profiles defaults to 10 and is currently not optimized to change.
	int timeBlock, selectedProfile, currentProfile;		///< Used for tracking the current selected time and frequency profile
	double zoom;						///< Current degree of zoom of frequency profile
	
	Component::SafePointer<Component> midiKeyboard;  ///< Pointer to reference the MIDI keyboard popup window when in use.

    static float notes[12];				///< List for all notes in hz for fundmental frequency.
    float currentNoteFreq = 27.5;		///< Currently selected fundemental frequency note in hz.

    Converter *converterPtr;                      ///< Reference to Soundshapes converter, used to share information with the back-end.
    Soundshape_pluginAudioProcessor& processor;   ///< Reference to Soundshapes processor, used to get a reference to the converter, loading Soundshape files,and playing sounds using the GUI.
    AudioProcessorValueTreeState& valueTreeState; ///< Event listener for back-end parameters.

	CustomLookAndFeel * laf;  ///< Set and changes Soundshape's themes.

	ApplicationCommandManager commandManager;					///< Handles commands for the menu options.
	std::unique_ptr<MenuBarComponent> menuBar;					///< Menu bar drop down component.
	MenuBarPosition menuBarPosition = MenuBarPosition::window;	///< Stores Soundshape's current menu style.
	SidePanel sidePanel{ "Menu", 300, true };                   ///< Panel used for Soundshape's burger menu slide out.
	BurgerMenuComponent burgerMenu;								///< Burger menu component.
	BurgerMenuHeader menuHeader{ sidePanel };					///< Sets the burger menu button to show the sidePanel.
	CommandID currentTheme = CommandIDs::DefaultTheme;			///< Stores Soundshape's current theme.
	bool tips = true;											///< Boolean stores whether tool tips show or not.
	bool showKeyboard = false;									///< Boolean stores whether the popup MIDI keyboard is in use or not.

	//-----------File handling-----------------------------------------------
	File presetPath;											///< Stores the path of directory for the currently selected presets.
	File selectedFile;									        ///< Stores the file name of the currently selected preset file.
	const File newFile = File();								///< Null pointer file to compare with selectedFile to see if file is empty.
	bool pushedWriteBtn = true, newSave = false;				///< Boolen to single the state under which a save is being requested.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
//==============================================================================

