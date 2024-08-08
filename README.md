*DISCLAIMER* To build these projects it is recommended to use Projucer.

JUCE DOWNLOAD LINK:
https://juce.com/get-juce/download

Steps for opening in Projucer:
1. Open Projucer.
2. Click File -> Open.
3. Select "Project Name".jucer in project folder.
4. Click open and Visual Studio button next to 'Selected Exporter'.

**ConvolutionProject** is my final project. ConvolutionReverb, testAudio and testPlugin are experimentations I did at the beginning of the project to learn the ropes of JUCE and are not related to the final project.

The folder 'Release' contains the VST3 file which is the final build of the plugin.

VST3 plugins work only with Windows and Linux. 

To install the plugin copy the VST3 file into your OS VST3 directory and make sure that your DAW is correctly configured to pick it up.

Typical paths for VST3 folders are: 
 C:\Program Files\Common Files\VST3.

PROJECTS::

- 'ConvolutionProject' - Main Project.
- 'testAudio' - white noise generator (DOES NOT REQUIRE DAW TO DEBUG).
- 'testPlugin' - chorus DSP implementation.

*ConvolutionProject and testPlugin require a host DAW to debug. You can attach it by following these steps:*

1. In Visual Studio 2019 set testPlugin_VST3 or ConvolutionProject_VST3 in solution Explorer to startup project.
2. In solution explorer, right click testPlugin_VST3 or ConvolutionProject_VST3 and click properties.
3. Under 'Configuration Properties > Debugging' select 'Command > Browse' and select DAW .exe file (i.e Reaper.exe (x64)).
4. Select apply all and run debug to test.
5. Make sure that your DAW also has the path for VST3's in the plugin debug directory. i.e testPlugin/Builds/VisualStudio2019/x64/Debug/VST3
