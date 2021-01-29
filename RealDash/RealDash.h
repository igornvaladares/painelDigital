#include "Arduino.h"

class RealDash
{
	public:	    

	HardwareSerial* MinhaSerial;
	long Baund;
	RealDash(HardwareSerial *minhaSerial, long baund){

		MinhaSerial = minhaSerial;
		Baund = baund;
	}

	void iniciar(){
		MinhaSerial->begin(Baund);
	}
	 void SendCANFrameToSerial(unsigned long canFrameId, const byte* frameData){
	  // the 4 byte identifier at the beginning of each CAN frame
	  // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
	  const byte serialBlockTag[4] = { 0x44, 0x33, 0x22, 0x11 };
	  MinhaSerial->write(serialBlockTag, 4);
	  // the CAN frame id number (as 32bit little endian value)
	  MinhaSerial->write((const byte*)&canFrameId, 4);
	  
	  // CAN frame payload
	  MinhaSerial->write(frameData, 8);
	  
	}


	void SendTextExtensionFrameToSerial(unsigned long canFrameId, const char* text)	{
	  if (text)
	  {
	    // the 4 byte identifier at the beginning of each CAN frame
	    // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
	    const byte textExtensionBlockTag[4] = { 0x55, 0x33, 0x22, 0x11 };
	    MinhaSerial->write(textExtensionBlockTag, 4);
	    

	    // the CAN frame id number (as 32bit little endian value)
	    MinhaSerial->write((const byte*)&canFrameId, 4);
	    

	    // text payload
	    MinhaSerial->write(text, strlen(text) + 1);
	    
	  }
	}


	
};
