#!/bin/bash
echo -e "\e[1;32mUpdating Submodules\e[0m"
git submodule update --init --recursive --remote
echo -e "\e[1;32mEntering SDL-Mixer-X source\e[0m"
cd SDL-Mixer-X
echo -e "\e[1;33m> mkdir build\e[0m"
mkdir build
echo -e "\e[1;33m> cd build\e[0m"
cd build
echo -e "\e[1;32mConfiguring SDL-Mixer-X without LGPL/GPL code... If you want this changed, check this script!\e[0m"
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=output -DDOWNLOAD_AUDIO_CODECS_DEPENDENCY=ON -DAUDIO_CODECS_BUILD_LOCAL_SDL2=ON -DAUDIOCODECS_BUILD_OPUS=OFF -DAUDIOCODECS_BUILD_MPG123=OFF ..
echo -e "\e[1;32mCompiling...\e[0m"
cmake --build . --config Release -- -j12
echo -e "\e[1;32mGenerating Install Directory...\e[0m"
make install
echo -e "\e[1;32mCompiling finished, moving to lib/inc folder...\e[0m"
echo -e "\e[1;33m> cd output\e[0m"
cd output
echo -e "\e[1;33m> mv lib ../../../\e[0m"
mv lib ../../../
echo -e "\e[1;33m> cd ..\e[0m"
cd ..
if test -d ./lib64; then
    echo -e "\e[1;33m> lib64/* ../../lib\e[0m"
    mv lib64/* ../../lib
fi
if test -d ./external/AudioCodecs/src/AudioCodecs-build/lib64/; then
    echo -e "\e[1;33m> mv ./external/AudioCodecs/src/AudioCodecs-build/lib64/** ../../lib\e[0m"
    mv ./external/AudioCodecs/src/AudioCodecs-build/lib64/** ../../lib
fi
echo -e "\e[1;33m> mv include/** ../../include\e[0m"
mv include/** ../../include
echo -e "\e[1;33m> cd ..\e[0m"
cd ..
echo -e "\e[1;33m> rm -rf output\e[0m"
# rm -rf output
echo -e "\e[1;32mDone! You should be able to compile now.\e[0m"
