git submodule update --init --recursive
sudo dnf install libXext-devel
sudo dnf install pulseaudio-libs-devel alsa-libs-devel
sudo dnf install SDL2_ttf-devel
sudo dnf install SDL2_image-devel glew-devel glm-devel
sudo dnf install cmake

#may not be necessary \/ \/
sudo dnf groupinstall "Development Tools" "Development Libraries"
echo -e "\e[1;32mDone! You can now go ahead and compile the SDL_Mixer_X library with the compilemixer.sh script.\e[0m"
