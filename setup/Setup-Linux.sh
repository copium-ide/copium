#!/bin/bash

pushd ..
./vendor/bin/Premake/Linux/premake5 --cc=clang --file=build.lua gmake2
popd
