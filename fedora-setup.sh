git submodule update --init --recursive
sudo dnf install libXext-devel
sudo dnf install pulseaudio-libs-devel alsa-libs-devel
sudo dnf install SDL2_ttf-devel
sudo dnf install SDL2_image-devel glew-devel glm-devel
sudo dnf install cmake

#may not be necessary \/ \/
sudo dnf groupinstall "Development Tools" "Development Libraries"
./compilermixer.sh
