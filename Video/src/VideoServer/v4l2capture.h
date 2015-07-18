#ifndef V4L2CAPTURE_H
#define V4L2CAPTURE_H
#include <iostream>
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
#define CLEAR(x) memset (&(x), 0, sizeof (x))
class VideoCapture {
public:
    int  init_capture();  //(int width ,int height ,const char* dev ) 图象采集初始化函数，采集宽度、高度、设备路径
    int  read_frame(unsigned char *frame_addr); //图象采集函数，使用frameaddr
    int  stop_capture();
    ~VideoCapture();
    int read_frame_count;
private:
    struct buffer {
        void *       start;
        size_t       length;


    };

    int      fd  ;
    struct buffer *buffers ;
    unsigned int  n_buffers;
    unsigned char *FrameAddr;

};




#endif // V4L2CAPTURE_H
