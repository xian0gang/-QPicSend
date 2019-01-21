#include "mythread.h"
#include <QDebug>
#include <QDateTime>

extern int www;
extern int hhh;
extern int mode;

mythread::mythread(QObject *parent) : QThread(parent)
{
    num = 0;
    Quit = false;
   // ww = 480; //显示label宽
   // hh = 270; //显示label高
    ww = www;
    hh = hhh;
}

//退出线程
void mythread::th_quit(bool quit)
{
    Quit = quit;
    qDebug()<<"quit:"<<quit;
}

//分辨率设置
void mythread::ff_set(int w, int h, int fps)
{
    ffps = QString::number(fps, 10);
    ffwh = QString::number(w, 10) + "x" + QString::number(h, 10);
    qDebug()<<ffps;
    qDebug()<<ffwh;
   /* if (mode == 1)
    {
        switch (h) {
        case 1080:
            ww = 480; //显示label宽
            hh = 270; //显示label高
            break;
        case 1024:
            ww = 320; //显示label宽
            hh = 256; //显示label高
            break;
        case 720:
            ww = 320; //显示label宽
            hh = 180; //显示label高
            break;
        case 768:
            ww = 256; //显示label宽
            hh = 192; //显示label高
            break;
        case 600:
            ww = 320; //显示label宽
            hh = 240; //显示label高
            break;
        case 480:
            ww = 320; //显示label宽
            hh = 240; //显示label高
            break;
        case 240:
            ww = 320; //显示label宽
            hh = 240; //显示label高
            break;
        default:
            break;
        }
    }
    else
    {*/
        switch (h) {
        case 1080:
            ww = 960; //显示label宽
            hh = 540; //显示label高
            break;
        case 1024:
            ww = 640; //显示label宽
            hh = 512; //显示label高
            break;
        case 720:
            ww = 640; //显示label宽
            hh = 360; //显示label高
            break;
        case 768:
            ww = 512; //显示label宽
            hh = 384; //显示label高
            break;
        case 600:
            ww = 400; //显示label宽
            hh = 300; //显示label高
            break;
        case 480:
            ww = 640; //显示label宽
            hh = 480; //显示label高
            break;
        case 240:
            ww = 320; //显示label宽
            hh = 240; //显示label高
            break;
        default:
            break;
        }
    //}
    //qDebug()<<ww;
   // qDebug()<<hh;

}

