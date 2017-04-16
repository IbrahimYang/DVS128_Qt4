/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    event.h
Description:  define the event class
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#ifndef EVENT_H
#define EVENT_H

/*************************************************
Struct:       Event
Description:  the struct save the event data for dvs128
Calls:        none
Called By:    MyDVS128::run()
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
typedef struct
{
   int x;
   int y;
   float t;
   bool polarity;
}Event;

#endif // EVENT_H
