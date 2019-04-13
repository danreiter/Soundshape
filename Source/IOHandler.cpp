#include "IOHandler.h"

XmlElement * IOHandler::createProfileXML(Converter & converter)
{
    XmlElement* result = new XmlElement("Profile");

    // a profile in XML is all of its chunks.
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
        XmlElement* profileChunkElement = new XmlElement("Chunk");
        profileChunkElement->setAttribute("index", i);
        // the chunks all of have children for each of their frequency values.
        // step by 2, we're skipping every other because we also do this in the gui
        for (int freq = 2; freq < 4000; freq += 1) {
            double val = converter.getFrequencyValue(i, freq);
            if (val != 0.0f) {
                XmlElement* binElement = new XmlElement("f");
                //binElement->addTextElement(String(val));
                binElement->setAttribute("bin", freq);
                binElement->setAttribute("val", val);
                profileChunkElement->addChildElement(binElement);
            }
        }
        result->addChildElement(profileChunkElement);
    }
    
    return result;
}

std::unique_ptr<XmlElement> IOHandler::createStateXML(Converter & converter, AudioProcessorValueTreeState & valueTreeState)
{
    auto state = valueTreeState.copyState();
    
    std::unique_ptr<XmlElement> xml(new XmlElement(state.getType())); // gives it the "Soundshape" tag at the top
    XmlElement* paramsXml(state.createXml());
    paramsXml->setTagName("Parameters");
    xml->addChildElement(paramsXml);
    XmlElement *profileXML = IOHandler::createProfileXML(converter); // stores profile frequency data
    xml->addChildElement(profileXML);
    return xml;
}

String IOHandler::createStateDocument(std::unique_ptr<XmlElement> root)
{
    return root->createDocument("", false, true, "UTF-8", 60);
}

void IOHandler::writeStateXMLFile(File f, String content)
{
    FileOutputStream ostream(f);
    if (ostream.openedOk()) {
        ostream.setPosition(0);
        ostream.truncate();
        ostream.writeText(content, false, false, nullptr);
    }
}

void IOHandler::restoreStateFromXml(AudioProcessorValueTreeState& valueTreeState, Converter& converter,
    XmlElement*  xml)
{

    if (xml->hasTagName(valueTreeState.state.getType())) {
        // This is a soundshape preset, hopefully not corrupt
        // Get its parameters and reset the current AudioProcessorValueTreeState's state with it.

        XmlElement* paramsXml = xml->getChildByName("Parameters");
        XmlElement* profileXml = xml->getChildByName("Profile");

        valueTreeState.replaceState(ValueTree::fromXml(*paramsXml));

        // iterate through the profile chunks
        XmlElement* chunkXml = profileXml->getFirstChildElement();
        while (chunkXml != nullptr) {

            int chunkIndex = chunkXml->getIntAttribute("index");

            XmlElement* frequenyXml = chunkXml->getFirstChildElement();
            while (frequenyXml != nullptr) {

                int bin = (int)frequenyXml->getDoubleAttribute("bin");
                double value = frequenyXml->getDoubleAttribute("val");

                converter.updateFrequencyValue(chunkIndex, bin, value);

                frequenyXml = frequenyXml->getNextElement();
            }

            // get its next sibling for iteration. eventually nullptr
            chunkXml = chunkXml->getNextElement(); 
        }
    }
    delete xml;
}


bool IOHandler::exportConverterAudio(File& outFile, Converter* converterPtr){

    String ext = outFile.getFileExtension();
    AudioFormatManager formatManager;

    // THIS IS NOT A LEAK. These objects are destroyed by the manager.
    formatManager.registerFormat(new WavAudioFormat(),false);
    formatManager.registerFormat(new FlacAudioFormat(), false);
    formatManager.registerFormat(new OggVorbisAudioFormat(),false);

    AudioFormat* format =  formatManager.findFormatForFileExtension(ext); // doesnt need deleted manually

    double exportSampleRate = 48000.0; // hz
    int bitsPerSample = 16;
    int numChannels = 1;
    StringPairArray metadata({""}); // empty metadata
    int qualityOption = 10; // don't know what this does

    // THIS IS NOT A LEAK. This will be used in a writer which should delete it.
    FileOutputStream* outStream = new FileOutputStream(outFile);
    // truncate the file if it exists, and start writing
    if(outStream->openedOk()){

        outStream->setPosition(0);
        outStream->truncate();

        AudioFormatWriter* writer = format->createWriterFor(
                outStream,
                exportSampleRate,
                numChannels,
                bitsPerSample,
                metadata,
                qualityOption
        );

        if(writer != nullptr) {
            // temp storage for rendering one chunk at a time to the export file. Just 1 channel
            AudioBuffer<float> writeBuffer(1, SOUNDSHAPE_CHUNK_SIZE);
            for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
                converterPtr->renderExportChunkToBuffer(i, writeBuffer,exportSampleRate);
                writer->writeFromAudioSampleBuffer(writeBuffer,0,SOUNDSHAPE_CHUNK_SIZE);
            }

        }
        writer->flush();
        delete writer;
        return true;
    }
    return false;
}
