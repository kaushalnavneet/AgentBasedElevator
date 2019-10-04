#ifndef BOOST_SIMULATION_TYPES_H
#define BOOST_SIMULATION_TYPES_H

#include <iostream>
#include <string>

#include <boost/simulation.hpp>
#include "CmdValue.hpp"
// #include "SWO.h"

#include "eTime.h"

using namespace std;
using namespace boost::simulation;
using namespace boost::simulation::pdevs;
using namespace boost::simulation::pdevs::basic_models;

using ToModel_t = std::shared_ptr<model<Time>>;
using Value = CmdValue;
using tofromPort = string;

struct Message {

  Time tm;
  ToModel_t to;
  tofromPort port;
  Value val;

  Message()
  : tm(Time::Zero), to(nullptr), port(""), val(0){}

  Message(const ToModel_t& other_model, const tofromPort& other_port, const Value& other_val)
  :tm(Time()), to(other_model), port(other_port), val(other_val){}

  //Default conStructor to use for testing
  Message(const tofromPort& other_port, const Value& other_val)
  :tm(Time()), to(nullptr), port(other_port), val(other_val){}

  Message(const tofromPort& other_port, int cmd, int val)
    :tm(Time()), to(nullptr), port(other_port), val(Value(cmd, val)){
  }


  Message(const tofromPort& other_port)
  :tm(Time()), to(nullptr), port(other_port), val(0){}

  Message(const Message& other)
  : tm(other.tm), to(other.to), port(other.port), val(other.val){}

  Message(Message* other)
  : tm(other->tm), to(other->to), port(other->port), val(other->val){}

  void clear() {
    tm     =  Time::Zero;
    to     =  nullptr;
    port   =  "";
    val    =  0;
  }

  bool isEmpty(){
	  if((tm == Time::Zero) && (to==nullptr) && (port.compare("")==0) && (val.val == 0))
		  return true;
	  else
		  return false;
  }

  void print(){
	  //Testing - Debugging purposes
	  // char buf[20];

	  // SWO_PrintString((" Time: " + tm.asString() + "\n").c_str());
	  // SWO_PrintString((" Port: " + port).c_str());
	  // sprintf(buf," Value : %d \n", val); SWO_PrintString(buf);
  }

  // Add const message = operator
};


ostream& operator<<(ostream& os, const Message& msg) {
  os << endl;
  os << "Requested Input: " << msg.val.val << endl;
  os << "Port: " << msg.port << endl;
  os << "Command(12=Up, 13=Down, 14=Stop): " << msg.val.cmd << endl;

  return os;
}


/*******************************************/
/************** End Message **************/
/*******************************************/


#endif // BOOST_SIMULATION_TYPES_H
