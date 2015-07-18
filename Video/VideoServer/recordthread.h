#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

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

class RecordThread : public QThread
{
    Q_OBJECT
public:

    RecordThread();
    virtual ~RecordThread(){
        delete []frametest;
        frametest = 0;
    }


signals:
protected:
    void run();
public slots:
    void quit();
private:
    VideoCapture *video_capture;
    MFCH264Encoder *h264_encoder;
    unsigned char *frametest;
    volatile bool stopped;

};

#endif // RECORDTHREAD_H
