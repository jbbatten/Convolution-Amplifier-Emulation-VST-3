*DISCLAIMER* These projects need to be opened with Projucer and there you can choose an exporter, I recommend visual studio 2022 as that was the IDE I used for the majority of the project.

JUCE LIBRARY DOWNLOAD LINK:
https://juce.com/get-juce/download

You can also open projects from Projucer by selecting the .jucer file extension.

Steps for opening in Projucer:
1. Open Projucer.
2. Click File -> Open.
3. Select "Project Name".jucer in project folder.
4. Click open and Visual Studio Code button next to 'Selected Exporter'

**ConvolutionProject** is my final project. ConvolutionReverb, testAudio and testPlugin are experimentations I did at the beginning of the project to learn the ropes of JUCE and are not related to the final outcome.

The folder 'Release' contains the VST3 file which is the final build of the plugin.

VST3 works only with windows and linux. 

To install the plugin drag the VST3 file into your VST3 directory and make sure that you DAW is searching that folder for the plugin.

typical paths for VST3 folders are: 
 C:\Program Files\Common Files\VST3.

The plugin will most likely display in your DAW under 'yourCompany' and will have the same name as the project folder name, i.e ConvolutionProject.

PROJECTS::

- 'ConvolutionProject' - Main Project.
- 'testAudio' - white noise generator (DOES NOT REQUIRE DAW TO DEBUG).
- 'testPlugin' - chorus DSP implementation.

*ConvolutionProject and testPlugin require a host DAW to debug. You can attach it by following these steps:*

1. In Visual Studio Code 2019 set testPlugin_VST3 or ConvolutionProject_VST3 in solution Explorer to startup project.
2. In solution explorer, right click testPlugin_VST3 or ConvolutionProject_VST3 and click properties.
3. Under 'Configuration Properties > Debugging' select 'Command > Browse' and select DAW .exe file (i.e Reaper.exe (64 BIT VERSION IS REQUIRED)).
4. Select apply all and run debug to test.
5. Make sure that your DAW also has the path for VST3's in the plugin debug directory. i.e testPlugin/Builds/VisualStudio2019/x64/Debug/VST3
