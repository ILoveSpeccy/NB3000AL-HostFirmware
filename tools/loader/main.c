#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "libusb.h"

#define MY_VID 0x0547
#define MY_PID 0x1002

#define EP_IN  0x84
#define EP_OUT 0x02

libusb_device_handle* handle;

unsigned char InputPacket[64];
unsigned char OutputPacket[64];

FILE *fin;

unsigned long flen;
unsigned long fcnt;

int main(int argc, char *argv[])
{
    printf("Simple Altium NanoBoard 3000 FPGA Loader\n");

    if (argc < 2)
    {
        printf("Usage: loader <Altera bitstream file (rbf)>\n");
        return -1;
    }

    printf("Opening bitstream file...\n");

    fin = fopen(argv[1], "rb");

    if (fin == NULL)
    {
        printf("Can't open file %s\n", argv[1]);
        return -1;
    }

	if (libusb_init(NULL) < 0)
        printf("LibUSB init error\n");
    else
    {
        handle = libusb_open_device_with_vid_pid(NULL, MY_VID, MY_PID);
        if (handle == NULL)
            printf("USB Device not connected\n");
        else
        {
            if (libusb_claim_interface(handle, 0) < 0)
                printf("Error claim interface\n");
            else
            {
                printf("Init FPGA...\n");
                OutputPacket[0] = 0xF0;
                libusb_bulk_transfer(handle, EP_OUT, OutputPacket, 64, NULL, 5000U);

                printf("Configure FPGA...\n");
                OutputPacket[0] = 0xF1;

                unsigned char tt = 0;

                while(fread(&OutputPacket[1],1,63,fin))
                {
                    libusb_bulk_transfer(handle,  EP_OUT, OutputPacket, 64, NULL, 5000);
                    tt++;
                    if (tt==163)
                    {
                        tt=0;
                        printf("*");
                    }
                }
                printf("\nDone!\n");
                fclose(fin);
                libusb_release_interface(handle, 0);
            }
        }
    }

    if (handle)
		libusb_close(handle);

	libusb_exit(NULL);
	return 0;
}

//usb_dev_handle *MyLibusbDeviceHandle = NULL;
//
//// Open USB Device
///////////////////////////////////////////////////////////////
//
//usb_dev_handle *open_dev(void)
//{
//   struct usb_bus *bus;
//   struct usb_device *dev;
//
//   for(bus = usb_get_busses(); bus; bus = bus->next)
//   {
//      for(dev = bus->devices; dev; dev = dev->next)
//	   {
//	      if(dev->descriptor.idVendor == MY_VID && dev->descriptor.idProduct == MY_PID)
//	      {
//		      return usb_open(dev);
//	      }
//	   }
//   }
//   return NULL;
//}
//
//// Connect USB Device
///////////////////////////////////////////////////////////////
//
//unsigned char Connect_Device(void)
//{
//   usb_init(); /* initialize the library */
//   usb_find_busses(); /* find all busses */
//   usb_find_devices(); /* find all connected devices */
//   if(!(MyLibusbDeviceHandle = open_dev()))
//      return 1;
//   if(usb_set_configuration(MyLibusbDeviceHandle, 1) < 0)
//   {
//      usb_close(MyLibusbDeviceHandle);
//      return 2;
//   }
//	if(usb_claim_interface(MyLibusbDeviceHandle, 0) < 0)
//   {
//      usb_close(MyLibusbDeviceHandle);
//	   return 3;
//   }
//   return 0;
//}
//
//int main(int argc, char **argv)
//{
//   unsigned char t;
//
//   printf("===========================================\n");
//   printf(" Altium NanoBoard NB3000AL Custom Firmware \n");
//   printf("-------------------------------------------\n");
//   printf("   ILoveSpeccy * www.speccyland.net '2012  \n");
//   printf("===========================================\n\n");
//
//   printf("Open USB Device.. ");
//   switch (Connect_Device())
//   {
//      case 1:  {printf("Open USB Device failed\n");   return 1;}
//      case 2:  {printf("Config USB Device failed\n"); return 2;}
//      case 3:  {printf("Claim USB Device failed\n");  return 3;}
//   }
//   printf("ok\n");
//
//   FILE *MFILE;
//   if ((MFILE=fopen("test.rbf","rb")) == NULL)
//   {
//      printf("Input file not found");
//      return 4;
//   }
//
//   printf("Init FPGA...\n");
//   OutputPacket[0] = 0xF0;
//   usb_bulk_write(MyLibusbDeviceHandle, EP_OUT, OutputPacket, 64, 5000);
//
//   printf("Configure FPGA...\n");
//   OutputPacket[0] = 0xF1;
//
//   unsigned char tt = 0;
//
//   printf("\n================================\n");
//
//   while(fread(&OutputPacket[1],1,63,MFILE))
//   {
//      if (usb_bulk_write(MyLibusbDeviceHandle,  EP_OUT, &OutputPacket[0], 64, 5000) != 64)
//      {
//         printf("\n\nError USB Transmit\n");
//         exit(4);
//      }
//      tt++;
//      if (tt==163)
//      {
//         tt=0;
//         printf("*");
//      }
//   }
//   fclose(MFILE);
//
//   printf("\n\nClose USB Device\nGood by...\n");
//   usb_release_interface(MyLibusbDeviceHandle, 1);
//   usb_close(MyLibusbDeviceHandle);
//   return 0;
//}
//
