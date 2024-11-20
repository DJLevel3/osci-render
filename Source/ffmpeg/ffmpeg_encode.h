// Uncomment the below define to enable using
// FFMPEG to encode the exports to video. This
// also requires a full rebuild of the project!
// Meaning, you have to recompile every file,
// not just the ones that are detected as
// changed by uncommenting the define. Also,
// there are some dependencies required for this
// to work, which are listed below:
// 
// On Windows:
// - MinGW is installed and set up
// - mingw-w64-x264 has been installed on MinGW
//   (with all dependencies)
// - FFMPEG has been built from source and
//   installed via MinGW, configured using at
//   minimum the following options:
//     ./configure --prefix=C:\ffmpeg \
//     --enable-shared --enable-gpl \
//     --enable-libx264
// - C:\ffmpeg is in PATH, and is BEFORE the
//   MinGW directory in PATH
// 
// TODO: MacOS dependencies
// 

#define USE_FFMPEG

#ifdef USE_FFMPEG
#include <stdint.h>
#include <iostream>

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libavutil\rational.h>
#include <libswscale\swscale.h>
#include <libavutil\opt.h>
#include <libavutil\error.h>
}

// presets:
//
// ultrafast
// superfast
// veryfast
// faster
// fast
// medium – default preset
// slow
// slower
// veryslow


class FfmpegEncoder
{
public:
	struct Params
	{
		uint32_t width;
		uint32_t height;
		double fps;
		uint32_t bitrate;
		const char *preset;

		uint32_t crf; //0–51

		enum AVPixelFormat src_format;
		enum AVPixelFormat dst_format;
	};

	FfmpegEncoder() = default;
	FfmpegEncoder(const char *filename, const Params &params);
	~FfmpegEncoder();

	bool Open(const char *filename, const Params &params);

	void Close();

	bool Write(const unsigned char *data);

	bool IsOpen() const;

private:
	bool FlushPackets();

private:
	bool mIsOpen = false;

	struct Context
	{
		struct AVFormatContext *format_context = nullptr;
		struct AVStream *stream = nullptr;
		struct AVCodecContext *codec_context = nullptr;
		struct AVFrame *frame = nullptr;
		struct SwsContext *sws_context = nullptr;
		AVCodecID codec = AV_CODEC_ID_H264;
		
		uint32_t frame_index = 0;
	};

	Context mContext = {};
};

#endif