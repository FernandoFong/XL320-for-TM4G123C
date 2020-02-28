#include "dxl_pro.h"
#include "XL320.h"
#include <stdlib.h>
#include <stdarg.h>

#define NANO_TIME_DELAY 12000

XL320::XL320() {}

XL320::~XL320() {}

void XL320::begin(Stream &stream)
{
  this->stream = &stream;
  
}

void XL320::moveJoint(int id, int val) {

  int addr = XL_GOAL_POSITION_L;
  sendPacket(id, addr, val);
  this->stream->flush();

}

void XL320::setJointSpeed(int id, int val) {

  int addr = XL_GOAL_SPEED_L;
  sendPacket(id, addr, val);
  this->stream->flush();
  
}

void XL320::LED(int id, char led_color[]) {

  int addr = XL_LED;
  int val = 0;
  switch(led_color[0]) {
  case 'r':
    val = 1;
    break;
  case 'g':
    val = 2;
    break;
  case 'y':
    val = 3;
    break;
  case 'b':
    val = 4;
    break;
  case 'p':
    val = 5;
    break;
  case 'c':
    val = 6;
    break;
  case 'w':
    val = 7;
    break;
  }
  sendPacket(id, addr, val);
  this->stream->flush();
  
}

void XL320::setJointTorque(int id, int val) {
  int Address = XL_GOAL_TORQUE;
	sendPacket(id, Address, val);
	this->stream->flush();
	nDelay(NANO_TIME_DELAY);
}

void XL320::TorqueON(int id){
	
	int Address = XL_TORQUE_ENABLE;
	int value = 1;
	
	sendPacket(id, Address, value);
	this->stream->flush();
	nDelay(NANO_TIME_DELAY);
}

void XL320::TorqueOFF(int id){
	
	int Address = XL_TORQUE_ENABLE;
	int value = 0;
	
	sendPacket(id, Address, value);
	this->stream->flush();
	nDelay(NANO_TIME_DELAY);
}

int XL320::getSpoonLoad(){
	int spoon = RXsendPacket(5, XL_PRESENT_LOAD);
	this->stream->flush();
	return spoon;
}

int XL320::getJointPosition(int id) {
    return getJointPosition(id,NULL);
}

int XL320::getJointPosition(int id, Stream *debugStream){
    unsigned char buffer[255];
    RXsendPacket(id, XL_PRESENT_POSITION, 2); 
    this->stream->flush();
    if(this->readPacket(buffer,255)>0) {
      Packet p(buffer,255);
      //if(debugStream) p.toStream(*debugStream);
      if(p.isValid() && p.getParameterCount()>=3) {
	return (p.getParameter(1))|(p.getParameter(2)<<8);
      } else {
	return -1;
      }
    }
    return -2;
}

int XL320::getJointSpeed(int id) {
	return getJointSpeed(id, NULL);
}

int XL320::getJointSpeed(int id, Stream *debugStream){
	unsigned char buffer[255];
	RXsendPacket(id, XL_PRESENT_SPEED, 2);
	this->stream->flush();
	if (this->readPacket(buffer, 255)>0) {
		Packet p(buffer, 255);
		//if(debugStream) p.toStream(*debugStream);
		//if (p.isValid() && p.getParameterCount() >= 3) {
			return (p.getParameter(1)) | (p.getParameter(2) << 8);
		//}
		//else {
			return -1;
		//}
	}
	return -2;
}

int XL320::getJointLoad(int id){
	unsigned char buffer[255];
	//Direction_Pin = 1;
	RXsendPacket(id, XL_PRESENT_LOAD, 2);
	//Direction_Pin = 0;
	this->stream->flush();
	//Direction_Pin = 1;
	if (this->readPacket(buffer, 255)>0) {
		Packet p(buffer, 255);
		//if(debugStream) p.toStream(*debugStream);
		if (p.isValid() && p.getParameterCount() >= 3) {
			return (p.getParameter(1)) | (p.getParameter(2) << 8);
		}
		else {
			return -1;
		}
	}
	return -2;
}

int XL320::getJointTemperature(int id){
	unsigned char buffer[255];
	//Direction_Pin = 1;
	RXsendPacket(id, XL_PRESENT_TEMPERATURE, 2);
	//Direction_Pin = 0;
	this->stream->flush();
	//Direction_Pin = 1;
	if (this->readPacket(buffer, 255)>0) {
		Packet p(buffer, 255);
		//if(debugStream) p.toStream(*debugStream);
		if (p.isValid() && p.getParameterCount() >= 3) {
			return (p.getParameter(1)) | (p.getParameter(2) << 8);
		}
		else {
			return -1;
		}
	}
	return -2;
}

int XL320::isJointMoving(int id){
    int motion = RXsendPacket(id, XL_MOVING);
    this->stream->flush();
    nDelay(NANO_TIME_DELAY);
    return motion;
}

void XL320::nDelay(int nTime) {
  
}

int XL320::sendPacket(int id, int Address, int value){

  /*Dynamixel 2.0 communication protocol
    used by Dynamixel XL-320 and Dynamixel PRO only.
  */

  // technically i think we need 14bytes for this packet 

  const int bufsize = 16;

  unsigned char txbuffer[bufsize];

  Packet p(txbuffer,bufsize,id,0x03,4,
           DXL_LOBYTE(Address),
           DXL_HIBYTE(Address),
           DXL_LOBYTE(value),
           DXL_HIBYTE(value));


  int size = p.getSize();
  stream->write(txbuffer,size);
  //p.toStream(*stream);
  stream->write(txbuffer,bufsize);

  return bufsize;	
}

int XL320::RXsendPacket(int id, int Address) {
  return this->RXsendPacket(id, Address, 2);
} 

