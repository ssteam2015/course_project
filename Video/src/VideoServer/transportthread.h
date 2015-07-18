#ifndef TRANSPORTTHREAD_H
#define TRANSPORTTHREAD_H

#include <QWidget>
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
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
}
#include <iostream>
#include <string>
#include <QtGui>
#include<QThread>

#include "v4l2capture.h"
#include "MFCH264Encoder.h"
#include "rtpsend.h"


class TransportThread : public QThread
{
    Q_OBJECT
public:
 TransportThread();
 virtual ~TransportThread(){
     delete []frametest;
     frametest = 0;
     delete []h264frame;
     h264frame = 0;
 }

 std::string dest_ip;
 unsigned int  portbase, destport;

signals:
protected:
    void run();
public slots:
    void quit();
private:
    VideoCapture *video_capture;
    MFCH264Encoder *h264_encoder;
    RTPsend *rtp_send;
    unsigned char *frametest;
    unsigned char *h264frame;
    volatile bool stopped;


};

#endif // TRANSPORTTHREAD_H
