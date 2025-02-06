@echo off
echo Running unit tests...
cd build
ctest -C Release --output-on-failure
cd ..
echo Done!
