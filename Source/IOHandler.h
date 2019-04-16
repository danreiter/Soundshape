
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

/**
Class with helper functions for getting data ready for import/export.

This class has helper functions that can convert a profile object in the converter
into an XML element and also update a converter's state to match a certain XML element.

Soundshape's XML structure is like:
\code
<Soundshape>
  <Parameters>
    <PARAM id="attack" value="2.5e-1"/>
    <PARAM id="beginningChunk" value="0"/>
    <PARAM id="decay" value="2.5e-1"/>
    ...
    ...
  </Parameters>
  <Profile>
    <Chunk index="0">
      <f bin="440" val="5e2"/>
      <f bin="880" val="3e2"/>
      <f bin="1760" val="2e2"/>
      <f bin="2640" val="1e2"/>
      <f bin="3520" val="5e1"/>
      ...
    </Chunk>
    ...
    ...
  </Profile>
</Soundshape>
\endcode
*/
class IOHandler {
public:

    /**
    Creates an XmlElement object that represents a converter's current profile.

    IMPORTANT: The XmlElement that this creates will automatically be deleted if it is added
    to another one as a child element. Don't manually delete it unless you know you're supposed to!

    This method records the converter's profile data points. The resultant
    XmlElement should be able to also be used to restore a converter's state.
    @param converter The Converter with the profile to serialize.
    @return A pointer to the created XmlElement
    */
    static XmlElement* createProfileXML(Converter & converter);

    /**
    Create a XML element that represents the whole state (audio parameters and the profile data).

    This isn't a whole Xml file, but it can be easily made into one with XmlElement::createDocument()
    @param converter The converter whith the profile to serialize
    @param valueTreeState The AudioProcessorValueTreeState that has parameters to be serialized.
    */
    static std::unique_ptr<XmlElement> createStateXML(Converter& converter, AudioProcessorValueTreeState& valueTreeState);

    /**
    Turns the XML element made with createStateXML into an actual XML document.

    @param root The root element of the document.
    @return The string representation of a XML document with the specified root.
    */
    static String createStateDocument(std::unique_ptr<XmlElement> root);

    /**
    Writes Soundshape's state to a file.
    
    If the file doesn't exist, it's created.
    If it does exist, it's completely overwritten by this string.
    @param f The file to write to.
    @param content The content to write to f.
    */
    static void writeStateXMLFile(File f, String content);


    /**
    Restores an AudioProcessorValueTreeState and profile data to the values in a Soundshape parameters XmlElement.

    This can be used for either restoring from a DAW preset or from restoring from a XML file thats saved manually.
    @param valueTreeState The processor's AudioProcessorValueTreeState whose parameters this method should restore to match the XML parameters.
    @param paramsXML The XML data that contains serialized parameters. <b>MAKE SURE THIS IS NOT NULL BEFORE CALLING THIS METHOD. paramsXML
    will be deleted in this method, so don't use it again afterwards.</b>
    */
    static void restoreStateFromXml(AudioProcessorValueTreeState& valueTreeState, Converter& converter,
        std::unique_ptr<XmlElement>& xml);

    /**
     * Exports the entire Soundshape time domain to an audio file whose format is specified by the extension.
     *
     * Supported file extensions are .wav .flac and .ogg. This method throws a SoundshapeAudioExportException
     * if the export process encounters a problem.
     *
     * @param outFile The file that will be created/overwritten.
     * @param converterPtr Pointer to the converter object whose data is being exported.
     */
    static void exportConverterAudio(File& outFile, Converter* converterPtr);

    /**
     * Imports an audio file and overwrites a Converter's profile with its spectral content.
     *
     * The converter performs a spectral analysis on each chunk of audio
     * in the file and overwrites its current profile with this data.
     * Accepted file formats are .wav, .flac, and .ogg.
     * The Converter's profile will be filled up with as much of the audio as it can, and the
     * rest of its profile will be zeros.
     *
     * <b> This will throw a SoundshapeAudioImportException if the import process didn't happen properly.</b>
     * This could occur if there weren't enough samples in the data to fill up even one chunk or if the
     * specified file has a bad format or file extension.
     *
     * @param inFile The file to analyze.
     * @param converterPtr A pointer to the target Converter object.
     */
    static void importConverterAudio(File &inFile, Converter* converterPtr);


    static double exportSampleRate;
    static int bitsPerSample ;
    static int numChannels;
    static int qualityOption;

private:

};




/**
 * Exception for problems during exporting a profile as an audio file
 */
struct SoundshapeAudioExportException : public std::exception {
    SoundshapeAudioExportException():exception(){}

};


/**
 * Exception for problems during importing an audio file into a profile
 */
struct SoundshapeAudioImportException : public std::exception {
    SoundshapeAudioImportException():exception(){}
};
