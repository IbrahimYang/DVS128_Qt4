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
}
