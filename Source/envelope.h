/*
  ==============================================================================

    envelope.h
    Created: 24 Dec 2018 1:00:17pm
    Author:  Daniel Reiter
	Description: Component contains slider for the user to manipulate the envelope settings

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "laf.h"

#define ENVELOPE_ATTACK 7002	///< Component ID number for the Attack slider
#define ENVELOPE_DECAY 7003		///< Component ID number for the Decay slider
#define ENVELOPE_SUSTAIN 7004	///< Component ID number for the Sustain slider
#define ENVELOPE_RELEASE 7005	///< Component ID number for the Release slider

//==============================================================================
/*
Envelope contains controls for the ADSR envelope.

Component has four slider to control the each of the setting of the adsr envelope 
and draws the box component that contains the the slider.

@param _valueTreeState Used to update the back-end of Soundshape on setting when the enevolope is sliders are changed. 
*/
class envelope    : public Component
{
public:
    envelope(AudioProcessorValueTreeState& _valueTreeState);
    ~envelope();
	/**
	Sets sliders, draws labels and scale for envelope component.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
    void paint (Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
    void resized() override;
	/**
	Set an event listeners on each of the adsr envelope's sliders.

	Used to call repaint on for mainComponent on updates.
	@param _listener Is adds an event listener to each of the adsr envelope's sliders.
	*/
	void setListener(Slider::Listener *_listener);

private:
	Slider *Attack;	 ///< Slider to change value for the attack value in the ADSR envelope	
	Slider *Decay;	 ///< Slider to change value for the decay value in the ADSR envelope
	Slider *Sustain; ///< Slider to change value for the sustain value in the ADSR envelope
	Slider *Release; ///< Slider to change value for the release value in the ADSR envelope  

    
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment; ///> Attachments components to the back-end parameters.
    std::unique_ptr<SliderAttachment> attackAttachment;						 ///> Attachments attack component to the back-end parameters
    std::unique_ptr<SliderAttachment> decayAttachment;						 ///> Attachments decay component to the back-end parameters
    std::unique_ptr<SliderAttachment> sustainAttachment;					 ///> Attachments sustain component to the back-end parameters
    std::unique_ptr<SliderAttachment> releaseAttachment;					 ///> Attachments release component to the back-end parameters
    AudioProcessorValueTreeState& valueTreeState;							 ///> Handles component event linked to the back-end parameters

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (envelope)
};