//主线程
void mythread::run()
{

//变量
    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
//ffmpeg初始化和注册
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    avdevice_register_all();
    show_dshow_device();
    show_dshow_device_option();
//  show_vfw_device();

//参数设置
    AVDictionary* options = NULL;
    av_dict_set(&options, "fflags", "nobuffer", 0);
    av_dict_set(&options, "max_delay", "100000", 0);

    QByteArray aa;
    aa = ffps.toLatin1();
    av_dict_set(&options, "framerate", /*"30"*/aa.data(), 0);
    av_dict_set(&options, "input_format", "mjpeg", 0);

    QByteArray bb;
    bb = ffwh.toLatin1();
    av_dict_set(&options, "video_size", /*"1920x1080"*/bb.data(), 0);

    AVInputFormat *ifmt=av_find_input_format("dshow");
    //Set own video device's name
    if(avformat_open_input(&pFormatCtx,"video=HD USB Camera",ifmt,&options)!=0){
        printf("Couldn't open input stream.\n");
        return;
    }

    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {
        printf("Couldn't find stream information.\n");
        return;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoindex=i;
            break;
        }
    }
    if(videoindex==-1)
    {
        printf("Couldn't find a video stream.\n");
        return;
    }
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;

    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL)
    {
        printf("Codec not found.\n");
        return;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
    {
        printf("Could not open codec.\n");
        return;
    }
//    视频格式转换配置
    AVFrame	*pFrame,*pFrameYUV;
    AVFrame *pFrameRGB;
    pFrameRGB = av_frame_alloc();
    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();

    unsigned char *out_buffer = (uint8_t *) av_malloc(avpicture_get_size(AV_PIX_FMT_RGB32, /*pCodecCtx->width*/ww, /*pCodecCtx->height*/hh));
    avpicture_fill( (AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB32, /*pCodecCtx->width*/ww, /*pCodecCtx->height*/hh);


    int ret, got_picture;

    AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width,
                                     pCodecCtx->height,
                                     pCodecCtx->pix_fmt,
                                     /*pCodecCtx->width*/ww,
                                     /*pCodecCtx->height*/hh,
                                     AV_PIX_FMT_RGB32,
                                     SWS_BICUBIC,
                                     NULL, NULL, NULL);
/****************录像 编码 设置***********************************************/
//    变量
    AVOutputFormat *ofmt = NULL;
    AVFormatContext *ofmt_ctx = NULL;
    AVStream *out_stream;
    AVCodecContext *ofcodec_ctx;
    AVCodec *ocodec;
    AVPacket outpkg;

//文件名 时间+分辨率.mp4
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMddhhmmss");
    qDebug()<<current_date;
    QString mp4_name = "./video/" + current_date + "_" + ffwh + ".mp4";
    QByteArray cc;
    cc = mp4_name.toLatin1();
    char* out_filename = cc.data();

    qDebug()<<out_filename;

    ofmt_ctx = avformat_alloc_context();
    if (!ofmt_ctx)
    {
        qDebug("could not create output context\n");
    }

    ofmt = av_guess_format("mjpeg", NULL, NULL);
    ofmt_ctx->oformat = ofmt;
    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream)
    {
        qDebug("failed allocating output stream\n");
    }
    ofcodec_ctx = out_stream->codec;
//    ofcodec_ctx->codec_id = ofmt->video_codec;
    ofcodec_ctx->codec_id = AV_CODEC_ID_MPEG4;
    qDebug()<<"id:"<<ofcodec_ctx->codec_id;
    ofcodec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ofcodec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    ofcodec_ctx->width = pCodecCtx->width;
    ofcodec_ctx->height = pCodecCtx->height;
//    帧率
    ofcodec_ctx->time_base.den = pCodecCtx->framerate.num/pCodecCtx->framerate.den;
    ofcodec_ctx->time_base.num = 1;
    //压缩比特率
    ofcodec_ctx->bit_rate = 4000000;
    qDebug("timebase %d %d\n", ofcodec_ctx->time_base.den,
            ofcodec_ctx->time_base.num);
    out_stream->codec->codec_tag = 0;
    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    ocodec = avcodec_find_encoder(ofcodec_ctx->codec_id);
    if (!ocodec)
    {
        qDebug("find encoder err\n");
        return;
    }
    if (avcodec_open2(ofcodec_ctx, ocodec, NULL) < 0)
    {
        qDebug("open encoder err\n");
        return;
    }
//编码视频格式转换 设置
    AVFrame *pFrameYUV420;
    unsigned char *out_buffer2;
    struct SwsContext *out_conCtx;

    pFrameYUV420 = av_frame_alloc();
    av_new_packet(&outpkg, ofcodec_ctx->width * ofcodec_ctx->height * 3);
    out_buffer2 = (unsigned char *) av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, ofcodec_ctx->width,ofcodec_ctx->height));
    avpicture_fill((AVPicture*) pFrameYUV420, out_buffer2, AV_PIX_FMT_YUV420P,ofcodec_ctx->width, ofcodec_ctx->height);

    out_conCtx = sws_getContext(pCodecCtx->width,
                                pCodecCtx->height,
                                pCodecCtx->pix_fmt,
                                ofcodec_ctx->width,
                                ofcodec_ctx->height,
                                ofcodec_ctx->pix_fmt,
                                SWS_BICUBIC, NULL, NULL, NULL);

    int sss = avio_open(&ofmt_ctx->pb,out_filename,AVIO_FLAG_WRITE);
    if(sss < 0)
    {
       qDebug()<<sss;
       return;
    }

    av_dump_format(ofmt_ctx, 0, out_filename, 1);
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0)
    {
        qDebug("error occurred when opening output URL\n");
        return;
    }

    pFrameYUV420->format = AV_PIX_FMT_YUV420P;
    pFrameYUV420->width = ofcodec_ctx->width;
    pFrameYUV420->height = ofcodec_ctx->height;

