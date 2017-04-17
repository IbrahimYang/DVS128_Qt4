/*****************************************************************************
*******************Ibrahim,CBICR,Tsinghua University**************************
******************************************************************************
File name:    mydvs.cpp
Description:  define the functions in mydvs.h
Author:       Ibrahim Yang
Version:      V1.0
Date:         2017-4-12
History:      inilabs->libcaer VLOGroup->dvs-reconstruction libusb-1.0
*****************************************************************************/
#include "mydvs.h"

/*************************************************
Function:     MyDVS::MyDVS(Data_Handle *DVS_work)
Description:  the consturction function of MyDVS
Calls:        MyDVS Class's function
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
MyDVS::MyDVS()
{

}

/*************************************************
Function:     void MyDVS::run()
Description:  the is the run function to get dvs image
Calls:        MyDVS::init() MyDVS::deinit() MyDVS::stop(void)
Called By:    none
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void MyDVS::run()
{
    if(dvs128_init())
    {
        while(running_)
        {
            // get event and update timestamps
            caerEventPacketContainer packetContainer = caerDeviceDataGet(dvs128_handle);
            if (packetContainer == NULL) {
                continue; // Skip if nothing there.
            }
            events_buffer.clear();
            int32_t packetNum = caerEventPacketContainerGetEventPacketsNumber(packetContainer);
            for (int32_t i = 0; i < packetNum; i++) {
                caerEventPacketHeader packetHeader = caerEventPacketContainerGetEventPacket(packetContainer, i);
                if (packetHeader == NULL) {
                    continue; // Skip if nothing there.
                }
                // Packet 0 is always the special events packet for DVS128, while packet is the polarity events packet.
                if (i == POLARITY_EVENT) {
                    caerPolarityEventPacket polarity = (caerPolarityEventPacket) packetHeader;
                    for (int32_t caerPolarityIteratorCounter = 0; caerPolarityIteratorCounter < caerEventPacketHeaderGetEventNumber(&(polarity)->packetHeader);caerPolarityIteratorCounter++) {
                        caerPolarityEvent caerPolarityIteratorElement = caerPolarityEventPacketGetEvent(polarity, caerPolarityIteratorCounter);
                        if (!caerPolarityEventIsValid(caerPolarityIteratorElement)) { continue; }
                        Event event;
                        event.t = caerPolarityEventGetTimestamp(caerPolarityIteratorElement)*1e-6;
                        event.x = caerPolarityEventGetX(caerPolarityIteratorElement); // don't know why it is other way round?
                        event.y = caerPolarityEventGetY(caerPolarityIteratorElement);
                        event.polarity = caerPolarityEventGetPolarity(caerPolarityIteratorElement)?1:0;
                        //if(undistortPoint(event,params.K_cam,params.radial))
                        //cout<<"event - t: "<<event.t<<"x: "<<event.x<<"y: "<<event.y<<"polarity: "<<event.polarity<<endl;
                        events_buffer.push_back(event);
                    }
                }
            }//for
            caerEventPacketContainerFree(packetContainer);
            for(int ii=0; ii < (int)(events_buffer.size()); ii++)
            {
                events_show.push_back(events_buffer[ii]);
            }
            emit DVSimagechanged();
            emit Packetnumberchanged((int)events_buffer.size());
        }
        dvs128_deinit();
    }
}

/*************************************************
Function:     void MyDVS::dvs128_stop(void)
Description:  stop fuction
Calls:        none
Called By:    void MyDVS::run()
Input:        none
Output:       running_
Return:       none
Others:       none
*************************************************/
void MyDVS::dvs128_stop(void)
{
    running_=false;
    cout << "Stop DVS128 image acquire"<<endl;
}

/*************************************************
Function:     void MyDVS::dvs128_start(void)
Description:  start fuction
Calls:        none
Called By:    void MyDVS::run()
Input:        none
Output:       running_
Return:       none
Others:       none
*************************************************/
void MyDVS::dvs128_start(void)
{
    running_=true;
    cout << "Start DVS128 image acquire"<<endl;
}

/*************************************************
Function:     bool MyDVS::init()
Description:  dvs128 init fuction
Calls:        none
Called By:    void MyDVS::run()
Input:        none
Output:       dvs128_handle
Return:       bool does dvs init status
Others:       none
*************************************************/
bool MyDVS::dvs128_init()
{
    // init camera
    // Open a DVS128, give it a device ID of 1, and don't care about USB bus or SN restrictions.
    dvs128_handle = caerDeviceOpen(1, CAER_DEVICE_DVS128, 0, 0, NULL);
    if (dvs128_handle == NULL) {
        cout << "Failed to open DVS128 device"<<endl;
        return false;
    }
    // Let's take a look at the information we have on the device.
    struct caer_dvs128_info dvs128_info = caerDVS128InfoGet(dvs128_handle);

    printf("%s --- ID: %d, Master: %d, DVS X: %d, DVS Y: %d, Logic: %d.\n", dvs128_info.deviceString,
        dvs128_info.deviceID, dvs128_info.deviceIsMaster, dvs128_info.dvsSizeX, dvs128_info.dvsSizeY,
        dvs128_info.logicVersion);
    caerDeviceSendDefaultConfig(dvs128_handle);

    // Values taken from DVS_FAST
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_CAS, 1992);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFF, 13125);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFON, 209996);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFOFF, 132);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_FOLL, 271);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_INJGND, 1108364);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PR, 217);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUX, 8159221);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUY, 16777215);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REFR, 969);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQ, 309590);
     caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQPD, 16777215);
    // Values taken from DVS_SLOW
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_CAS, 54);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFF, 30153);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFON, 482443);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_DIFFOFF, 132);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_FOLL, 51);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_INJGND, 1108364);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PR, 3);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUX, 8159221);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_PUY, 16777215);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REFR, 6);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQ, 159147);
//    caerDeviceConfigSet(dvs128_handle, DVS128_CONFIG_BIAS, DVS128_CONFIG_BIAS_REQPD, 16777215);

    caerDeviceDataStart(dvs128_handle, NULL, NULL, NULL, NULL, NULL);
    caerDeviceConfigSet(dvs128_handle, CAER_HOST_CONFIG_DATAEXCHANGE, CAER_HOST_CONFIG_DATAEXCHANGE_BLOCKING, true);
    return true;
}

/*************************************************
Function:     void MyDVS::deinit()
Description:  dvs128 deinit fuction
Calls:        none
Called By:    void MyDVS::run()
Input:        none
Output:       none
Return:       none
Others:       none
*************************************************/
void MyDVS::dvs128_deinit()
{
    caerDeviceDataStop(dvs128_handle);

    caerDeviceClose(&dvs128_handle);
}
