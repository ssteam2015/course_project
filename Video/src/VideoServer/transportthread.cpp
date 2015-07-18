#include "transportthread.h"
#define FRAME_SIZE 130000
#define YUV420_SIZE 320*240*3<<1

TransportThread::TransportThread()
{
    frametest = new unsigned char[FRAME_SIZE];
    h264frame = new unsigned char[YUV420_SIZE];

}

void TransportThread::run()
{
    rtp_send = new RTPsend();
    video_capture = new VideoCapture();
    h264_encoder = new MFCH264Encoder();
    rtp_send->rtp_params_init(dest_ip, portbase, destport);
    h264_encoder->init();
    video_capture->init_capture();
    stopped = false;
    while(!stopped)
    {
      video_capture->read_frame(frametest);
      h264_encoder->Encode_for_send(frametest);
     //debug fprintf(stderr,"the %d  frame's size  is %ld  \n",h264_encoder->getFrameNumber(), h264_encoder->encoded_size);
      memcpy(h264frame, h264_encoder->encoded_buf, h264_encoder->encoded_size);
     //debug  fprintf(stderr,"%d  frames is memcpy  \n",h264_encoder->getFrameNumber());
      rtp_send->nal_rtp_send(h264frame, h264_encoder->encoded_size);
     //debug fprintf(stderr,"%d  frames is sent  \n",h264_encoder->getFrameNumber());
     usleep(10*1000);

    }
}
void TransportThread::quit()
{
   stopped = true;
   rtp_send->close_rtp_session();
   h264_encoder->Encoder_stop();
   video_capture->stop_capture();
   delete video_capture;
   video_capture = 0;
   delete h264_encoder;
   h264_encoder = 0;
   delete rtp_send;
   rtp_send = 0;
}
