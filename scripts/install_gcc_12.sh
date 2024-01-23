#!/bin/sh
apt-get install gcc-12 g++-12 --yes
# Force GCC/G++ 12 to be default
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 12