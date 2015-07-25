#include "widget.h"
#include "ui_widget.h"
#include <iostream>



Widget::Widget(QWidget *parent) :QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    input = "";
    record_chosen = send_chosen = false;
    playback = new Play();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::setUIdisabled_record()
{
    ui->record->setEnabled(false);
    ui->playback->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stop->setEnabled(true);
}

void Widget::setUIenabled_record()
{
    ui->record->setEnabled(true);
    ui->playback->setEnabled(true);
    ui->start->setEnabled(true);
    ui->stop->setEnabled(false);
}

void Widget::setUIdisabled_send()
{
    ui->record->setEnabled(false);
    ui->playback->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stop->setEnabled(true);
}

void Widget::setUIenabled_send()
{
    ui->record->setEnabled(true);
    ui->playback->setEnabled(true);
    ui->start->setEnabled(true);
    ui->stop->setEnabled(false);
}
void Widget::stop_record_thread()
{
    if(record_chosen)
    {
       if(Trecord.isRunning())
       {
        Trecord.quit();
        record_chosen = false;
         ui->label->setText("Succeed!!!!");
         setUIenabled_record();
       }
    }
   disconnect(ui->stop, SIGNAL(clicked()), this, SLOT(stop_record_thread()));
}

void Widget::stop_send_thread()
 {
     if(send_chosen)
     {
        if(Tsend.isRunning())
        {
          Tsend.quit();
          send_chosen = false;
          ui->label->setText("sending is closed");
          setUIenabled_send();
        }
     }
    disconnect(ui->stop, SIGNAL(clicked()), this, SLOT(stop_send_thread()));
 }

void Widget::on_record_clicked()
{
    ui->label->setText("Recording... click 'stop' button to stop it");
    setUIdisabled_record();
    record_chosen = true;
    connect( ui->stop, SIGNAL(clicked()), this, SLOT(stop_record_thread()));
    Trecord.start();
}
// call palyback process
void Widget::on_playback_clicked()
{
    setUIdisabled_record();

    //QProcess for playback
    playback->FBOpen();
    playback->playback();

    setUIenabled_record();
}

// start send video
void Widget::on_start_clicked()
{
  ui->label->setText("Please enter the destination IP...");
  setUIdisabled_send();
  send_chosen = true;
  input_count = 0;
}


// simulated board
void Widget::on_Num1_clicked()
{
    ui->user_input->setText(ui->user_input->text()+"1");
    input += "1";
}

void Widget::on_Num2_clicked()
{
    ui->user_input->setText(ui->user_input->text()+"2");
    input += "2";
}

void Widget::on_Num3_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"3");
     input += "3";
}

void Widget::on_Num4_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"4");
     input += "4";
}

void Widget::on_Num5_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"5");
     input += "5";
}

void Widget::on_Num6_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"6");
     input += "6";
}

void Widget::on_Num7_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"7");
     input += "7";
}

void Widget::on_Num8_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"8");
     input += "8";
}

void Widget::on_Num9_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"9");
     input += "9";
}

void Widget::on_Num0_clicked()
{
     ui->user_input->setText(ui->user_input->text()+"0");
     input += "0";
}

void Widget::on_dot_clicked()
{
     ui->user_input->setText(ui->user_input->text()+".");
     input += ".";
}

void Widget::on_OK_clicked()
{
    int len = input.length();
    bool ok;
    input_count++;
    if (input_count == 1)
    {
        std::string strIP = ui->user_input->text().toStdString();
        if(is_destip_valid(strIP))
        {
          printf("IP valid\n");
          Tsend.dest_ip = strIP;
          ui->user_input->clear();
          ui->label->setText("Please enter the local port ..");
        }
        else
        {
            printf("IP invalid!!!!!!!!\n");
            ui->user_input->clear();
            input_count--;
            ui->label->setText("Please enter the destination IP...");
        }

    }
    else if (input_count == 2)
    {
        Tsend.portbase = ui->user_input->text().toUInt(&ok,10);
        printf("input_count is %d \n", input_count);
        if(ok)
        {
            ui->user_input->clear();
            ui->label->setText("Please enter the dest port ..");
            printf("input_count is %d \n", input_count);
        }
        else
        {
            ui->user_input->clear();
             input_count--;
            ui->label->setText("Please enter the local port ..");
        }

    }
    else if (input_count == 3)
    {
        Tsend.destport= ui->user_input->text().toUInt(&ok,10);
        printf("input dest port ok\n");
        if(ok)
        {
            ui->user_input->clear();
            ui->label->setText("Sending to client ...");
            input_count = 0;
            connect(ui->stop, SIGNAL(clicked()), this, SLOT(stop_send_thread()));
            Tsend.start();
             printf("begin send\n");
        }

        else
        {
            ui->user_input->clear();
            input_count--;
            ui->label->setText("Please enter the dest port ..");
        }

    }
    else
    {

    }
}

bool Widget::is_destip_valid(std::string & strIP)
{

    int nipseg = 0;
    int count = 0;
  for(int i = 0; strIP[i] != '\0'; i++)
  {
     if(strIP[i] >= '0' && strIP[i] <= '9') // number
      {
         nipseg = nipseg * 10 + strIP[i] - '0';
     }

    else if(strIP[i] == '.') // dot
     {
         if (nipseg < 0 || nipseg > 255)
             return false;
         count++;
         nipseg = 0;

    }

    else //other character
    {
        return false;
    }

    if(strIP[i+1] == '\0')
    {
        if (nipseg < 0 || nipseg > 255)
            return false;
        if (count != 3)
            return false;
        return true;

    }
    if(i > 15)
        break;
  }
return false;
}


