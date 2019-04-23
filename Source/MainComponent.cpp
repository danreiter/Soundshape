#include "MainComponent.h"

float MainComponent::notes[12] = { 27.5f, 29.50f, 30.87f, 16.35f, 17.32f, 18.35f, 19.45f, 20.60f, 21.83f, 23.12f, 24.5f, 25.96 };

//==============================================================================
//  Component declares and instantiates other gui components and passes variables from 
//==============================================================================
MainComponent::MainComponent(Soundshape_pluginAudioProcessor& p, AudioProcessorValueTreeState& _valueTreeState) :
	processor(p),
	enve(_valueTreeState),
	valueTreeState(_valueTreeState),
	volComp(_valueTreeState),
    bTWindow(_valueTreeState)
{
	//----------Setting reference to the converter----------------------------------
    setConverter(&(processor.getConverter()));
	//------------------------------------------------------------

    //----------Default settings----------------------------------
	add = -1;
	harm = -1;
	zoom = 4.0;
	presetPath = File().getCurrentWorkingDirectory();
	selectedFile = newFile;
	laf = new CustomLookAndFeel();
	laf->initColors(Colour(SoundshapeLAFs::base1ID), Colour(SoundshapeLAFs::base1textID), 
		Colour(SoundshapeLAFs::base2ID), Colour(SoundshapeLAFs::base2textID), 
		Colour(SoundshapeLAFs::background1ID), Colour(SoundshapeLAFs::background2ID),
		Colour(SoundshapeLAFs::background3ID));
	laf->setColors();
	

	//------------------------------------------------------------


	//------Passing references to child components----------------


	//------------------------------------------------------------

	addAndMakeVisible(sTWindow);
	addAndMakeVisible(bTWindow);
	addAndMakeVisible(fund);
	addAndMakeVisible(volComp);
	addAndMakeVisible(enve);
	addAndMakeVisible(cb);

	//-----Setting testing values for the combo box-------------
	loadPresetPath();
	cb.setSelectedItemIndex(0);
    cb.setTooltip("Contains all the current presets");
	cb.addListener(this);
	//------------------------------------------------------------

	//------------Setting Button Values---------------------------

	// Harmonic button to toggle harmonic filter for selecting 
	harmonicButton = new TextButton("Harmonic");
	harmonicButton->setClickingTogglesState(true);
    harmonicButton->setTooltip("When enabled, the harmonic setting will only allow the creation of new frequency spikes at harmonically correct frequencies");
	harmonicButton->onClick = [this]
	{
		harm = -1 * (harm);
		repaint();
        
	};

	// Button to add friquency spike to a frequency profile
	addButton = new TextButton("Add");
	addButton->setClickingTogglesState(true);
	addButton->addListener(this);
    addButton->setTooltip("Allows the creation of a frequency spike at any available frequency");
	addButton->onClick = [this]
	{
        
		add = -1 * (add);
		repaint();
	};

	// Write button to save sound profiles to the preset folder
	writeButton = new TextButton("Write");
	writeButton->addListener(this);
	writeButton->setComponentID((String)WRITE_BUTTON);
    writeButton->setTooltip("Writes the chosen preset");

	// Zoom slider to zoom in and out of frequncy window
	zoomSlider = new Slider(Slider::IncDecButtons, Slider::TextBoxAbove);
	zoomSlider->setRange(1.0f, 40.0f, .5);
	zoomSlider->setValue(zoom, sendNotificationAsync);
	zoomSlider->setTextValueSuffix(" X");
	zoomSlider->addListener(this);
	zoomSlider->setTooltip("Zoom the spectrum window in and out");

	//------------------------------------------------------------

	addAndMakeVisible(writeButton);
	addAndMakeVisible(harmonicButton);
	addAndMakeVisible(addButton);
	addAndMakeVisible(zoomSlider);

	loadSound();
	addAndMakeVisible(fWindow);
	setAllLookAndFeels(laf, this);

	//----------Menu settings----------------------------------
	menuBar.reset(new MenuBarComponent(this));
	addAndMakeVisible(menuBar.get());
	menuBar->setLookAndFeel(laf);
	setApplicationCommandManagerToWatch(&commandManager);
	commandManager.registerAllCommandsForTarget(this);
	addKeyListener(commandManager.getKeyMappings());
	addChildComponent(menuHeader);
	sidePanel.setLookAndFeel(laf);
	addAndMakeVisible(sidePanel);
	burgerMenu.setLookAndFeel(laf);


	// this is necessary to give the component focus so the menu isn't grayed out
	Timer::callAfterDelay(250, [this](void) {this->grabKeyboardFocus();});
	
	//------------------------------------------------------------

	// this is necessary to give the component focus so the menu isn't grayed out
	Timer::callAfterDelay(250, [this](void){this->grabKeyboardFocus();});

	setSize(600, 400);
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function showKey
//------------------------------------------------------------------------------------
void MainComponent::showKey(bool vis)
{
	if (vis)
	{
		keyboardPopup *keyboard = new keyboardPopup(converterPtr);
		keyboard->addToDesktop(ComponentPeer::windowIsTemporary);
		midiKeyboard = keyboard;
		Rectangle<int> area(0, 0, 600, 100);

		RectanglePlacement placement(RectanglePlacement::xLeft
			| RectanglePlacement::yBottom
			| RectanglePlacement::doNotResize);

		auto result = placement.appliedTo(area, Desktop::getInstance().getDisplays()
			.getMainDisplay().userArea.reduced(20));
		keyboard->setBounds(result);

		keyboard->setVisible(vis);
	}
	else
	{
		midiKeyboard->setVisible(vis);
		midiKeyboard.deleteAndZero();
	}
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function showLic
//------------------------------------------------------------------------------------
void MainComponent::showLic()
{
		String m = "Soundshape. Spectral synthesis application / audio plugin. \n"
            "Copyright(C) 2019 by Mardigon Toler, Daniel Reiter, Gregory Hughes \n"
            "\n"
            "This program is free software : you can redistribute it and/or modify\n"
            "it under the terms of the GNU General Public License as published by\n"
            "the Free Software Foundation, either version 3 of the License, or\n"
            "(at your option) any later version.\n "
            "\n"
            "This program is distributed in the hope that it will be useful, but\n"
            "WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\n"
            "GNU General Public License for more details.\n"
            "\n"
            "You should have received a copy of the GNU General Public License\n"
            "along with this program.If not, see < http://www.gnu.org/licenses/>."
            "\n"
            "\n"
            "Built with the JUCE framework: \n"
            "www.juce.com"
            "\n"
            "VST is a trademark of Steinberg Media Technologies GmbH, registered in Europe \n"
            "and other countries."
            ;


		DialogWindow::LaunchOptions options;
		auto* label = new Label();
        
		label->setText(m, dontSendNotification);
		label->setColour(Label::textColourId, Colours::black);
		options.content.setOwned(label);

		Rectangle<int> area(0, 0, 600, 300);

		options.content->setSize(area.getWidth(), area.getHeight());

		options.dialogTitle = "License";
		options.dialogBackgroundColour = laf->findColour(SoundshapeLAFs::background2ID);
		options.escapeKeyTriggersCloseButton = true;
		options.useNativeTitleBar = false;
		options.resizable = false;

		options.launchAsync();

}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function showDev
//------------------------------------------------------------------------------------
void MainComponent::showDevs()
{
	String m = "Soundshape is a free audio synthesizer that lets you create sounds by designing their spectrum. It\n"
		"runs as a standalone plugin or a VST. Soundshape takes advantage of Fast Fourier Transforms to synthesize the sound you design in real time.\n"
		"\nSoundshape is a software instrument but can also be used for education or as a sound effect\n"
		"generator, or any other use you might have for sound synthesis.";


	DialogWindow::LaunchOptions options;
	auto* label = new Label();

	label->setText(m, dontSendNotification);
	label->setColour(Label::textColourId, laf->findColour(SoundshapeLAFs::base1textID));
	options.content.setOwned(label);

	Rectangle<int> area(0, 0, 600, 300);

	options.content->setSize(area.getWidth(), area.getHeight());

	options.dialogTitle = "About Soundshape";
	options.dialogBackgroundColour = laf->findColour(SoundshapeLAFs::background2ID);
	options.escapeKeyTriggersCloseButton = true;
	options.useNativeTitleBar = false;
	options.resizable = false;

	options.launchAsync();

}

MainComponent::~MainComponent()
{
    if (showKeyboard) {
        showKey(false);
        showKeyboard = false;
    }

	delete harmonicButton;
	delete addButton;
	delete zoomSlider;
	delete writeButton;
	delete laf;

}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function for changing from one lookandfeel to the other
//------------------------------------------------------------------------------------
void MainComponent::setAllLookAndFeels(LookAndFeel* laf, Component* comp)
{
	for (auto* child : comp->getChildren())
	{
		child->setLookAndFeel(laf); // this will change everything in Soundshape to the current colors of laf
		if(child == &volComp) // to change specific buttons from non-this components, we need to specifically loop through those components
		{
			for (auto * subChild : child->getChildren()) {
				if (subChild->getComponentID().getIntValue() == PANIC_BUTTON) {
					subChild->setColour(TextButton::buttonColourId, Colours::red);
					subChild->setColour(TextButton::textColourOffId, Colours::black);
				}
				else if (subChild->getComponentID().getIntValue() == IMPORT_BUTTON)
				{
					subChild->setColour(TextButton::buttonColourId, laf->findColour(SoundshapeLAFs::base2ID));
					subChild->setColour(TextButton::textColourOffId, laf->findColour(SoundshapeLAFs::base2textID));

				}
				else if (subChild->getComponentID().getIntValue() == EXPORT_BUTTON)
				{
					subChild->setColour(TextButton::buttonColourId, laf->findColour(SoundshapeLAFs::base2ID));
					subChild->setColour(TextButton::textColourOffId, laf->findColour(SoundshapeLAFs::base2textID));
				}
			}
		}
		else if (child->getComponentID().getIntValue() == WRITE_BUTTON)
		{
			child->setColour(TextButton::buttonColourId, laf->findColour(TextButton::buttonOnColourId));
			child->setColour(TextButton::textColourOffId, laf->findColour(TextButton::textColourOnId));
		}
		

	}

}

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
	g.fillAll(laf->findColour(SoundshapeLAFs::background1ID));

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
		menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
		menuBar->repaint();
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
		converterPtr->updateFrequencyValue(currentProfile, slider->getComponentID().getIntValue(), slider->getValue());
		converterPtr->renderPreview(currentProfile);

	}
	// on change of a frequency spike slider updates conveter with new value
	if (slider->getComponentID().getIntValue() == PLAYTIME_SLIDER)
	{
        // NOTE: setValueNotifyingHost doesn't notifiy other listeners, so we have to do that separately
        RangedAudioParameter* beginningParam = valueTreeState.getParameter("beginningChunk");
        RangedAudioParameter* endingParam = valueTreeState.getParameter("endingChunk");
        float maxParamValue = beginningParam->getNormalisableRange().end;
        int bottomValue = (int)slider->getMinValue();
        int upperValue = (int)slider->getMaxValue();

        beginningParam->setValueNotifyingHost(bottomValue/maxParamValue);
        beginningParam->sendValueChangedMessageToListeners(bottomValue/maxParamValue);
        endingParam->setValueNotifyingHost(upperValue/maxParamValue);
        endingParam->sendValueChangedMessageToListeners(upperValue/maxParamValue);
	}
	// on change of zoom slider updates zoom for frequency domain view
	if(slider == zoomSlider)
	{
		zoom = slider->getValue();

	}


	repaint();
}

