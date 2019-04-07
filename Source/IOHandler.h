
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

/**
Class with helper functions for getting data ready for import/export.

This class has helper functions that can convert a profile object in the converter
into an XML element and also update a converter's state to match a certain XML element.
*/
class IOHandler {
public:

    /**
    Creates an XmlElement object that represents a converter's current profile.

    IMPORTANT: The XmlElement that this creates will automatically be deleted if it is added
    to another one as a child element. Don't manually delete it unless you know you're supposed to!

    This method records the converter's current sample rate and all of its data points. The resultant
    XmlElement should be able to also be used to restore a converter's state.
    */
    static XmlElement* createProfileXML(Converter & converter);



private:

};