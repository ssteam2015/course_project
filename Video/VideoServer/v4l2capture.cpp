#include "v4l2capture.h"

VideoCapture::~VideoCapture()
{

}

int VideoCapture::init_capture()
{
    struct v4l2_format fmt;
    read_frame_count = 0;
    fd  = open ("/dev/video0",  O_RDWR /* required */ );//blocked打开设备
    if (fd == -1)
    {
        perror ("Can't open device");
        return -1;
    }

        CLEAR (fmt);
        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width       = 320;
        fmt.fmt.pix.height      = 240;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
        fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
        fmt.fmt.pix.priv  = 0xfeedcafe;
        fmt.fmt.pix.priv  = 1;
        if (-1 == ioctl(fd,VIDIOC_S_FMT,&fmt))
        {
            perror ("While getting format");
            return -2;
        }

        struct v4l2_requestbuffers req;
        CLEAR (req);
        req.count               = 8;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;

        if(-1 == ioctl (fd, VIDIOC_REQBUFS, &req))//申请缓冲，count是申请的数量
        {
            perror ("While requesting buffers");
            return -3;
        }

        if (req.count < 2)
        {
            fprintf (stderr, "Can't get enough buffers!\n");
            return -4;
        }

        buffers = (struct buffer*)malloc(req.count *sizeof(*buffers));

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
        {
           struct v4l2_buffer buf;   //驱动中的一帧
           CLEAR (buf);
           buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
           buf.memory      = V4L2_MEMORY_MMAP;
           buf.index       = n_buffers;

           if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
           {
               perror ("While querying buffer");
               return -5;
           }

           buffers[n_buffers].length = buf.length;
           buffers[n_buffers].start =
           mmap (NULL /* start anywhere */,    //通过mmap建立映射关系
            buf.length,
            PROT_READ|PROT_WRITE /* required */,
            MAP_SHARED /* recommended */,
            fd, buf.m.offset);

           if (MAP_FAILED == buffers[n_buffers].start)
           {
               perror ("While mapping memory");
               return -6;
           }

        }
        //printf("n_buffers is %d ",n_buffers );
        /*这个循环完成后，所有缓存区都保存在
         *了buffers这个数组里了，完了就再将它们munmap即可。
         */
        /* 打开视频捕获 */
        enum v4l2_buf_type type;
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1==ioctl(fd,VIDIOC_STREAMON,&type))
        {
            perror ("While opening stream");
            return -7;
        }
   return 1;
}

int VideoCapture::read_frame (unsigned char *frame_addr)
{
struct v4l2_buffer buf;
CLEAR (buf);
buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory = V4L2_MEMORY_MMAP;
ioctl (fd, VIDIOC_DQBUF, &buf); //出列采集的帧缓冲
memcpy(frame_addr,buffers[buf.index].start,buffers[buf.index].length);
read_frame_count++;
ioctl (fd, VIDIOC_QBUF, &buf); //再将其入列

return 1;
}

int VideoCapture::stop_capture()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1==ioctl(fd,VIDIOC_STREAMOFF,&type))
    {
        perror ("While closing stream");
        return -7;
    }
    unsigned int  i;
    for (i = 0; i < n_buffers; ++i) {
       if (-1 == munmap (buffers[i].start, buffers[i].length))
        printf ("munmap error");
       buffers[i].start = 0;
   }
    free(buffers);
    buffers = 0;
    close (fd);
    return 1;
}
