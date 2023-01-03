# FileIndexGUI

## Windows Deployment (pain)
Firstly create your build (preferrably in QtCreator), then open the Visual Studio developer console
 > **Note**
 >
 > Make sure you use the Visual Studio developer console, else the compiler runtime does not get installed!

Then execute the command in the resulting build directory:
```bash
<cmake> --install <buildDir> --prefix <targetDir>
```
Where:
 - `<cmake>` Is the path to cmake: `Qt\Tools\CMake...\bin\cmake.exe`
 - `<buildDir>` Is the directory your build has been made in
 - `<targetDir>` Is the directory you want to install `FileIndexGUI` in
