/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    mydvs.h
Description:  the header file of dvs128_class.cpp
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#ifndef MYDVS_H
#define MYDVS_H

/*include local origin library*/
#include <libcaer/libcaer.h>
#include <libcaer/devices/dvs128.h>

/*include c++11 library*/
#include <iostream>
#include <queue>

/*include local My library*/
#include "event.h"

/*include local Qt library*/
#include <QtCore>
#include <QtGui>
#include <QThread>

using namespace std;

/*************************************************
Class:        MyDVS
Description:  the class for dvs128
Calls:        none
Called By:    functions in dvs128_class.cpp
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
class MyDVS : public QThread
{
    Q_OBJECT
public:
    MyDVS();
    void run() Q_DECL_OVERRIDE;
    void dvs128_stop(void);
    void dvs128_start(void);
    std::vector<Event> events_show;

signals:
    void DVSimagechanged(void);
    void Packetnumberchanged(int);

protected:
    std::vector<Event> events_buffer;
    caerDeviceHandle dvs128_handle;
    bool running_ = false;

private:
    bool dvs128_init(void);
    void dvs128_deinit(void);
};

#endif // MYDVS_H
