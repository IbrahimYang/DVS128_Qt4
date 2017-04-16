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
void Dialog::onDVSimagechanged(void)
{
    //QMutex mutex_events;
    QRgb value_red = qRgb(255, 0, 0),
         value_green = qRgb(0, 255, 0);
         //value_black = qRgb(0, 0, 0);
    QImage events_Grap=QImage(128,128,QImage::Format_RGB32);
           //scene_black=QImage(128,128,QImage::Format_RGB32);

//    for(int x_counter; x_counter<128; x_counter++)
//    {
//        for(int y_counter; y_counter<128; y_counter++)
//        {
//            events_Grap.setPixel(x_counter,y_counter, value_black);
//        }
//    }
//    scene_black = scene_black.scaled(640, 640,Qt::KeepAspectRatioByExpanding);
//    scene->addPixmap(QPixmap::fromImage(scene_black));

    scene->clear();
    //mutex_events.lock();
    for(int counter = 0 ; counter< (int)(myDVS128->events_show.size()) ; counter++)
    {
        if(myDVS128->events_show[counter].polarity == 1)
        {
           events_Grap.setPixel((myDVS128->events_show[counter].x),(myDVS128->events_show[counter].y), value_red);
        }
        else if(myDVS128->events_show[counter].polarity == 0)
        {
            events_Grap.setPixel((myDVS128->events_show[counter].x),(myDVS128->events_show[counter].y), value_green);
        }
    }
    myDVS128->events_show.clear();
    //mutex_events.unlock();
    events_Grap = events_Grap.scaled(640, 640,Qt::KeepAspectRatioByExpanding);
    scene->addPixmap(QPixmap::fromImage(events_Grap));
}

/*************************************************
Function:     Dialog::on_pushButton_clicked()
Description:  start function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::on_pushButton_clicked()
{
    myDVS128->start();
    myDVS128->dvs128_start();
}

/*************************************************
Function:     Dialog::on_pushButton_2_clicked()
Description:  stop function
Calls:        none
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void Dialog::on_pushButton_2_clicked()
{
    myDVS128->dvs128_stop();
    scene->clear();
}
