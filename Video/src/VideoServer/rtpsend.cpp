#include "rtpsend.h"
#define H264                    96
#define MAX_RTP_PKT_LENGTH     1350

RTPsend::RTPsend()
{
       timeStampInc = 90000 / 30;
       sendFrameCount = 0;
}
bool RTPsend::close_rtp_session()
{
    free(sendBuffer);
    sendBuffer = 0;
    free(nalBuffer);
    nalBuffer = 0;
    sess.BYEDestroy(RTPTime(0,100), 0, 0);
    return true;

}

//set params: IP addr ,port number
// 需要输入IP地址参数
int RTPsend::rtp_params_init(std::string ipstr, unsigned int portbase, unsigned int destport)
{
 /*
    std::cout << "please enter IP address : " << std::endl;
    std::cout << "please enter desitination port number:" << std::endl;
    std::cout << "please enter  port base:" << std::endl;
*/
    num = 0;
    destip = inet_addr(ipstr.c_str());
    if (destip == INADDR_NONE)
    {
        std::cerr << "Bad IP address specified" << std::endl;
        return -1;
    }

       destip = ntohl(destip);

       sessparams.SetOwnTimestampUnit((double)(1.0f/90000.0f));
       transparams.SetPortbase(portbase);
       fprintf(stderr,"SetPortbass\n");
       status = sess.Create(sessparams,&transparams);
       fprintf(stderr, "Creat session\n");
       checkerror(status);

       RTPIPv4Address addr(destip,destport);


      status = sess.AddDestination(addr);

       fprintf(stderr,"Add to Destination\n");
       checkerror(status);
       currentTime = clock();

       sendBuffer = (unsigned char*)malloc(1500);
       nalBuffer = (unsigned char*)malloc(130000);
}
 //NAL单元可能大于MTU（分片发送FU-A），可能小于MTU（单片发送）
void RTPsend::rtp_send(unsigned char *buf, int len)
{
    int n    = len / MAX_RTP_PKT_LENGTH;
    int last = len % MAX_RTP_PKT_LENGTH;
    if(last > 0) n++;

    int timeInc ;

    //currentTime += timeStampInc;
    //fprintf(stderr,"buf len = \n");
    if(len>0)
          fprintf(stderr,".");

    char nalHead = buf[0];  //获取编码后原始头部的1个byte

        //单片NAL
    if(len < MAX_RTP_PKT_LENGTH)
    {
        status = sess.SendPacket((void*)buf,len,H264,true,timeStampInc);
        checkerror(status);

    }
        //FU-A分片
    else
    {
       for(int i = 0; i < n;i++)
       {
            //原始的NAL header的前三位为FU indicator的前三位
            sendBuffer[0] = (nalHead & 0x60)|28;
                        //原始的NAL头的后五位为FU header的后五位
            sendBuffer[1] = (nalHead & 0x1f);

                if(0 == i) //send the first subpacket
                {
                       timeInc = timeStampInc;
                       sendBuffer[1] |= 0x80;
                       memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH],MAX_RTP_PKT_LENGTH);
                       status = sess.SendPacket((void*)sendBuffer,MAX_RTP_PKT_LENGTH+2,H264,false,timeInc);
                }

            else if(i == n - 1) //send the last subpacket
            {
               timeInc = 0;
               sendBuffer[1] |= 0x40;
               memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH],last);
               status = sess.SendPacket((void*)sendBuffer,last +2 ,H264,true,timeInc);
            }
            else
            {
               timeInc = 0;
               memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH],MAX_RTP_PKT_LENGTH);
               status = sess.SendPacket((void*)sendBuffer,MAX_RTP_PKT_LENGTH+2 ,H264,false,timeInc);
            }

            checkerror(status);

       }
    }

}

void RTPsend::checkerror(int err)
{
  if (err < 0)
  {
    const  char* errstr = RTPGetErrorString(err).c_str();
    printf("Error:%s//n", errstr);
    exit(-1);
  }
}
//单帧H264图片,通过MFC硬编码后的一帧图像包含若干个NAL 单元
void RTPsend::nal_rtp_send(unsigned char* buf, int len)
{
    nalcount = 0;
    int pos = 0;

    while(pos < len)
    {
        int nalLen = get_next_nalu(&buf[pos],len - pos,nalBuffer);

        if((nalBuffer[4] & 0x1f) == 5)
        {
            fprintf(stderr,"Key(IDR) frame is found \n"); //debug
            fprintf(stderr,"Key(IDR) frame's length is %d \n",nalLen); //debug
        }

        rtp_send(nalBuffer + 4,nalLen - 4);          // drop the start code 00 00 00 01
        pos += nalLen;
        nalcount++;
    }

    if(nalcount > 1)
       fprintf(stderr,"this frame has %d nal units \n", nalcount); //debug

}
//每次根据 start code 判断一个NAL单元
int RTPsend::get_next_nalu(unsigned char *buf,int buf_len,unsigned char* outbuf)
{
    int pos = 0;
        bool findStartcode;
        if(buf_len > 4)
        {
            findStartcode = find_start_code(&buf[pos]);
        }
        else return 0 ;

        if(findStartcode)
        {
            do
            {
                outbuf[pos] = buf[pos];
                        pos++;
                findStartcode = find_start_code(&buf[pos]);
            } while(!findStartcode && (pos + 4 < buf_len));
        }

        if(findStartcode)
        {
          //debug fprintf(stderr,"find start code \n");
            return pos ;
        }

        else if(pos + 4 >= buf_len)
        {
            do
                    {
                     outbuf[pos] = buf[pos];
                     pos++;
                    } while(pos < buf_len);
            return buf_len;
        }
        else return 0;
}

bool RTPsend::find_start_code(unsigned char*buf)
{
  if( (buf[0] == 0)&&
      (buf[1] == 0)&&
      (buf[2] == 0)&&
      (buf[3] == 1)
          )
  {
    return true;
  }
  else
      return false;
}
