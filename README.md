# media-driver-non-kmd
A KMD-less app+libva+umd execution environment

# code base
- [media-driver](https://github.com/intel/media-driver) (tag-[1e4cfae](https://github.com/intel/media-driver/commit/1e4cfae7c72cde426a93ebbbbfc2103cfe900216))
- [gmmlib](https://github.com/intel/gmmlib) (tag-[b20e484](https://github.com/intel/gmmlib/commit/b20e484a5b4c002bfba40d7464bc9e67fbed5d28))
- [libva](https://github.com/intel/libva) (tag-[b3be72a](https://github.com/intel/libva/commit/b3be72a5a110880f70626d7c3bed953cdde124b2))
- [libva-utils](https://github.com/intel/libva-utils) (tag-[1bea64e](https://github.com/intel/libva-utils/commit/1bea64e12144fce74363538232c1a94ee9c98446))

# build
```bash
# install tools and dependencies
sudo apt install autoconf libtool libdrm-dev xorg xorg-dev openbox libx11-dev libgl1-mesa-glx libgl1-mesa-dev

# build libva
cd libva
./autogen.sh CFLAGS=-g CXXFLAGS=-g
make
sudo make install

# build media-driver
mkdir build_driver
cd build_driver
cmake ../media-driver \
-DCMAKE_INSTALL_PREFIX=/usr \
-DMEDIA_VERSION="2.0.0" \
-DBUILD_ALONG_WITH_CMRTLIB=1 \
-DBS_DIR_GMMLIB=`pwd`/../gmmlib/Source/GmmLib/ \
-DBS_DIR_COMMON=`pwd`/../gmmlib/Source/Common/ \
-DBS_DIR_INC=`pwd`/../gmmlib/Source/inc/ \
-DBS_DIR_MEDIA=`pwd`/../media-driver
make -j4
sudo make install

# build libva-utils
mkdir build_utils
cd build_utils
../libva-utils/autogen.sh CFLAGS=-g CXXFLAGS=-g
make
```