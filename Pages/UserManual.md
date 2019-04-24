. {#UserManual}
=================

# Soundshape User Manual

## General Operation
Soundshape's primary purpose is to output sound.
It has virtual MIDI inputs and as it receives input notes,
it will play various sections of its spectrum. 

## Modes of Operation
Soundshape has two major modes of operation:
- Plugin
    + Runs as a VST plugin in a Digital Audio Workstation (DAW) 
    or other VST compatible host. Soundshape will receive all
    MIDI input from the DAW and output its audio to the DAW. 
    Check your DAW user manual for more information about how to
    set up a VST plugin instrument.
    + In this mode, the DAW can directly record the software's 
    output and also allow the user to route the output through
    other plugins. (Try applying a reverb effect to Soundshape!)
    + Soundshape's parameters ( such as its volume and envelope)
    can be automated from the DAW or controlled with a MIDI 
    controller with your DAW's MIDI-Learn feature.
    
- Standalone
    + Runs as a standalone application. In this mode, Soundshape
    will have an `Options` button at the top of its window.
    This presents options for selecting your MIDI input device,
    audio output device, and a buffer size.
    Samller buffer sizes lead to lower latencies, so try changing
    this setting if you feel like there is too much lag between 
    playing a note and hearing its sound.
    + This mode supports JACK Audio output if JACK is installed
    and running on your system.
    
## User Interface Overview
!["Soundshape GUI"](GUI_diagram.png)

<br/>
<br/>

Soundshape features two time-domain veiws of your data, as you can see 
in this part of the interface.
![](timeWindows.png)
The window at the bottom shows you the entire sound wave that Soundshape
can generate.
It is broken down into 50 sections.
There is a slider with two handles that you can drag over the region
of time that you want to play.
![](doubleSlider.png)

The window above it is a zoomed-in time-domain view. It shows you
the portion of the bottom window that's highlighted in red.
You can click on the button of a section of the zoomed-out window 
to change the section shown in the top window. Then, you can click
on a button of a section in the top window to highlight it. This
screenshot shows the 3rd little section of the 4th big section selected:

![](timeSelection.png)

The tan portions represent the same time interval. 

When you select a section of audio like this, the window at the top changes to
show the frequencies that make up this audio. Right now, we have a section
of silence selected, so theres only 1 bar appearing, and it's at zero.

![](frequenciesZero.png)

New frequency spikes can be added with the `Add` button.
Press the `+` and `-` buttons to zoom in and out of the frequency-domain window.

![](zoomAndAdd.png)

To build a sound out of harmonic frequencies, click the `Harmonic` button.
When you're adding frequencies with harmonic mode enabled, you can only add
harmonics of 440 Hz.
In the editor, harmonic frequencies will show up red, and all others will
show up as blue.
Soundshape will assume that the fundamental frequency of your sound is 
440 Hz, so it's a good idea to always have the first harmonic be nonzero.

![](harmonics.png)

<br/>
<br/>

In addition to editing the *spectrum* of your sound, you can edit its
*envelope*. The envelope is controlled by four parameters:
- Attack
    + How long it takes the sound to fade in when you press a key.
    Make the attack small to make your sound "punchier."
- Decay
    + How long it takes the sound's volume to reach the Sustain level.
- Sustain
    + The volume that a sound remains at while its key is being held.
- Release
    + How long it takes the sound to fade out once you've released its
    key.
    
    ![](envelope.png)

<br/>
<br/>

Soundshape is capable of saving and loading presets so you can always
recall a sound you were working on. There are two types of presets:
- DAW presets
     + If you save your preset in a DAW, then the DAW can load that preset
     back into Soundshape.
     
- Built-in presets.
     + You can use the `Write` button to save your current sound as a preset.
     **Make sure your preset is saved with the `.xml` extension when asked.**
     + The built-in presets in the current preset directory will show up in
     the preset selector. To change which directory you are loading presets
     from, got to `Settings -> preset file path`.
     
![](presets.png)
     
<br/>
<br/>
<br/>

Finally, Soundshape features an `Import` and `Export` button that can be used
to render Soundshape's time domain data to an audio file or to 
read an audio file and change your current sound's spectrum to try to match it.
Supported file types are `.ogg`, `.wav`, and `.flac`.

For more help, you can watch the [Soundshape tutorial videos](@ref Tutorials).