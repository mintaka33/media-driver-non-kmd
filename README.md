# media-driver-non-kmd
A KMD-less app+libva+umd running environment

# code base
- [media-driver](https://github.com/intel/media-driver) (tag-[1e4cfae](https://github.com/intel/media-driver/commit/1e4cfae7c72cde426a93ebbbbfc2103cfe900216))
- [gmmlib](https://github.com/intel/gmmlib) (tag-[b20e484](https://github.com/intel/gmmlib/commit/b20e484a5b4c002bfba40d7464bc9e67fbed5d28))
- [libva](https://github.com/intel/libva) (tag-[b3be72a](https://github.com/intel/libva/commit/b3be72a5a110880f70626d7c3bed953cdde124b2))
- [libva-utils](https://github.com/intel/libva-utils) (tag-[1bea64e](https://github.com/intel/libva-utils/commit/1bea64e12144fce74363538232c1a94ee9c98446))
- [FFmpeg](https://github.com/FFmpeg/FFmpeg) (tag-[efde2a7](https://github.com/FFmpeg/FFmpeg/commit/efde2a70ffe6d8f5b53d0d1ea9c9c54fe9d979d4))
- [MediaSDK](https://github.com/Intel-Media-SDK/MediaSDK) (tag-[e9a09d8](https://github.com/Intel-Media-SDK/MediaSDK/commit/e9a09d89d5b20756668243eb0ada75cb826d6294))
# build
```bash
# install tools and dependencies
sudo apt install autoconf libtool libdrm-dev xorg xorg-dev \
openbox libx11-dev libgl1-mesa-glx libgl1-mesa-dev

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

# FFmpeg

build ffmpeg
```bash
sudo apt update
sudo apt install \
  autoconf \
  automake \
  build-essential \
  cmake \
  git \
  libass-dev \
  libfreetype6-dev \
  libsdl2-dev \
  libtool \
  libvorbis-dev \
  libxcb1-dev \
  libxcb-shm0-dev \
  libxcb-xfixes0-dev \
  pkg-config \
  texinfo \
  wget \
  zlib1g-dev \
  nasm
  
 sudo apt install libx264-dev libx265-dev
 
 mkdir build_ffmpeg
 cd build_ffmpeg
../FFmpeg/configure --enable-libx264 --enable-libx265 --enable-gpl \
--enable-debug=3 --disable-optimizations 

make -j8
```

run ffmpeg
```bash
# set environment
export LD_PRELOAD=../build_driver/media_driver/linux/ult/libdrm_mock/libdrm_mock.so

# run transcode
./ffmpeg -hwaccel vaapi -hwaccel_output_format vaapi -i test.mp4 \
-vframes 1000 -c:v h264_vaapi out.mp4 -y

# transcode with vpp scaling
./ffmpeg -hwaccel vaapi -hwaccel_output_format vaapi -i test.mp4 \
-vf 'scale_vaapi=640:480:nv12' -vframes 1000 -c:v h264_vaapi out.mp4 -y
```

# MSDK

build MediaSDK
```bash
mkdir build_msdk
cd build_msdk
cmake ../msdk -DCMAKE_DEBUG_TYPE=Debug
make -j8
sudo make install
```

run MSDK sample application
```bash
# set environment
export LD_PRELOAD=../build_driver/media_driver/linux/ult/libdrm_mock/libdrm_mock.so

# decode
./sample_decode h264 -i ~/test.264

# transcode
./sample_multi_transcode -i::h264 ~/test.264 -o::h264 out.264
```

