#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
}

class mythread : public QThread
{
     Q_OBJECT
public:
    explicit mythread(QObject *parent = 0);



    AVFormatContext *pFormatCtx;
    int             i, videoindex;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;


    void show_dshow_device();
    void show_dshow_device_option();
    void show_vfw_device();
    void show_avfoundation_device();
    void th_quit(bool quit);
    void ff_set(int w, int h, int fps);

signals:

    void getImg(QImage img);
    void sendquit();

public slots:

protected:
    void run();

private:
    int num;
    int num2;
    bool Quit;
    QString ffps;
    QString ffwh;

    int ww;
    int hh;
};

#endif // MYTHREAD_H
