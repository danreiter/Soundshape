#include "MainComponent.h"

float MainComponent::notes[12] = { 27.5f, 29.50f, 30.87f, 16.35f, 17.32f, 18.35f, 19.45f, 20.60f, 21.83f, 23.12f, 24.5f, 25.96 };

//==============================================================================
//  Component declares and instaites other gui components and passes variables from 
//==============================================================================
MainComponent::MainComponent(Soundshape_pluginAudioProcessor& p, AudioProcessorValueTreeState& _valueTreeState) :
	processor(p),
	enve(_valueTreeState),
	valueTreeState(_valueTreeState),
	volComp(_valueTreeState),
	keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
	//----------Setting reference to the converter----------------------------------
    setConverter(&(processor.getConverter()));
	//------------------------------------------------------------

    //----------Default settings----------------------------------
	amp = 0.0f;
	freq = 0;
	add = -1;
	harm = -1;
	zoom = 4.0;
	//------------------------------------------------------------


	//------Passing references to child components----------------
	loadSound();

	//------------------------------------------------------------

	addAndMakeVisible(fWindow);
	addAndMakeVisible(sTWindow);
	addAndMakeVisible(bTWindow);
	addAndMakeVisible(fund);
	addAndMakeVisible(volComp);
	addAndMakeVisible(enve);
	addAndMakeVisible(cb);

	//-----Setting testing values for the combo box-------------

	cb.addItem("Gregory's game tune", 1);
	cb.addItem("Daniel's groovy sound", 2);
	cb.addItem("Mardigon's math sound", 3);
	cb.addItem("My rad sound", 4);
	cb.addItem("The sound of angry paper", 5);
	cb.addItem("Love sound", 6);
	cb.addItem("The sound of not silence", 7);
	cb.setSelectedItemIndex(3);
	cb.setColour(ComboBox::backgroundColourId, Colours::peachpuff);
	cb.setColour(ComboBox::arrowColourId, Colours::black);
	cb.setColour(ComboBox::textColourId, Colours::black);
    cb.setTooltip("Contains all the current presets");

	//------------------------------------------------------------

	//------------------------------------------------------------

	//------------Setting Button Values---------------------------

	// Harmonic button to toggle harmonic filter for selecting 
	harmonicButton = new TextButton("Harmonic");
	harmonicButton->setClickingTogglesState(true);
	harmonicButton->setColour(TextButton::textColourOffId, Colours::white);
	harmonicButton->setColour(TextButton::textColourOnId, Colours::white);
	harmonicButton->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	harmonicButton->setColour(TextButton::buttonOnColourId, Colours::orange);
    harmonicButton->setTooltip("When enabled, the harmonic setting will only allow the creation of new frequency spikes at harmonically correct frequencies");
	harmonicButton->onClick = [this]
	{
		harm = -1 * (harm);
		repaint();
        
	};

	// Button to add friquency spike to a frequency profile
	addButton = new TextButton("Add");
	addButton->setClickingTogglesState(true);
	addButton->setColour(TextButton::textColourOffId, Colours::white);
	addButton->setColour(TextButton::textColourOnId, Colours::white);
	addButton->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	addButton->setColour(TextButton::buttonOnColourId, Colours::orange);
	addButton->addListener(this);
    addButton->setTooltip("Allows the creation of a frequency spike at any available frequency");
	addButton->onClick = [this]
	{
        
		add = -1 * (add);
		repaint();
	};

	// Write button to save sound profiles to the preset folder
	writeButton = new TextButton("Write");
    writeButton->setTooltip("Writes the chosen preset");
	zoomSlider = new Slider(Slider::IncDecButtons, Slider::TextBoxAbove);
	zoomSlider->setColour(Slider::textBoxBackgroundColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	zoomSlider->setRange(1.0f, 40.0f, .5);
	zoomSlider->setValue(zoom, sendNotificationAsync);
	zoomSlider->setTextValueSuffix(" X");
	zoomSlider->addListener(this);

	//------------------------------------------------------------

	addAndMakeVisible(writeButton);
	addAndMakeVisible(harmonicButton);
	addAndMakeVisible(addButton);
	addAndMakeVisible(zoomSlider);
	addAndMakeVisible(keyboardComponent);
	keyboardState.addListener(&keyboardComponent);

	//----------Menu settings----------------------------------
	menuBar.reset(new MenuBarComponent(this));
	addAndMakeVisible(menuBar.get());
	setApplicationCommandManagerToWatch(&commandManager);
	commandManager.registerAllCommandsForTarget(this);
	addKeyListener(commandManager.getKeyMappings());
	addChildComponent(menuHeader);

	//addAndMakeVisible(outerCommandTarget);
	addAndMakeVisible(sidePanel);

	if (menuBarPosition != MenuBarPosition::burger)
		sidePanel.showOrHide(false);

	menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
	burgerMenu.setModel(menuBarPosition == MenuBarPosition::burger ? this : nullptr);
	menuHeader.setVisible(menuBarPosition == MenuBarPosition::burger);

	sidePanel.setContent(menuBarPosition == MenuBarPosition::burger ? &burgerMenu : nullptr, false);
	menuItemsChanged();
	//------------------------------------------------------------

	setSize(600, 400);
}

