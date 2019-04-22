. {#Installation}
===================

# Soundshape Installation Manual

Steps for getting this software runnong on Windows and Linux are below.

The plugin version of Soundshape needs to run in a host like a DAW.
In case you don't have on, some decent free DAWs (or dedicated hosts) are:
- [Tracktion 7]( https://www.tracktion.com/products/t7-daw )
- [LMMS]( https://lmms.io/ )
- [Cakewalk](https://www.bandlab.com/products/cakewalk)
- [Ardour]( https://ardour.org/ ) 
- [Carla] (https://kx.studio/Applications:Carla)

When using the VST3 plugin format (the default unless you
build for vst2 from source), make sure that your DAW
supports this plugin format.

## To run Soundshape on Windows (Only tested on Windows 10):

Extract Soundshape.zip to C:/Program Files/Common Files/VST3.
Create that folder if it does not exist. Once extracted, Soundshape.exe
can be relocated anywhere desired. We recommend having a folder for
presets in the same folder as Soundshape.exe. Run Soundshape.exe to start
making sounds. Alternatively, open your favorite digital audio workstation,
have it re-scan for plug-ins, and run Soundshape from there. Click
Soundshape's tutorial option to learn more.

## To run Soundshape on Linux: (Only tested on Ubuntu 18.04 and Antergos)
The Linux version of Soundshape is a VST2 plugin, a format
which we are unfortunately not allowed to release source code or binary for.
To run on Linux, build soundshape from source, 
extract the vst .so file to ~/.vst, and scan for it with your DAW.
Create that directory if it does not exist. Instructions for building on
Linux follow.

## Building/Modifying Soundshape
Soundshape can be built from source on platforms supported by the JUCE
framework.
- Grab its source code from the [GitHub repository]( 
https://github.com/danreiter/Soundshape )
- Install JUCE (with the Projucer) from [their website]( https://juce.com ).
- Install the VST3 SDK from [Steinberg's Developer page]( https://www.steinberg.net/en/company/developers.html).
    + **On Linux**, the plugin needs to be built as a VST2. JUCE no longer directly
    includes the headers for the VST2 SDK, but they can be acquired [from this
    old snapshot from JUCE's git repository]
    ( https://github.com/WeAreROLI/JUCE/archive/a54535bc317b5c005a8cda5c22a9c8d4c6e0c67e.zip ).
    Extract the files in
    `modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst2.x/`
    into 
    `(your VST_SDK)/VST2_SDK/pluginterfaces/vst2.x/`
    
    + Add the VST2 SDK path to the Projucer's global paths
    
- Open Soundshaper.jucer in the Projucer
    + **On Linux**, go into the project settings and check the VST Legacy option
    
- Set up your build target.
Two are already configured:
    + Windows: Visual Studio 2017
        * Juce will generate a VS2017 project for you.
        * Open this and build the plugin or standalone app in 
        release mode.
    
    + Linux Makefile
        * Juce will create a Makefile for you in Builds/LinuxMakefile.
        * In this directory, run it with: 
        `make CONFIG=Release -j4`
        