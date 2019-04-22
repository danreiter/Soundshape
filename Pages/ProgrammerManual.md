Programmer's Manual {#ProgrammerManual}
======================

# Soundshape Programmer's Manual

## Overview
Soundshape is written as an audio plugin with the [JUCE Audio 
Framework]( https://juce.com ). The standalone application
is simply the plugin running in a small wrapper program.

Important components are the [Audio Processor]
( @ref Soundshape_pluginAudioProcessor ),
which interacts with the host software and is responsible
for filling an audio buffer with synthesized sound in real time
in response to user input, 
the [Converter]( @ref Converter ), which keeps track of the 
state of the keyboard and actually performs the requiredand
Digital Signal Processing and synthesis, and the [Main GUI Component]
( @ref MainComponent ), which
coinsists of the interface and all of the components that make it up.

File input and output for storing/loading presets and
importing/exporting audio is handled by the 
[IOHandler]( @ref IOHandler ).

## Details
For a more complete listing of the the other components of Soundshape as
well as their descriptions and what their role is, see the
[Class List]( annotated.html ).
