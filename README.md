# Building the game
## Using the installer
Running `setup.exe` in the `install/` directory will automate the installation of the game on a Windows machine. Note that the game will ask for administrator privileges as otherwise it can't save the users high score.

## From the source code
The game can be simply built from the source code by creating a Visual Studio project from it or by manually invoking a compiler in a Cygwin terminal -- e.g. `g++ *.cpp -O3 -o game && ./game` -- as long as the appropriate Windows SDK is installed.
