#!/bin/sh
apt-get install gcc-11 g++-11 --yes
# Force GCC/G++ 11 to be default
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11