/******************************主循环 采集 编解码 保存****************************************/
    for (;;)
    {
        if(!Quit)
        {
            if(av_read_frame(pFormatCtx, packet)>=0)
            {
                if(packet->stream_index==videoindex)
                {
//                    解码
                    ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
                    if(ret < 0)
                    {
                        printf("Decode Error.\n");
                        return;
                    }
                    if(got_picture)
                    {
                        num++;
//                        qDebug()<<"第"<<num<<"帧";
                        sws_scale(img_convert_ctx,
                                  (const unsigned char* const*)pFrame->data,
                                  pFrame->linesize,
                                  0,
                                  pFrame->height,
                                  pFrameRGB->data,
                                  pFrameRGB->linesize);
                        //rgb数据放入qimage
                        QImage image((uchar *)pFrameRGB->data[0],/*pCodecCtx->width*/ww,/*pCodecCtx->height*/hh,QImage::Format_RGB32);
                        //发送给主进程 显示
                        getImg(image);


/********************************************录像保存***********************************/
                        sws_scale(out_conCtx,
                                (const unsigned char * const *) pFrame->data,
                                pFrame->linesize,
                                0,
                                ofcodec_ctx->height,
                                pFrameYUV420->data,
                                pFrameYUV420->linesize);
//                        编码
                        int rett = avcodec_encode_video2(ofcodec_ctx, &outpkg, pFrameYUV420, &got_picture);
                        if (rett < 0)
                        {
                            qDebug("encode err\n");
                        }
                        av_write_frame(ofmt_ctx, &outpkg);

/**********************************************************************************/
                    }
                }
                av_free_packet(&outpkg);
                av_free_packet(packet);
            }
        }
        else
        {
            break;
        }
    }
//    释放
    //写文件尾
    av_write_trailer(ofmt_ctx);
    sws_freeContext(img_convert_ctx);
    sws_freeContext(out_conCtx);
    av_free(pFrameYUV420);
    avcodec_close(ofcodec_ctx);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);
    qDebug()<<"6";
//    av_free(out_buffer);
    av_free(pFrameYUV);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    Quit = false;
    sendquit();
    return;
}


//Show Dshow Device
void mythread::show_dshow_device()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("dshow");

    qDebug("========Device Info=============\n");
    avformat_open_input(&pFormatCtx,"video=HD USB Camera",iformat,&options);
    qDebug("================================\n");
}


//Show Dshow Device Option
void mythread::show_dshow_device_option()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_options","true",0);
    AVInputFormat *iformat = av_find_input_format("dshow");
    qDebug("========Device Option Info======\n");
    avformat_open_input(&pFormatCtx,"video=HD USB Camera",iformat,&options);
    qDebug()<<iformat->long_name;
    qDebug()<<iformat->next->long_name;
    qDebug("================================\n");
}

//Show VFW Device
void mythread::show_vfw_device()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVInputFormat *iformat = av_find_input_format("vfwcap");
    qDebug("========VFW Device Info======\n");
    avformat_open_input(&pFormatCtx,"list",iformat,NULL);
    qDebug("=============================\n");
}

//Show AVFoundation Device
void mythread::show_avfoundation_device()
{
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);
    AVInputFormat *iformat = av_find_input_format("avfoundation");
    qDebug("==AVFoundation Device Info===\n");
    avformat_open_input(&pFormatCtx,"",iformat,&options);
    qDebug("=============================\n");
}

