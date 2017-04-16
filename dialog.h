/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    dialog.h
Description:  the header file of dvs128_class.cpp
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#ifndef DIALOG_H
#define DIALOG_H

/*include local Qt library*/
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QImage>

/*include local My library*/
#include "mydvs.h"
//#include "data_handle.h"
#include "event.h"

namespace Ui {
class Dialog;
}

/*************************************************
Class:        Dialog
Description:  the class for Dialog
Calls:        none
Called By:    functions in dialog.cpp
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    MyDVS *myDVS128;
};

#endif // DIALOG_H