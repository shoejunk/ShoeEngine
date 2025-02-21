@echo off
echo Running unit tests...
cd build
ctest -C Release --output-on-failure > ..\test_output.txt
cd ..
IF %ERRORLEVEL% EQU 0 (
    echo Tests succeeded! Output saved to test_output.txt
) ELSE (
    echo Tests failed! Check test_output.txt for details
)

