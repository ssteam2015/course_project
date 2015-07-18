#ifndef WIDGET_H
#define WIDGET_H

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

}
#include <iostream>
#include <string>
#include <QtGui>
#include<QThread>
#include "recordthread.h"
#include "transportthread.h"


#include "Play.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))


namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    RecordThread Trecord;
    TransportThread Tsend;

    Play* playback;

    QString input;
    void setUIdisabled_record();
    void setUIenabled_record();
    void setUIdisabled_send();
    void setUIenabled_send();

    std::string dest_ip;
    unsigned int local_port;
    unsigned int dest_port;
    bool record_chosen;
    bool send_chosen;

    unsigned int input_count;


private slots:
    void on_OK_clicked();
    void on_dot_clicked();
    void on_Num0_clicked();
    void on_Num9_clicked();
    void on_Num8_clicked();
    void on_Num7_clicked();
    void on_Num6_clicked();
    void on_Num5_clicked();
    void on_Num4_clicked();
    void on_Num3_clicked();
    void on_Num2_clicked();
    void on_Num1_clicked();

    void on_playback_clicked();
    void on_start_clicked();
    void on_record_clicked();

    bool is_destip_valid(std::string & strIP);
    void stop_record_thread();
    void stop_send_thread();

};

#endif // WIDGET_H
