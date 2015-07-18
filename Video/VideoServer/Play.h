#ifndef PLAY_H
#define PLAY_H

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <linux/fb.h>
#include <signal.h>

#include "SsbSipH264Decode.h"
#include "FrameExtractor.h"
#include "H264Frames.h"
#include "LogMsg.h"
#include "lcd.h"
#include "s3c_pp.h"
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
}

#define MEDIA_FILE_NAME "/tmp/cam_encoding.264"
#define LCD_BPP_V4L2        V4L2_PIX_FMT_RGB565
#define VIDEO_WIDTH   320
#define VIDEO_HEIGHT  240
#define PP_DEV_NAME     "/dev/s3c-pp"
#define INPUT_BUFFER_SIZE       (204800)
#define FB0_BPP         16
#define FB0_COLOR_SPACE RGB16

class Play{
public:
	Play();
	~Play();
	void FBOpen();
	int playback();
        int init_for_record();
        int display_for_record(unsigned char* h264frame);
        void stop_for_record();
	static void sig_del_h264(int signo);
private:
	int FriendlyARMWidth, FriendlyARMHeight;

	unsigned char delimiter_h264[4];

};
#endif // PLAY_H