int XL320::RXsendPacket(int id, int Address, int size){

	/*Dynamixel 2.0 communication protocol
	  used by Dynamixel XL-320 and Dynamixel PRO only.
	*/

  const int bufsize = 16;

  unsigned char txbuffer[bufsize];

  Packet p(txbuffer,bufsize,id,0x02,4,
           DXL_LOBYTE(Address),
           DXL_HIBYTE(Address),
           DXL_LOBYTE(size),
           DXL_HIBYTE(size));


  stream->write(txbuffer,p.getSize());
  //p.toStream(*stream);
  return p.getSize();
}

#define FSM
#define STATE(x)        s_##x : if(!stream->readBytes(&BUFFER[I++],1)) goto sx_timeout ; if(I>=SIZE) goto sx_overflow; sn_##x :
#define THISBYTE        (BUFFER[I-1])
#define NEXTSTATE(x)    goto s_##x
#define NEXTSTATE_NR(x) goto sn_##x
#define OVERFLOW        sx_overflow :
#define TIMEOUT         sx_timeout :

int XL320::readPacket(unsigned char *BUFFER, size_t SIZE) {
    int C;
    int I = 0;

    int length = 0;

      // state names normally name the last parsed symbol

    FSM {
      STATE(start) {
	if(THISBYTE==0xFF) NEXTSTATE(header_ff_1);
	I=0; NEXTSTATE(start);
      }
      STATE(header_ff_1) {
	if(THISBYTE==0xFF) NEXTSTATE(header_ff_2);
	I=0; NEXTSTATE(start);	
      }
      STATE(header_ff_2) {
	if(THISBYTE==0xFD) NEXTSTATE(header_fd);
	// yet more 0xFF's? stay in this state
	if(THISBYTE==0xFF) NEXTSTATE(header_ff_2);
	// anything else? restart
	I=0; NEXTSTATE(start);
      }
      STATE(header_fd) {
	  // reading reserved, could be anything in theory, normally 0
      }
      STATE(header_reserved) {
	  // id = THISBYTE
      }
      STATE(id) {
	length = THISBYTE;
      }
      STATE(length_1) {
	length += THISBYTE<<8; // eg: length=4
      }
      STATE(length_2) {
      }
      STATE(instr) {
	// instr = THISBYTE
        // check length because
        // action and reboot commands have no parameters
	if(I-length>=5) NEXTSTATE(checksum_1);
      }
      STATE(params) {
	  // check length and maybe skip to checksum
	  if(I-length>=5) NEXTSTATE(checksum_1);
	  // or keep reading params
	  NEXTSTATE(params);
      }
      STATE(checksum_1) {
      }
      STATE(checksum_2) {
	  // done
	  return I; 
      }
      OVERFLOW {
          return -1;
      }
      TIMEOUT {
	  return -2;
      }

    }
}


XL320::Packet::Packet(
	unsigned char *data,
	size_t data_size,
	unsigned char id,
	unsigned char instruction,
	int parameter_data_size,
	...) {


    // [ff][ff][fd][00][id][len1][len2] { [instr][params(parameter_data_size)][crc1][crc2] }
    unsigned int length=3+parameter_data_size;
    if(!data) {
	// [ff][ff][fd][00][id][len1][len2] { [data(length)] }
	this->data_size = 7+length;   
	this->data = (unsigned char*)malloc(data_size);
	this->freeData = true;
    } else {
	this->data = data;
	this->data_size = data_size;
	this->freeData = false;
    }
    this->data[0]=0xFF;
    this->data[1]=0xFF;
    this->data[2]=0xFD;
    this->data[3]=0x00;
    this->data[4]=id;
    this->data[5]=length&0xff;
    this->data[6]=(length>>8)&0xff;
    this->data[7]=instruction;
    va_list args;
    va_start(args, parameter_data_size); 
    for(int i=0;i<parameter_data_size;i++) {
	unsigned char arg = va_arg(args, int);
	this->data[8+i]=arg;
    }
    unsigned short crc = update_crc(0,this->data,this->getSize()-2);
    this->data[8+parameter_data_size]=crc&0xff;
    this->data[9+parameter_data_size]=(crc>>8)&0xff;
    va_end(args);
}

XL320::Packet::Packet(unsigned char *data, size_t size) {
    this->data = data;
    this->data_size = size;
    this->freeData = false;
}


XL320::Packet::~Packet() {
    if(this->freeData==true) {
	free(this->data);
    }
}

void XL320::Packet::toStream(Stream &stream) {
  stream.print("id: ");
  stream.println(this->getId(),DEC);
  stream.print("length: ");
  stream.println(this->getLength(),DEC);
  stream.print("instruction: ");
  stream.println(this->getInstruction(),HEX);
  stream.print("parameter count: ");
  stream.println(this->getParameterCount(), DEC);
  for(int i=0;i<this->getParameterCount(); i++) {
    stream.print(this->getParameter(i),HEX);
    if(i<this->getParameterCount()-1) {
      stream.print(",");
    }
  }
  stream.println();
  stream.print("valid: ");
  stream.println(this->isValid() ? "yes" : "no");
}

unsigned char XL320::Packet::getId() {
    return data[4];
}

int XL320::Packet::getLength() {
    return data[5]+((data[6]&0xff)<<8);
}

int XL320::Packet::getSize() {
    return getLength()+7;
}

int XL320::Packet::getParameterCount() {
    return getLength()-3;
}

unsigned char XL320::Packet::getInstruction() {
    return data[7];
}

unsigned char XL320::Packet::getParameter(int n) {
    return data[8+n];
}

bool XL320::Packet::isValid() {
    int length = getLength();
    unsigned short storedChecksum = data[length+5]+(data[length+6]<<8);
    return storedChecksum == update_crc(0,data,length+5);
}