//-------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function listensfor buttons to get hovered, pressed, or released
//------------------------------------------------------------------------------------
void MainComponent::buttonStateChanged(Button * button)
{
    // Play button
    if (button->getComponentID().getIntValue() == PLAY_BUTTON)
    {
        if (button->getState() == Button::buttonDown) {
            processor.playFreq(16.0f * notes[fund.getNote()]);
        }
        else if( ! volComp.getSustainedPlayBtnToggled()){
            processor.panic();
        }
    }
}
//-------------------------------------------------------------------------------------


void MainComponent::drawSound() {
    fWindow.setProfile();
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
        converterPtr->renderPreview(i);
    }
    repaint();
}

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

		fWindow.setProfile();

	}

	// On frequnecy profile selection updates new frequency profile 
	if (button->getRadioGroupId() == TIME_SELECT_BUTTON)
	{
		timeBlock = (int)button->getComponentID().getIntValue();
	}

	// On Time domain selection repaint GUI
	if (button->getParentComponent() == &bTWindow)
	{

	}

	// add Button hides and shows buttons to add frequency spikes 
	if (button->getComponentID() == addButton->getComponentID())
	{

	}

	// add Button hides and shows buttons to add frequency spikes 
	if (button->getComponentID() == writeButton->getComponentID())
	{
		pushedWriteBtn = true;
		if (selectedFile == newFile)
		{
			saveAs();
		}
		else
		{
			promptSaveOptions();
		}

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
		exportFile();
	}

	// Import button
	if (button->getComponentID().getIntValue() == IMPORT_BUTTON)
	{
		// need back end call for export
		importFile();
	}

	// Panic button
	if (button->getComponentID().getIntValue() == PANIC_BUTTON)
	{
		// need back end call for panic
        processor.panic();
	}
	if (button == harmonicButton)
	{
		harm = harm * -1;

	}
	repaint();
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function listens for ComboBox component changes
//-------------------------------------------------------------------------------------
void MainComponent::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
	if(comboBoxThatHasChanged->getComponentID() == cb.getComponentID())
	{
		if (pushedWriteBtn)
		{
			pushedWriteBtn = false;
		}
		else
		{
			saveFilePrompt();
			if (cb.getSelectedItemIndex() == 0)
			{

				selectedFile = newFile;
				cb.setSelectedItemIndex(0);
			}
			else
			{

				String fileName = cb.getText();
				fileName.append((String)".xml", 4);
				selectedFile = File(presetPath.getChildFile(fileName));
			}
			loadFile();
            drawSound();
		}

	};
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function saveIs for saving exist sounds
//-------------------------------------------------------------------------------------
bool MainComponent::save()
{
	// selectedFile is the full path to the current sound
	String docString = IOHandler::createStateDocument(IOHandler::createStateXML(*converterPtr, valueTreeState));
	IOHandler::writeStateXMLFile(selectedFile, docString);
	return false;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function saveAs is for saving new sounds
//-------------------------------------------------------------------------------------
bool MainComponent::saveAs()
{
	FileChooser chooser("Save As File Name", presetPath, "*.xml");
	grabKeyboardFocus();
	if (chooser.browseForFileToSave(true))
	{
		File tempFile = chooser.getResult();
		String fileName = tempFile.getFileName();
		if (fileName.endsWith((String) ".xml"))
		{
			if (pushedWriteBtn)
			{
				selectedFile = tempFile;
			}

			//save file
			String docString = IOHandler::createStateDocument(IOHandler::createStateXML(*converterPtr, valueTreeState));
			IOHandler::writeStateXMLFile(tempFile, docString);
			if (presetPath == tempFile.getParentDirectory())
			{	 
					pushedWriteBtn = true;
					cb.addItem(fileName.dropLastCharacters(4), cb.getNumItems() + 1);
					if (selectedFile == tempFile)
					{
						pushedWriteBtn = true;
						cb.setSelectedItemIndex(cb.getNumItems() - 1);
					}
			}
			else
			{
				if (selectedFile == tempFile)
				{
					presetPath = tempFile.getParentDirectory();
					cb.clear();
					cb.addItem("New Sound", 1);
					DirectoryIterator iter(presetPath, false, "*.xml");
					int i = 2;
					while (iter.next())
					{
						File nextSound(iter.getFile());
						pushedWriteBtn = true;
						cb.addItem(nextSound.getFileName().dropLastCharacters(4), i);
						i++;
					}
					pushedWriteBtn = true;
					cb.setText(fileName.dropLastCharacters(4));
				}
			}
		}

	}
	// set index of comboBox to new file name
	return false;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function saveAs is for saving new sounds
//-------------------------------------------------------------------------------------
void MainComponent::loadFile()
{

	
	timeBlock = 0;
	selectedProfile = 0;
	timeSize = 10;
	currentProfile = 0;
	add = -1;
	harm = -1;
	zoom = 1.0;

	
	//file handling here
	std::unique_ptr<XmlElement> stateXml(XmlDocument::parse(selectedFile));
	if (stateXml != nullptr) {
		IOHandler::restoreStateFromXml(valueTreeState, *converterPtr, stateXml);
	}
	else
	{
		//pushedWriteBtn = true;
		cb.setSelectedItemIndex(0);
		selectedFile = newFile;

        // clear out all the profile bins and reset
        processor.restoreNewSoundParams();
        processor.restoreDefaultProfile();
        repaint();

	}
	
	//do After file load
	fWindow.setProfile();
	repaint();
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function importFile
//-------------------------------------------------------------------------------------
void MainComponent::importFile()
{
	FileChooser chooser("Please select file to import (.ogg, .wav, or .flac)", presetPath, "*.ogg;*.wav;*.flac",false);
	if (chooser.browseForFileToOpen())
	{
		File inFile = chooser.getResult();//.withFileExtension();
		try{
		    IOHandler::importConverterAudio(inFile, converterPtr);
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Success",
                                        "Imported " + inFile.getFileName());
			if (presetPath == inFile.getParentDirectory())
			{
				pushedWriteBtn = true;
				cb.addItem(inFile.getFileNameWithoutExtension(), cb.getNumItems() + 1);
			}
		}
		catch (SoundshapeAudioImportException&){
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error",
                                        "There was problem reading that file."
                                        " Please make sure it is valid and has a proper extension."
                                        " (.ogg, .wav, .flac)");
		}
	}
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function exportFile
//-------------------------------------------------------------------------------------
void MainComponent::exportFile()
{
	String ext = "";
	PopupMenu exportPrompt;
	exportPrompt.addSectionHeader("Export file type:");
	exportPrompt.addItem(1, "WAV");
	exportPrompt.addItem(2, "OGG");
	exportPrompt.addItem(3, "FLAC");
	exportPrompt.addItem(4, "cancel");
	const int result = exportPrompt.show();
	if (result == 0)
	{
		// user dismissed the menu without picking anything
		pushedWriteBtn = false;
	}
	else if (result == 1)
	{
		ext = ".wav";
	}
	else if (result == 2)
	{
		ext = ".ogg";
	}
	else if (result == 3)
	{
		ext = ".flac";
	}
	if (ext != "")
	{
		FileChooser chooser("Export Audio as " + ext, presetPath, "*"+ext, false);
		if (chooser.browseForFileToSave(true))
		{
			// .withFileExtension is intelligent about if they enter the extension themselves or not
			File outFile = chooser.getResult().withFileExtension(ext);

			// this method handles the correct audio format based on the file name
			try {
				IOHandler::exportConverterAudio(outFile, converterPtr);
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Success",
											"Exported " + outFile.getFileName());
			}
			catch (SoundshapeAudioExportException&) {
				AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error",
											"There was problem writing to that file.");
			}

		}
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

	fWindow.setZoom(&zoom, &harm, &add, this, this, converterPtr, 4000, &currentProfile, laf);
	sTWindow.setTimeDomain(&timeBlock, &currentProfile, &timeSize, this, converterPtr);
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
	// For interface with both a drop down and burger menu
	//return { "Menu Position", "Settings", "Themes", "Help", "About" };
	// For interface with only a one menu type
	return { "Settings", "Themes", "Help", "About" };
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function getMenuForIndex
//-------------------------------------------------------------------------------------
PopupMenu MainComponent::getMenuForIndex(int menuIndex, const String &)
{
	// for use with more than one menu option
	PopupMenu menu;
	//if (menuIndex == 0)
	//{
	//	menu.addCommandItem(&commandManager, CommandIDs::menuPositionInsideWindow);
	//	menu.addCommandItem(&commandManager, CommandIDs::menuPositionBurgerMenu);
	//}
	if (menuIndex == 0)
	{
		menu.addCommandItem(&commandManager, CommandIDs::Keyboard);
		menu.addCommandItem(&commandManager, CommandIDs::PresetPath);
	}
	else if (menuIndex == 1)
	{
		menu.addCommandItem(&commandManager, CommandIDs::DefaultTheme);
		menu.addCommandItem(&commandManager, CommandIDs::TestTheme);
		menu.addCommandItem(&commandManager, CommandIDs::SchoolTheme);
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
			_result.setInfo("Drop down Menu", "Places the menu bar inside the application window", "Menu", 0);
			_result.setTicked(menuBarPosition == MenuBarPosition::window);
			_result.addDefaultKeypress('w', ModifierKeys::shiftModifier);
			break;
		case CommandIDs::menuPositionBurgerMenu:
			_result.setInfo("Sidebar Menu", "Uses a side bar menu", "Menu", 0);
			_result.setTicked(menuBarPosition == MenuBarPosition::burger);
			_result.addDefaultKeypress('b', ModifierKeys::shiftModifier);
			break;
		case CommandIDs::Keyboard:
			_result.setInfo("MIDI Keyboard", "Turns on/off MIDI Keyboard for standalone mode", "Settings", 0);
			_result.setTicked(showKeyboard);
			_result.addDefaultKeypress('k', ModifierKeys::commandModifier);
			break;
		case CommandIDs::PresetPath:
			_result.setInfo("Preset file path", "Set preset file reading and writing path", "Settings", 0);
			_result.setTicked(false);
			_result.addDefaultKeypress('p', ModifierKeys::shiftModifier);
			break;
		case CommandIDs::DefaultTheme:
			_result.setInfo("Default Theme", "Sets theme to Default Theme", "Themes", 0);
			_result.setTicked(currentTheme == CommandIDs::DefaultTheme);
			_result.addDefaultKeypress('r', ModifierKeys::commandModifier);
			break;
		case CommandIDs::TestTheme:
			_result.setInfo("Is it Purple?", "Sets theme to Is it Purple?", "Themes", 0);
			_result.setTicked(currentTheme == CommandIDs::TestTheme);
			_result.addDefaultKeypress('h', ModifierKeys::commandModifier);
			break;
		case CommandIDs::SchoolTheme:
			_result.setInfo("Golden Bear", "Sets theme to Golden Bear", "Themes", 0);
			_result.setTicked(currentTheme == CommandIDs::SchoolTheme);
			_result.addDefaultKeypress('g', ModifierKeys::commandModifier);
			break;
		case CommandIDs::ToolTips:
			_result.setInfo("Tool Tips", "Turns on and off tool tips", "Help", 0);
			_result.setTicked(tips);
			_result.addDefaultKeypress('t', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			break;
		case CommandIDs::Tutorial:
			_result.setInfo("Tutorial", "How to use Soundshape", "Help", 0);
			_result.setTicked(false);
			break;
		case CommandIDs::Developers:
			_result.setInfo("About Soundshape", "About Soundshape", "About", 0);
			_result.setTicked(false);
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
						CommandIDs::Keyboard,
						CommandIDs::PresetPath,
						CommandIDs::DefaultTheme,
						CommandIDs::TestTheme,
						CommandIDs::SchoolTheme,
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
	case CommandIDs::Keyboard:
		showKeyboard = !showKeyboard;
		showKey(showKeyboard);
		break;
	case CommandIDs::PresetPath:
		setPresetPath();
		break;
	case CommandIDs::DefaultTheme:
		setTheme(CommandIDs::DefaultTheme);
		laf->initColors(Colours::orange, Colours::black, Colours::darkblue, Colours::white, Colours::lightgrey, Colours::peachpuff, Colours::red);
		laf->setColors();
		setAllLookAndFeels(laf, this);
		break;
	case CommandIDs::TestTheme:
		setTheme(CommandIDs::TestTheme);
		laf->initColors(Colour(0xff40005b), Colours::white, Colour(0xffdd0000), Colours::black, Colour(0xff202020), Colour(0xffd57fed), Colours::darkred);
		laf->setColors();
		setAllLookAndFeels(laf, this);
		break;
	case CommandIDs::SchoolTheme:
		setTheme(CommandIDs::SchoolTheme);
		laf->initColors(Colour(0xff002855), Colours::white, Colour(0xffeaaa00), Colours::black, Colour(0xff333f48), Colour(0xff9abeaa), Colour(0xfffdda24));
		laf->setColors();
		setAllLookAndFeels(laf, this);
		break;
	case CommandIDs::ToolTips:
		tips = (!tips);
		if (!tips)
			ttp.setMillisecondsBeforeTipAppears(1000000); // 16 minutes and 40 seconds
		else
			ttp.setMillisecondsBeforeTipAppears(700);
		break;
	case CommandIDs::Tutorial:
		break;	
	case CommandIDs::Developers:
		showDevs();
		break;	
	case CommandIDs::Licence:
			showLic();
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
// Function setTheme
//-------------------------------------------------------------------------------------
void MainComponent::setTheme(CommandID newTheme)
{
	if (currentTheme != newTheme)
	{
		currentTheme = newTheme;
		
		
		sidePanel.showOrHide(false);

		menuBar->setVisible(menuBarPosition == MenuBarPosition::window);
		burgerMenu.setModel(menuBarPosition == MenuBarPosition::burger ? this : nullptr);
		menuHeader.setVisible(menuBarPosition == MenuBarPosition::burger);

		sidePanel.setContent(menuBarPosition == MenuBarPosition::burger ? &burgerMenu : nullptr, false);
		menuItemsChanged();

		repaint();
		if (menuBarPosition == MenuBarPosition::burger)
			sidePanel.showOrHide(true);
	}
}

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function setPresetPath
//-------------------------------------------------------------------------------------
void MainComponent::setPresetPath()
{

	FileChooser chooser("New Preset Path", presetPath, "", false);
	if (chooser.browseForDirectory())
	{
		File tempDir = chooser.getResult();
		if (presetPath != tempDir)
		{
			saveFilePrompt();
			presetPath = tempDir;
			loadPresetPath();
		}
		repaint();
	}
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function loadPresetPath
//-------------------------------------------------------------------------------------
void MainComponent::loadPresetPath()
{
	cb.clear();
	cb.addItem("New Sound", 1);

	DirectoryIterator iter(presetPath, false, "*.xml");
	int i = 2;
	while(iter.next())
	{
		File nextSound(iter.getFile());
		pushedWriteBtn = true;
		cb.addItem(nextSound.getFileName().dropLastCharacters(4), i);
		i++;
	}
	selectedFile = newFile;
	pushedWriteBtn = true;
	cb.setSelectedItemIndex(0);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function saveFilePrompt
//-------------------------------------------------------------------------------------
void MainComponent::saveFilePrompt()
{
	PopupMenu savePrompt;
	savePrompt.addSectionHeader("Would you like to save your current sound?");
	savePrompt.addItem(1, "save");
	savePrompt.addItem(2, "Cancel");
	const int result = savePrompt.show();
	DBG(result);
	if (result == 0)
	{
		DBG("result == 1");
			pushedWriteBtn = false;
	}
	if (result == 1)
	{
		if (selectedFile == newFile)
		{
			saveAs();
		}
		else
		{
			promptSaveOptions();

		}

		// user picked item 1
	}
	else if (result == 2)
	{
		// user picked item 2
		pushedWriteBtn = false;
	}

}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function promptSaveOptions
//-------------------------------------------------------------------------------------
void MainComponent::promptSaveOptions()
{
	PopupMenu saveAsPrompt;
	saveAsPrompt.addSectionHeader("Would you like to save as a new sound?");
	saveAsPrompt.addItem(1, "Save");
	saveAsPrompt.addItem(2, "Save as a new sound");
	saveAsPrompt.addItem(3, "Cancel");
	const int result = saveAsPrompt.show();
	if (result == 0)
	{
		// user dismissed the menu without picking anything
		pushedWriteBtn = false;
		newSave = false;
	}
	else if (result == 1)
	{
		save();
		pushedWriteBtn = false;
		newSave = false;
		// user picked item 1
	}
	else if (result == 2)
	{
		saveAs();
		if (pushedWriteBtn)
		{
			pushedWriteBtn = false;
			newSave = true;
		}
		// user picked item 2
	}
	else if (result == 3)
	{
		// user picked item 2
		pushedWriteBtn = false;
		newSave = false;
	}
}
//-------------------------------------------------------------------------------------
//==============================================================================
// end mainComponent
//==============================================================================


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