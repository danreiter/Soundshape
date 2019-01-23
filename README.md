# Soundshape
## Gregory Hughes, Daniel Reiter, & Mardigon Toler

Soundshape is a software synthesizer that creates sound from an easy-to-use spectrum editor.

## Features
- Cross platform (Windows, Linux)
- Available as a plugin (VST3)  or as a standalone application
- Easily edit your presets harmonics
- Control with MIDI or with an on-screen keyboard


## Building
### Requirements 
- [JUCE 5](https://shop.juce.com/get-juce)

### Building for Windows x64 (Visual C++)
Soundshape relies on the [FFTW3 library](https://www.fftw.org).
Extract the Windows DLLs into a folder called ```FFTW_Windows``` that's in the same directory as the `.jucer` file. Enter this directory in Visual Studio's Developer Command Prompt and run
`lib /machine:x64 /def:libfftw3-3.def`.


Open the `.jucer` file in the Projucer, and export a Visual Studio Project. 

### Building for Linux
...


