## Under construction!

# How to compile osci-render and sosci

This guide covers the process of compiling osci-render and Sosci for your local machine, as on MacOS, packaging the compiled software for distribution to other computers is complicated and requires paying Apple $100 per year.

### Table of contents:
- [Windows](#Windows)
- [MacOS](#MacOS)
- [Linux](#Linux)

### Guide completion status:
- Windows: Needs troubleshooting section
- MacOS: Todo
- Linux: Todo

-----

# Windows

### Prerequisites
1. Install Visual Studio 2022 with the "Desktop Development with C++" workload
2. Download and unpack [JUCE](https://juce.com/download/) to a memorable location
3. Install [Git for Windows](https://git-scm.com/downloads/win)

### Repository setup
1. Open a Git Bash terminal
2. Navigate to a memorable directory (such as `cd ~/Documents`)
3. Clone the osci-render Github repository with all submodules:
    - For stable release: `git clone https://github.com/jameshball/osci-render --recurse-submodules`
	- For development builds: `git clone https://github.com/jameshball/osci-render --recurse-submodules --branch develop`

### Compilation
WARNING: if you previously compiled osci-render and want to compile Sosci, or vice versa, make sure to delete the JuceLibraryCode folder and then save the appropriate project before you follow the steps below! If you don't, it will likely fail to compile and give you a bunch of errors.

1. Open Projucer (from your JUCE installation)
2. Open osci-render.jucer or sosci.jucer in Projucer
3. (optional but recommended) Enable the VST plugin copy step:
    - In the Exporters tab on the left side, select Visual Studio 2022 > Release, and change "Enable Plugin Copy Step" to Enabled. This will make Visual Studio automatically copy the VST plugin to your system location whenever you build it.
	- Note that if you already have the precompiled version of osci-render or Sosci installed, you'll have to delete it before doing this, as the folder structure is different and it can cause issues. If so, go to `C:\Program Files\Common Files\VST3` and delete osci-render.vst3
4. In the top middle, press the Visual Studio icon to create and open the Visual Studio project.
5. In the top area of Visual Studio, if the dropdown says "Debug", change it to "Release" to enable optimization. If you don't, it'll run terribly, as debug builds are only for testing and debugging.
6. In the top bar of Visual Studio, select Build > Build All. This step will likely max out your CPU, and it often takes a while, especially on slower hardware.
7. Congratulations! You have built osci-render or Sosci with all premium features enabled! If you enabled the plugin copy step, the VST has been copied to the appropriate location, and you can also find the completed files in these locations:
	- osci-render standalone: `Builds/osci-render/VisualStudio2022/x64/Release/Standalone Plugin/osci-render.exe`
	- Sosci standalone: `Builds/sosci/VisualStudio2022/x64/Release/Standalone Plugin/sosci.exe`
    - osci-render VST: `Builds/osci-render/VisualStudio2022/x64/Release/VST3/osci-render.vst3/Contents/x86_64-win/osci-render.vst3`
	- Sosci VST - `Builds/sosci/VisualStudio2022/x64/Release/VST3/sosci.vst3/Contents/x86_64-win/sosci.vst3`

-----

# MacOS

TODO

-----

# Linux

TODO

-----
