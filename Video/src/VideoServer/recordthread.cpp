#include "recordthread.h"

#define BUFF_SIZE 10
#define FRAME_SIZE 130000
#define YUV420_SIZE 320*240*3<<1

RecordThread::RecordThread()
{
    frametest = new unsigned char[FRAME_SIZE];

}
void RecordThread::run()
{
    video_capture = new VideoCapture();
    h264_encoder = new MFCH264Encoder();
    h264_encoder -> init();
    video_capture -> init_capture();
    stopped = false;
    while(!stopped)
    {
         video_capture->read_frame(frametest);
         h264_encoder->Encode(frametest);
         usleep(10*1000);
     }

}
void RecordThread::quit()
{
    //stop record
    stopped = true;
    fprintf(stderr,"%d  frames is encoded finally  \n",h264_encoder->getFrameNumber());
    h264_encoder->Encoder_stop();
    video_capture->stop_capture();
    fprintf(stderr,"stop  is ok \n");
    delete video_capture;
    video_capture = 0;
     fprintf(stderr,"delete capture is ok \n");
    delete h264_encoder;
    h264_encoder = 0;
    fprintf(stderr,"delete encoder is ok \n");


}
