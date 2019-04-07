#include "IOHandler.h"

XmlElement * IOHandler::createProfileXML(Converter & converter)
{
    XmlElement* result = new XmlElement("Profile");

    // a profile in XML is all of its chunks.
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
        XmlElement* profileChunkElement = new XmlElement("Chunk" + String(i));

        // the chunks all of have children for each of their frequency values.
        // step by 2, we're skipping every other because we also do this in the gui
        for (int freq = 2; freq < 4000; freq += 2) {
            double val = converter.getFrequencyValue(i, freq);
            if (val != 0.0f) {
                XmlElement* binElement = new XmlElement("f:" + String(freq));
                binElement->addTextElement(String(val));
                profileChunkElement->addChildElement(binElement);
            }
        }
        result->addChildElement(profileChunkElement);
    }
    
    return result;
}
