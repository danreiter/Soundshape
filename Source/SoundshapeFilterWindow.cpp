
#include "SoundshapeFilterWindow.h"
#include "../JuceLibraryCode/JuceHeader.h"

/**
This file was originally part of the JUCE framework
   Copyright (c) 2017 - ROLI Ltd.
   from the file juce_StandaloneFilterApp.cpp

   Modified 2019 by Mardigon Toler for Soundshape.

   This is the JUCEApplication that runs a SoundshapeFilterWindow, which
   in turn contains the Soundshape_pluginAudioProcessorEditor which draws
   the main GUI component, MainComponent.


*/
    class SoundshapeFilterApp : public JUCEApplication
    {
    public:
        SoundshapeFilterApp()
        {
            PluginHostType::jucePlugInClientCurrentWrapperType = AudioProcessor::wrapperType_Standalone;

            PropertiesFile::Options options;

            options.applicationName = getApplicationName();
            options.filenameSuffix = ".settings";
            options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX
            options.folderName = "~/.config";
#else
            options.folderName = "";
#endif

            appProperties.setStorageParameters(options);
        }

        const String getApplicationName() override { return JucePlugin_Name; }
        const String getApplicationVersion() override { return JucePlugin_VersionString; }
        bool moreThanOneInstanceAllowed() override { return true; }
        void anotherInstanceStarted(const String&) override {}

        virtual SoundshapeFilterWindow* createWindow()
        {
#ifdef JucePlugin_PreferredChannelConfigurations
            StandalonePluginHolder::PluginInOuts channels[] = { JucePlugin_PreferredChannelConfigurations };
#endif

            return new SoundshapeFilterWindow(getApplicationName(),
                LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                appProperties.getUserSettings(),
                false, {}, nullptr
#ifdef JucePlugin_PreferredChannelConfigurations
                , juce::Array<StandalonePluginHolder::PluginInOuts>(channels, juce::numElementsInArray(channels))
#else
                , {}
#endif
#if JUCE_DONT_AUTO_OPEN_MIDI_DEVICES_ON_MOBILE
                , false
#endif
            );
        }

        //==============================================================================
        void initialise(const String&) override
        {
            mainWindow.reset(createWindow());

#if JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
            Desktop::getInstance().setKioskModeComponent(mainWindow.get(), false);
#endif
            mainWindow->setUsingNativeTitleBar(false);
            mainWindow->setVisible(true);
        }

        void shutdown() override
        {
            mainWindow = nullptr;
            appProperties.saveIfNeeded();
        }

        //==============================================================================
        void systemRequestedQuit() override
        {
            if (mainWindow.get() != nullptr)
                mainWindow->pluginHolder->savePluginState();

            if (ModalComponentManager::getInstance()->cancelAllModalComponents())
            {
                Timer::callAfterDelay(100, []()
                {
                    if (auto app = JUCEApplicationBase::getInstance())
                        app->systemRequestedQuit();
                });
            }
            else
            {
                quit();
            }
        }

    protected:
        ApplicationProperties appProperties;
        std::unique_ptr<SoundshapeFilterWindow> mainWindow;
    };

START_JUCE_APPLICATION(SoundshapeFilterApp)
