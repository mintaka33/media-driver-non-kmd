
export LD_PRELOAD=./build_driver/media_driver/linux/ult/libdrm_mock/libdrm_mock.so

./build_msdk/__bin/Debug/sample_multi_transcode -i::h264 ~/test.264 -o::h264 ~/out.264
