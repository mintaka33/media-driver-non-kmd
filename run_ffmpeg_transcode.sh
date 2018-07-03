export LD_PRELOAD=./build_driver/media_driver/linux/ult/libdrm_mock/libdrm_mock.so

./build_ffmpeg/ffmpeg_g -hwaccel vaapi -hwaccel_output_format vaapi -i ~/test.mp4 -vframes 2000 -c:v h264_vaapi ~/out.mp4 -y