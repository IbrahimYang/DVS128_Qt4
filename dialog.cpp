/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    dialog.cpp
Description:  the dialog.cpp
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#include "dialog.h"
#include "ui_dialog.h"

#define events_length 2000
#define avarge_length 10

/*************************************************
Function:     Dialog()
Description:  Dialog Class's construction function
Calls:        none
Called By:    all
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    myDVS128 = new MyDVS();

    connect(myDVS128, SIGNAL(DVSimagechanged()),this,SLOT(onDVSimagechanged()));
    connect(myDVS128, SIGNAL(Packetnumberchanged(int)),this,SLOT(onPacketnumberchanged(int)));
}

/*************************************************
Function:     Dialog()
Description:  Dialog Class's deconstruction function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
Dialog::~Dialog()
{
    delete ui;
}

/*************************************************
Function:     Dialog::onDVSimagechanged()
Description:  slot get the DVS image changed
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::onDVSimagechanged()
{
    int show_size = (int)(myDVS128->events_show.size()),
        show_part = show_size / events_length,
        show_phase = show_size % events_length;
    QRgb value_red = qRgb(255, 0, 0),
         value_green = qRgb(0, 255, 0);
    QImage events_Grap=QImage(128,128,QImage::Format_RGB32);
    events_Grap.fill(Qt::black);

    if(show_part == 0)
    {
        for(int counter_phase = 0; counter_phase < show_phase; counter_phase++)
        {
            if(myDVS128->events_show[counter_phase].polarity == 1)
            {
               events_Grap.setPixel((myDVS128->events_show[counter_phase].x),(myDVS128->events_show[counter_phase].y), value_red);
            }
            else if(myDVS128->events_show[counter_phase].polarity == 0)
            {
                events_Grap.setPixel((myDVS128->events_show[counter_phase].x),(myDVS128->events_show[counter_phase].y), value_green);
            }
        }
        scene->clear();
        myDVS128->events_show.clear();
        events_Grap = events_Grap.scaled(640, 640, Qt::KeepAspectRatioByExpanding);
        scene->addPixmap(QPixmap::fromImage(events_Grap));
    }
    else
    {
        for(int counter_part = 0; counter_part < show_part; counter_part++)
        {
            for(int counter_length = 0; counter_length < events_length; counter_length++)
            {
                if(myDVS128->events_show[counter_part*events_length + counter_length].polarity == 1)
                {
                   events_Grap.setPixel((myDVS128->events_show[counter_part*events_length + counter_length].x),(myDVS128->events_show[counter_part*events_length + counter_length].y), value_red);
                }
                else if(myDVS128->events_show[counter_part*events_length + counter_length].polarity == 0)
                {
                    events_Grap.setPixel((myDVS128->events_show[counter_part*events_length + counter_length].x),(myDVS128->events_show[counter_part*events_length + counter_length].y), value_green);
                }
            }
            scene->clear();
            events_Grap = events_Grap.scaled(640, 640, Qt::KeepAspectRatioByExpanding);
            scene->addPixmap(QPixmap::fromImage(events_Grap));
            events_Grap = events_Grap.scaled(128, 128, Qt::KeepAspectRatioByExpanding);
            events_Grap.fill(Qt::black);
        }
        for(int counter_phase = 0; counter_phase < show_phase; counter_phase++)
        {
            if(myDVS128->events_show[show_part*events_length + counter_phase].polarity == 1)
            {
               events_Grap.setPixel((myDVS128->events_show[show_part*events_length + counter_phase].x),(myDVS128->events_show[show_part*events_length + counter_phase].y), value_red);
            }
            else if(myDVS128->events_show[show_part*events_length + counter_phase].polarity == 0)
            {
                events_Grap.setPixel((myDVS128->events_show[show_part*events_length + counter_phase].x),(myDVS128->events_show[show_part*events_length + counter_phase].y), value_green);
            }
        }
        scene->clear();
        myDVS128->events_show.clear();
        events_Grap = events_Grap.scaled(640, 640, Qt::KeepAspectRatioByExpanding);
        scene->addPixmap(QPixmap::fromImage(events_Grap));
    }
}

/*************************************************
Function:     Dialog::onPacketnumberchanged(int packet_size)
Description:  start function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::onPacketnumberchanged(int packet_size)
{
    int avarge = 0;

    if(packet_size_buffer.size() > avarge_length)
    {
        for(int ii=0; ii<(int)packet_size_buffer.size(); ii++)
        {
            avarge += packet_size_buffer[ii];
        }
        avarge /= avarge_length;
        ui->label->setText(QString::number(avarge));
        packet_size_buffer.clear();
    }
    else
    {
        packet_size_buffer.push_back(packet_size);
    }
}

/*************************************************
Function:     Dialog::on_start_Button_clicked()
Description:  start function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::on_start_Button_clicked()
{
    myDVS128->start();
    myDVS128->dvs128_start();
}

/*************************************************
Function:     Dialog::on_stop_Button_clicked()
Description:  stop function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::on_stop_Button_clicked()
{
    myDVS128->dvs128_stop();
    scene->clear();
}
