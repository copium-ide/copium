@echo off

pushd ..
vendor\bin\Premake\Windows\premake5.exe --file=build.lua vs2022
popd
pause