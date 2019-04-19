/*
  ==============================================================================

    GuiFunc.h
    Created: 24 Dec 2018 12:44:39pm
    Author:  Daniel Reiter
	Description:Custom Components that are used to control some of Soundshape's functions.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "laf.h"

#define VOLUME_SLIDER 2001			///< Component ID number for the volume slider.
#define PLAY_BUTTON 2002			///< Component ID number for the play button.
#define EXPORT_BUTTON 2003			///< Component ID number for the export button.
#define IMPORT_BUTTON 2004			///< Component ID number for the import button.
#define PANIC_BUTTON 2005			///< Component ID number for the panic button.
#define FUND_FREQ_SLIDER 2006		///< Component ID number for the funmental frequency slider.
#define SUSTAIN_PLAY_BUTTON 2007	///< Component ID number for the sustained play button.

//==============================================================================
/**
Volume Box is a slider component drawn in a box with volume symbols.

Componet is used to change Soundshapes volume settings.
@param _valueTreeState Used update the back-end of Soundshape on setting changes for volume.
*/
//==============================================================================
class volumeBox : public Component
{
public:

	volumeBox(AudioProcessorValueTreeState& _valueTreeState);
	~volumeBox();
	/**
	Sets slider and draws volume symbols in a rectangle.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
	void paint(Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
	void resized() override;
	/**
	Set an event listener on the volume slider.

	Used to call repaint on for mainComponent on updates.
	@param _listener Is adds an event listener to the volume slider.
	*/
	void setVolumeListener(Slider::Listener * _listener);
private:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment; ///< Handle the event listening for value tree state.
	Slider * volume;								  ///< Slider to change volume.
    std::unique_ptr<SliderAttachment> gainAttachment; ///< Lets the back and front end volume control each other.
    

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(volumeBox)
};
//==============================================================================


//==============================================================================
/**
Contains Soundshapes GUI functional controls.

Contains buttons that control Soundshapes import, export, play, and sustained play functions.
Also contains a volume box component that controls Soundshapes volume.

@see volumeBox

@param _valueTreeState Used update the back-end of Soundshape on setting changes.
*/
//==============================================================================
class GuiFunc : public Component
{
public:
	GuiFunc(AudioProcessorValueTreeState& _valueTreeState);
	~GuiFunc();
	/**
	Sets the Placement of buttons and sliders and paints the component.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
	void paint(Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
	void resized() override;
	/**
	Set event listener to all buttons and sliders in the component.

	@see volumeBox::setListener()
	@param _sliderListener Sets the volBox event listener.
	@param _buttonListener Adds button listeners to the panic, export, import, play, and sustained play buttons.
	*/
	void setListeners(Slider::Listener* _sliderListener, Button::Listener* _buttonListener);
	/**
	Returns the current state of the sustained play button.
	*/
    bool getSustainedPlayBtnToggled();

private:
    AudioProcessorValueTreeState& valueTreeState;		///> event listen to update setting of Soundshape directly to the back-end. 
	Slider::Listener * sListen;							///> Reference to parent for slider listener.
	Button::Listener * bListen;							///> Refernence to parent for button listener.
	Button *panicBtn;                                   ///> Button for the panic function.
	Button *exportBtn;									///> Button for the export function.
	Button *playBtn;									///> Button for the play function.
	Button *importBtn;									///> Button for the import function.
	Button *sustainPlyBtn;                              ///> Button for the sustained play function.
	volumeBox *volBox;									///> Component to control the volume.

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiFunc)
};
//==============================================================================

//==============================================================================
/**
Component contains controls and displays fundemental frequency.

Component returns the fundmental frequency (The note which Soundshape's stand alone mode plays sound in).
Comprised of a slider and a text box to display the note as a letter value.
*/
//==============================================================================
class fundFreq : public Component
{
public:
	fundFreq();
	~fundFreq();

	/**
	Sets the placement and paints of slider and text box components.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
	void paint(Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
	void resized() override;
	/**
	Returns the index of the current note that the fundmental frequency is set to.
	*/
	int getNote();
	/**
	Updates the text box to display fundmental frequency as a letter note.
	*/
	void updateText();
	/**
	Sets event listener for the fundFreqSlider.

	@see Slider::Listener
	@param _listener Passes an event handler to the fundFreqSlider slider.
	*/
	void setListener(Slider::Listener* _listener);

private:
	Label *txtBox;							///< Displays fundmental frequency as a letter value.
	int num;								///< Stores the current fundamental frequency value as integer to index an array.
	Slider::Listener * sListener;			///< Reference to parent as slider listener.
	Slider * fundFreqSlider;				///< Slider to change fundamental frequency value.
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(fundFreq)
};
//==============================================================================
//end fundFreq Functions
//==============================================================================