MainComponent::~MainComponent()
{
}
//==============================================================================

//------------------------------------------------------------------------------------
// Function sets a reference to a converter on the back end
//------------------------------------------------------------------------------------
void MainComponent::setConverter(Converter *_converter) {
	converterPtr = _converter;
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function paint
//------------------------------------------------------------------------------------
void MainComponent::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::lightgrey);

	// setting the boundary components for the child components
	auto area = getLocalBounds();
	area.removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());

	Point<int> bottomRight(area.getBottomRight());
	float margin = area.getWidth() * 0.01f;
	float h = (area.getHeight() / 5);
	auto sWindow = area.removeFromLeft(area.getWidth() - area.getWidth() / 10.0f);

	// sets the frequency domain window location and bounds
	fWindow.setBounds(sWindow.removeFromTop(h + (2 * h / 3)).reduced(margin));

	sWindow.removeFromTop(2 * margin);

	// sets the small time domain window location and bounds
	sTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));
	sWindow.removeFromLeft(-(area.getWidth() - area.getWidth() / 15.0f, 0));
	sWindow.setWidth(getLocalBounds().getWidth());

	// sets the big time domain window location and bounds
	bTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));
	
	// parameters to help layout soundshape's GUI controls (sliders, buttons, and combo boxes, i.e. non-widow components)
	Rectangle<float> topButtonArea(fWindow.getBounds().getTopRight().getX(), fWindow.getBounds().getTopRight().getY(), bottomRight.getX() - fWindow.getBounds().getTopRight().getX(), fWindow.getHeight());
	int h1 = (int)(topButtonArea.getHeight() * .15f);
	topButtonArea.reduce(topButtonArea.getWidth() * .15f, topButtonArea.getHeight() * .15f);

	// sets zoom slider location and bounds
	zoomSlider->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), 2 * h1);
	topButtonArea.removeFromTop((2 * h1) + (h1*.5));

	// sets harmonic button location and bounds
	harmonicButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);
	topButtonArea.removeFromTop(h1 + (h1*.5));

	// sets add button location and bounds
	addButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);

	// sets the fundumental frequency controls location and bounds
	Rectangle<float> fundFreqArea(sTWindow.getBounds().getTopRight().getX(), sTWindow.getBounds().getTopRight().getY(), bottomRight.getX() - sTWindow.getBounds().getTopRight().getX(), sTWindow.getHeight());
	fundFreqArea.reduce(fundFreqArea.getWidth() * .15f, 0.0f);
	fund.setBounds(fundFreqArea.getX(), fundFreqArea.getY(), fundFreqArea.getWidth(), h1);

	// sets the volume control location and bounds
	auto lowerRightQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	auto lowerMiddleQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	lowerRightQuad.reduce(lowerRightQuad.getWidth() * .10f, lowerRightQuad.getHeight() * .10f);
	volComp.setBounds(lowerRightQuad);

	float h2 = lowerMiddleQuad.getHeight() * .20f;
	lowerMiddleQuad.reduce(lowerMiddleQuad.getWidth() * .05f, lowerMiddleQuad.getHeight() * .20f);

	// sets the pre-sets and write button location and bounds
	cb.setBounds(lowerMiddleQuad.removeFromLeft((lowerMiddleQuad.getWidth() * 3) / 4).removeFromTop(h2));
	writeButton->setBounds(lowerMiddleQuad.removeFromTop(h2));

	// sets the envolope location and bounds
	sWindow.reduce(sWindow.getWidth() * .05f, sWindow.getHeight() * .05f);
	enve.setBounds(sWindow);

	if (menuBarPosition == MenuBarPosition::window)
	{
		menuBar->setBounds(getX(), getY(), getWidth(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
	}
	else
	{
		menuHeader.setBounds(getX(), getY(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight(), LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
	}

}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function resized
//------------------------------------------------------------------------------------
void MainComponent::resized()
{

}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function listens to sliders of child components
//------------------------------------------------------------------------------------
void MainComponent::sliderValueChanged(Slider * slider)
{
	// on change of a frequency spike slider updates conveter with new value
	if (slider->getParentComponent()->getComponentID().getIntValue() == FREQ_DOMAIN)
	{

			DBG(timeBlock);
			DBG(selectedProfile);
			DBG(currentProfile);
			converterPtr->updateFrequencyValue(currentProfile, slider->getComponentID().getIntValue(), slider->getValue());
			converterPtr->renderPreview(currentProfile);
			repaint();
	}
	// on change of a frequency spike slider updates conveter with new value
	if (slider->getComponentID().getIntValue() == PLAYTIME_SLIDER)
	{
		slider->getMaxValue();
		slider->getMinValue();
	}
	// on change of zoom slider updates zoom for frequency domain view
	if(slider == zoomSlider)
	{
		zoom = slider->getValue();
		repaint();
	}

	// on change update volume settings
	if (slider->getComponentID().getIntValue() == VOLUME_SLIDER)
	{
		//Needs hook up to back end

	}

	// on change update attack settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_ATTACK)
	{
		//Needs set up with back end calls
	}

	// on change update decay settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_DECAY)
	{
		//Needs set up with back end calls
	}

	// on change update release settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_RELEASE)
	{
		//Needs set up with back end calls
	}

	// on change update sustain settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_SUSTAIN)
	{
		//Needs set up with back end calls
	}
	repaint();
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function listens to buttons of child components
//-------------------------------------------------------------------------------------
void MainComponent::buttonClicked(Button* button)
{
	// On frequnecy profile selection updates new frequency profile 
	if(button->getRadioGroupId() == PROFILE_SELECT_BUTTON)
	{
		selectedProfile = (int)button->getComponentID().getIntValue();
        currentProfile = (int)(timeBlock * 5) + selectedProfile;

		DBG(timeBlock);
		DBG((int)(timeBlock * 5));
		DBG(selectedProfile);
		DBG(currentProfile);
		fWindow.setProfile();
		repaint();
	}

	// On frequnecy profile selection updates new frequency profile 
	if (button->getRadioGroupId() == TIME_SELECT_BUTTON)
	{
		timeBlock = (int)button->getComponentID().getIntValue();
	}

	// On Time domain selection repaint GUI
	if (button->getParentComponent() == &bTWindow)
	{
		repaint();
	}

	// add Button hides and shows buttons to add frequency spikes 
	if (button->getParentComponent()->getComponentID() == addButton)
	{
		//add = -1 * (add);
		repaint();
	}

	// Play button
	if (button->getComponentID().getIntValue() == PLAY_BUTTON)
	{

	}

	// Sustained Play button
	if (button->getComponentID().getIntValue() == SUSTAIN_PLAY_BUTTON)
	{
        if (button->getToggleState() == true) {
            float freq = fund.getNote();
            processor.playFreq(16.0f * notes[fund.getNote()]);
        }
        else {
            processor.panic();
        }
	}

	// Export button
	if (button->getComponentID().getIntValue() == EXPORT_BUTTON)
	{
		// need back end call for export
	}

	// Import button
	if (button->getComponentID().getIntValue() == IMPORT_BUTTON)
	{
		// need back end call for export
	}

	// Panic button
	if (button->getComponentID().getIntValue() == PANIC_BUTTON)
	{
		// need back end call for panic
        processor.panic();
	}

	// Funmental fruquency setting buttons
	if (button->getComponentID().getIntValue() == FUND_FREQ_BUTTON)
	{
		// need back end call for fundmental frequency

	}

	if (button == harmonicButton)
	{
		harm = harm * -1;
		repaint();
	}
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function loadSound
//-------------------------------------------------------------------------------------
void MainComponent::loadSound()
{
	timeBlock = 0;
	selectedProfile = 0;
	timeSize = 10;
	currentProfile = 0;

	fWindow.setZoom(&zoom, &harm, &add, this, this, converterPtr, 4000, &currentProfile);
	sTWindow.setTimeDomain(&timeBlock, &selectedProfile, &currentProfile, &timeSize, this, converterPtr);
	bTWindow.setProfile(&timeBlock, &currentProfile, &timeSize, this, this, converterPtr);
	volComp.setListeners(this, this);
	fund.setListener(this);
	enve.setListener(this);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getMenuBarNames
//-------------------------------------------------------------------------------------
StringArray MainComponent::getMenuBarNames()
{
	return { "Menu Position", "Themes", "Help", "About" };
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getMenuForIndex
//-------------------------------------------------------------------------------------
PopupMenu MainComponent::getMenuForIndex(int menuIndex, const String &)
{
	PopupMenu menu;
	if (menuIndex == 0)
	{
		menu.addCommandItem(&commandManager, CommandIDs::menuPositionInsideWindow);
		menu.addCommandItem(&commandManager, CommandIDs::menuPositionBurgerMenu);
	}
	else if (menuIndex == 1)
	{
		menu.addCommandItem(&commandManager, CommandIDs::DefaultTheme);
		menu.addCommandItem(&commandManager, CommandIDs::TestTheme);
	}
	else if (menuIndex == 2)
	{
		menu.addCommandItem(&commandManager, CommandIDs::ToolTips);
		menu.addCommandItem(&commandManager, CommandIDs::Tutorial);
	}
	else if (menuIndex == 3)
	{
		menu.addCommandItem(&commandManager, CommandIDs::Developers);
		menu.addCommandItem(&commandManager, CommandIDs::Licence);

	}

	return menu;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function MenuItemSelected
//-------------------------------------------------------------------------------------
void MainComponent::menuItemSelected(int, int)
{
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getNextCommandTarget
//-------------------------------------------------------------------------------------
ApplicationCommandTarget * MainComponent::getNextCommandTarget()
{
	//return &outerCommandTarget;
	return findFirstTargetParentComponent();
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getCommandInfo
//-------------------------------------------------------------------------------------
void MainComponent::getCommandInfo(CommandID _commandID, ApplicationCommandInfo & _result)
{
	switch (_commandID)
	{
		case CommandIDs::menuPositionInsideWindow:
			_result.setInfo("Toolbar Menu", "Places the menu bar inside the application window", "Menu", 0);
			_result.setTicked(menuBarPosition == MenuBarPosition::window);
			_result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
			break;
		case CommandIDs::menuPositionBurgerMenu:
			_result.setInfo("Sidebar Menu", "Uses a burger menu", "Menu", 0);
			_result.setTicked(menuBarPosition == MenuBarPosition::burger);
			_result.addDefaultKeypress('b', ModifierKeys::shiftModifier);
			break;
		case CommandIDs::DefaultTheme:
			_result.setInfo("Default Theme", "Sets theme to default theme", "Themes", 0);
			_result.setTicked(currentTheme == CommandIDs::DefaultTheme);
			_result.addDefaultKeypress('r', ModifierKeys::commandModifier);
			break;
		case CommandIDs::TestTheme:
			_result.setInfo("Test Theme", "Sets theme to test theme", "Themes", 0);
			_result.setTicked(currentTheme == CommandIDs::TestTheme);
			_result.addDefaultKeypress('h', ModifierKeys::commandModifier);
			break;
		case CommandIDs::ToolTips:
			_result.setInfo("Tool Tips", "Turns on and off tool tips", "Help", 0);
			_result.setTicked(tips);
			_result.addDefaultKeypress('t', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			break;
		case CommandIDs::Tutorial:
			_result.setInfo("Tutorial", "Tutorial about Soundshape", "Help", 0);
			_result.setTicked(false);
			break;
		case CommandIDs::Developers:
			_result.setInfo("Developers", "About the Developers", "About", 0);
			_result.setTicked(false);
			_result.addDefaultKeypress('d', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			break;
		case CommandIDs::Licence:
			_result.setInfo("Licence", "About Soundshape's Licence", "About", 0);
			_result.setTicked(false);
			break;
		default:
			break;

	}
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getAllCommands
//-------------------------------------------------------------------------------------
void MainComponent::getAllCommands(Array<CommandID>& c)
{
	Array<CommandID> commands{ CommandIDs::menuPositionInsideWindow,
						CommandIDs::menuPositionBurgerMenu,
						CommandIDs::DefaultTheme,
						CommandIDs::TestTheme,
						CommandIDs::ToolTips,
						CommandIDs::Tutorial,
						CommandIDs::Developers,
						CommandIDs::Licence };
	c.addArray(commands);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function perform
//-------------------------------------------------------------------------------------
bool MainComponent::perform(const InvocationInfo & info)
{
	switch (info.commandID)
	{
	case CommandIDs::menuPositionInsideWindow:
		setMenuBarPosition(MenuBarPosition::window);
		break;
	case CommandIDs::menuPositionBurgerMenu:
		setMenuBarPosition(MenuBarPosition::burger);
		break;
	case CommandIDs::DefaultTheme:
		setTheme(CommandIDs::DefaultTheme);
		break;
	case CommandIDs::TestTheme:
		setTheme(CommandIDs::TestTheme);
		break;
	case CommandIDs::ToolTips:
		tips = (!tips);
		break;
	case CommandIDs::Tutorial:
		break;	
	case CommandIDs::Developers:
		break;	
	case CommandIDs::Licence:
		break;
	default:
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function setMenuBarPosition
//-------------------------------------------------------------------------------------
void MainComponent::setMenuBarPosition(MenuBarPosition newPosition)
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

		repaint();
	}
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function setMenuBarPosition
//-------------------------------------------------------------------------------------
void MainComponent::setTheme(CommandID newTheme)
{
	if (currentTheme != newTheme)
	{
		currentTheme = newTheme;
		
		if (menuBarPosition != MenuBarPosition::burger)
			sidePanel.showOrHide(false);

		menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
		burgerMenu.setModel(menuBarPosition == MenuBarPosition::burger ? this : nullptr);
		menuHeader.setVisible(menuBarPosition == MenuBarPosition::burger);

		sidePanel.setContent(menuBarPosition == MenuBarPosition::burger ? &burgerMenu : nullptr, false);
		menuItemsChanged();;

		repaint();
	}
}
//-------------------------------------------------------------------------------------


//==============================================================================
/*
	Functions for burger Menu component
*/
//==============================================================================
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
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function Paint for Burger menu
//-------------------------------------------------------------------------------------
void BurgerMenuHeader::paint(Graphics& g)
{
	auto titleBarBackgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId)
		.darker();

	g.setColour(titleBarBackgroundColour);
	g.fillRect(getLocalBounds());
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function resized for buger menu
//-------------------------------------------------------------------------------------
void BurgerMenuHeader::resized()
{
	auto r = getLocalBounds();

	burgerButton.setBounds(r.removeFromRight(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()).withSizeKeepingCentre(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight() / 2, LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight() / 2));

	titleLabel.setFont(Font(getHeight() * 0.5f, Font::plain));
	titleLabel.setBounds(r);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function showOrHide for buger menu
//-------------------------------------------------------------------------------------
void BurgerMenuHeader::showOrHide()
{
	sidePanel.showOrHide(!sidePanel.isPanelShowing());
	repaint();
}
//==============================================================================

