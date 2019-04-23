#include "IOHandler.h"

double IOHandler::exportSampleRate = 48000.0; // hz
int IOHandler::bitsPerSample = 16;
int IOHandler::numChannels = 1;
int IOHandler::qualityOption = 0; // don't know what this does

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

XmlElement * IOHandler::createParamsXML(AudioProcessorValueTreeState & valueTreeState) {
    auto state = valueTreeState.copyState();
    // see the header file for the structure of a soundshape XML file
    XmlElement* paramsXml(state.createXml());
    paramsXml->setTagName("Parameters");
    return paramsXml;
}

std::unique_ptr<XmlElement> IOHandler::createStateXML(Converter & converter, AudioProcessorValueTreeState & valueTreeState)
{
    std::unique_ptr<XmlElement> xml(new XmlElement("Soundshape")); // gives it the "Soundshape" tag at the top
    auto paramsXml = IOHandler::createParamsXML(valueTreeState);
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


void IOHandler::restoreParamsFromXml(AudioProcessorValueTreeState& valueTreeState,
        XmlElement* xml) {

    auto x = xml->createDocument("");
    valueTreeState.replaceState(ValueTree::fromXml(*xml));
}

bool IOHandler::checkIsSoundshapeFile(File _file)
{
	//file handling here
	std::unique_ptr<XmlElement> stateXml(XmlDocument::parse(_file));
	if (stateXml != nullptr) {
		return stateXml->hasTagName("Soundshape");
	}
	return false;
}

void IOHandler::restoreStateFromXml(AudioProcessorValueTreeState& valueTreeState, Converter& converter,
        std::unique_ptr<XmlElement>&  xml)
{
    
    if (xml->hasTagName("Soundshape")) {
        // This is a soundshape preset, hopefully not corrupt
        // Get its parameters and reset the current AudioProcessorValueTreeState's state with it.
        
        XmlElement* paramsXml = xml->getChildByName("Parameters");
        XmlElement* profileXml = xml->getChildByName("Profile");
        IOHandler::restoreParamsFromXml(valueTreeState, paramsXml);
        // iterate through the profile chunks
        XmlElement* chunkXml = profileXml->getFirstChildElement();
        for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
            converter.clearChunk(i);
        }
        while (chunkXml != nullptr) {

            int chunkIndex = chunkXml->getIntAttribute("index");

            XmlElement* frequencyXml = chunkXml->getFirstChildElement();
            while (frequencyXml != nullptr) {

                int bin = (int)frequencyXml->getDoubleAttribute("bin");
                double value = frequencyXml->getDoubleAttribute("val");

                converter.updateFrequencyValue(chunkIndex, bin, (float)value);

                frequencyXml = frequencyXml->getNextElement();
            }

            // get its next sibling for iteration. eventually nullptr
            chunkXml = chunkXml->getNextElement(); 
        }
    }
}


void IOHandler::exportConverterAudio(File& outFile, Converter* converterPtr){

    String ext = outFile.getFileExtension();
    AudioFormatManager formatManager;

    // THIS IS NOT A LEAK. These objects are destroyed by the manager.
    formatManager.registerFormat(new WavAudioFormat(),false);
    formatManager.registerFormat(new FlacAudioFormat(), false);
    formatManager.registerFormat(new OggVorbisAudioFormat(),false);

    AudioFormat* format =  formatManager.findFormatForFileExtension(ext); // doesnt need deleted manually

    // This is deleted by a writer if it can be created, needs deleted manually if that fails.
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
                {""},
                qualityOption
        );

        if(writer != nullptr) {
            // temp storage for rendering one chunk at a time to the export file. Just 1 channel
            AudioBuffer<float> writeBuffer(1, SOUNDSHAPE_CHUNK_SIZE);
            for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
                converterPtr->renderExportChunkToBuffer(i, writeBuffer,exportSampleRate);
                writer->writeFromAudioSampleBuffer(writeBuffer,0,SOUNDSHAPE_CHUNK_SIZE);
            }
            writer->flush();
            delete writer;
        }
        else{
            // writer was nullptr
            delete outStream; // writer didn't delete it
            throw SoundshapeAudioExportException();
        }
    }
    else{
        throw SoundshapeAudioExportException();
    }
}

void IOHandler::importConverterAudio(File &inFile, Converter *converterPtr) {

    String ext = inFile.getFileExtension();
    AudioFormatManager formatManager;

    // THIS IS NOT A LEAK. These objects are destroyed by the manager.
    formatManager.registerFormat(new WavAudioFormat(),false);
    formatManager.registerFormat(new FlacAudioFormat(), false);
    formatManager.registerFormat(new OggVorbisAudioFormat(),false);

    FileInputStream* inStream = new FileInputStream(inFile); // don't delete manually; the reader will delete this.
    AudioFormat* format = formatManager.findFormatForFileExtension(inFile.getFileExtension());
    if(format == nullptr){
        throw SoundshapeAudioImportException();
    }

    if(inStream->openedOk()){
        AudioFormatReader* reader = format->createReaderFor(inStream,true); // reader auto deletes the stream if fails

        if(reader != nullptr){

            // stream is open and we can read this format. Start reading chunks.
            juce::int64 samplesRead = 0;
            int chunk = 0;
            int bufferSize = SOUNDSHAPE_CHUNK_SIZE;
            AudioBuffer<float> readBuffer(1, bufferSize);
            // Keep reading chunks until there aren't enough samples left.
            // We should throw an exception if there aren't enough samples in the
            // file to even fill up a single chunk.
            while(samplesRead + bufferSize < reader->lengthInSamples){
                reader->read(&readBuffer, 0,bufferSize,samplesRead,true,true);
                samplesRead += bufferSize;
                converterPtr->analyzeChunkIntoProfile(chunk, readBuffer, reader->sampleRate);
                chunk++;
            }
            if(chunk == 0){
                // we never entered the while loop
                throw SoundshapeAudioImportException();
            }

            delete reader;
        }
        else{
            throw SoundshapeAudioImportException();
        }
    }
    else{
        // stream didn't open correctly at all
        throw SoundshapeAudioImportException();
    }

}
