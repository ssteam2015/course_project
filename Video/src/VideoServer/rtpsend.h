#ifndef RTPSEND_H
#define RTPSEND_H
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"

#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>


void nal_rtp_send(unsigned char* buf, int len);
class RTPsend
{
public:
    RTPsend();
    int rtp_params_init(std::string ipstr, unsigned int portbase, unsigned int destport);
    void nal_rtp_send(unsigned char* buf, int len);
    bool close_rtp_session();

private:
     RTPSession                   sess;
     RTPUDPv4TransmissionParams   transparams;
     RTPSessionParams             sessparams;
     uint                         portbase,destport;
     uint                         destip;
    std::string                  ipstr;
    int                          status,i,num;
    int                          timeStampInc;
    long long                    currentTime;
    int                          sendFrameCount ;
    int                          nalcount; //debug
    unsigned char*                      nalBuffer;
    unsigned char*                      sendBuffer;

    void checkerror(int err);
    void rtp_send(unsigned char *buf, int len);
    int  get_next_nalu(unsigned char* buf,int buf_len,unsigned char* outbuf);
    bool find_start_code(unsigned char*buf);


};

#endif // RTPSEND_H
