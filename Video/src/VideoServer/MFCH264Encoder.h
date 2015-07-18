#ifndef MFCH264ENCODER_H
#define MFCH264ENCODER_H
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
#include <pthread.h>
#include <linux/videodev2.h>
#include <semaphore.h>
#include <math.h>
#include "s3c_pp.h"
#include "SsbSipH264Encode.h"
#include "LogMsg.h"
#include "performance.h"
#include "MfcDriver.h"
}
#define MEDIA_FILE_NAME  "/tmp/cam_encoding.264"
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define YUV_FRAME_BUFFER_SIZE (320*240*3)>>1

static void *mfc_encoder_init(int width, int height, int frame_rate, int bitrate, int gop_num);
static void *mfc_encoder_exe(void *handle, unsigned char *yuv_buf, int frame_size, int first_frame, long *size);
static void mfc_encoder_free(void *handle);

class MFCH264Encoder {
public:

    unsigned char   *encoded_buf;
    long            encoded_size;
    MFCH264Encoder()
    {

        encoded_size = 0;
    }
    void init()
    {
        frame_count = 0;
        // 30fps,bitrate = 1000* 30,gop = (15,25)
        handle = mfc_encoder_init(VIDEO_WIDTH, VIDEO_HEIGHT, 30, 30000, 15);
        if (handle == 0) {
           perror("cannot init mfc encoder");
        }
        encoded_fp = fopen(MEDIA_FILE_NAME, "wb+");
        if (encoded_fp == 0) {
             perror("cannot open /tmp/cam_encoding.264");
        }
    }

    void Encode( unsigned char *yuv420frame)
    {
        frame_count++;
        if(frame_count == 1)
                //mfc_encoder_exe函数
            encoded_buf = (unsigned char*)mfc_encoder_exe(handle, yuv420frame, YUV_FRAME_BUFFER_SIZE, 1, &encoded_size);
        else
            encoded_buf = (unsigned char*)mfc_encoder_exe(handle, yuv420frame, YUV_FRAME_BUFFER_SIZE, 0, &encoded_size);


        fwrite(encoded_buf, 1, encoded_size, encoded_fp);

    }

    void Encode_for_send( unsigned char *yuv420frame)
    {
        frame_count++;
        if(frame_count == 1)
                //mfc_encoder_exe函数
            encoded_buf = (unsigned char*)mfc_encoder_exe(handle, yuv420frame,
                                                      YUV_FRAME_BUFFER_SIZE, 1, &encoded_size);
        else
            encoded_buf = (unsigned char*)mfc_encoder_exe(handle, yuv420frame,
                                                          YUV_FRAME_BUFFER_SIZE, 0, &encoded_size);
    }
    void Encoder_stop()
    {
        mfc_encoder_free(handle);
        fclose(encoded_fp);
        encoded_buf = 0;
    }

    int getFrameNumber()
    {
        return frame_count;
    }

    ~MFCH264Encoder() {

    }

private:
    int frame_count;
    void* handle;
    FILE* encoded_fp;

};

void *mfc_encoder_init(int width, int height, int frame_rate, int bitrate, int gop_num)
{
        void	*handle;
        int	ret;
        int frame_size;
        frame_size	= (width * height * 3) >> 1;

        handle = SsbSipH264EncodeInit(width, height, frame_rate, bitrate, gop_num);
        if (handle == NULL) {
                LOG_MSG(LOG_ERROR, "Test_Encoder", "SsbSipH264EncodeInit Failed\n");
                return NULL;
        }

        ret = SsbSipH264EncodeExe(handle);

        return handle;
}

void *mfc_encoder_exe(void *handle, unsigned char *yuv_buf, int frame_size, int first_frame, long *size)
{
        unsigned char	*p_inbuf, *p_outbuf;
        int		hdr_size;
        int		ret;
        p_inbuf = (unsigned char*)SsbSipH264EncodeGetInBuf(handle, 0);
        memcpy(p_inbuf, yuv_buf, frame_size);

        ret = SsbSipH264EncodeExe(handle);
        if (first_frame) {
                SsbSipH264EncodeGetConfig(handle, H264_ENC_GETCONF_HEADER_SIZE, &hdr_size);
                printf("Header Size : %d\n", hdr_size); //debug
        }

        p_outbuf = (unsigned char*)SsbSipH264EncodeGetOutBuf(handle, size);

        return p_outbuf;
}

void mfc_encoder_free(void *handle)
{
        SsbSipH264EncodeDeInit(handle);
}


#endif // MFCH264ENCODER_H
