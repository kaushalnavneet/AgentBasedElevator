/*
 * ElevatorController.hpp
 *
 *  Dated: Mar 2019
 *  Authors: Navneet Kaushal
 */

#ifndef ElevatorController_HPP_
#define ElevatorController_HPP_

//#include <boost/any.hpp>
#include <boost/simulation/pdevs/atomic.hpp>
#include "commonH.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
//using namespace std;
// #include "SWO.h"
// #include "mbed.h"

namespace boost {
namespace simulation {
namespace pdevs {
namespace basic_models {
/**
 * @brief ElevatorController PDEVS Model
 *
 * SensorController PDEVS Model():
 * - X = {(econtrol_start_in, {START,STOP}); (econtrol_floor0_in, {TOKEN,VALUE}); (econtrol_floor1_in, {TOKEN,VALUE});
 * 			(econtrol_floor2_in, {TOKEN,VALUE}); (econtrol_floor3_in, {TOKEN,VALUE}); (econtrol_floor4_in, {TOKEN,VALUE});
 * 			(econtrol_floor5_in, {TOKEN,VALUE}); (econtrol_floor6_in, {TOKEN,VALUE}); (econtrol_floor7_in, {TOKEN,VALUE});}
 * - Y = {(start_out, {START,STOP}); (econtrol_elev1_out, {FLOOR,VALUE}); (econtrol_elev2_out, {FLOOR,VALUE}); }
 *
 * - S = { "WAIT_DATA", "IDLE", "PREP_RX", "TX_DATA", "PREP_STOP" }
 *

 */
template<class TIME, class MSG>
class ElevatorController : public atomic<TIME, MSG>
{
	TIME _next;
	std::vector<MSG> _outvalue;
private:
	enum State {IDLE, PREP_RX, WAIT_DATA, TX_DATA, PREP_STOP};
	enum Port {econtrol_start_in, econtrol_floor0_in, econtrol_floor1_in, econtrol_floor2_in, econtrol_floor3_in, econtrol_floor4_in, econtrol_floor5_in, econtrol_floor6_in, econtrol_floor7_in, econtrol_elev1_in, econtrol_elev1_out, econtrol_elev2_in, econtrol_elev2_out, econtrol_start_out};
	std::string portName[14];
	State _state;
	int floor0 = 0, floor1 = 0, floor2 = 0, floor3 = 0, floor4 = 0, floor5 = 0, floor6 = 0, floor7 = 0;
    int floorNum = 0;
	TIME scTxTime = TIME(00,00,00,050);
	TIME scRxPrepTime = TIME(00,00,00,050);
	const TIME infinity=TIME(99,00,00,00);

public:
    /**
     * @brief ElevatorController constructor.
     */
    explicit ElevatorController(const std::string &n = "econtrol") noexcept : _next(infinity), _outvalue(std::vector<MSG>{}),_state(State::IDLE) {
    	_state = State::IDLE;
    	portName[0] = "start";
    	portName[1] = "floor0";
    	portName[2] = "floor1";
    	portName[3] = "floor2";
    	portName[4] = "floor3";
    	portName[5] = "floor4";
    	portName[6] = "floor5";
    	portName[7] = "floor6";
    	portName[8] = "floor7";
    	portName[9] = "ELEV1_IN";
    	portName[10] = "ECONTROL_OUT1";
    	portName[11] = "ELEV2_IN";
    	portName[12] = "ECONTROL_OUT2";
    	portName[13] = "START_OUT";
    	_next = infinity;
    }

	/**
	 * @brief internal function.
	 */
	void internal() noexcept {
		switch (_state){
		case PREP_STOP:
			_state = IDLE;
			_next = infinity;
			break;
		case PREP_RX:
		case TX_DATA:
			_state = WAIT_DATA;
			_next = infinity;
			break;
		case IDLE:
			_next = infinity;
		}
	}

	/**
	 * @brief advance function.
	 * @return Time until next internal event.
	 */
	TIME advance() const noexcept {
		return _next;
	}

	/**
	 * @brief out function.
	 * @return MSG defined in construction.
	 */
	std::vector<MSG> out() const noexcept {
		std::map<std::string, std::string> logic_map;
		std::string action;
		MSG _outputMessageM1;

		//check the distance of requested floor from both elevators
		int d1 = abs(floorNum - curr_flr1);
		int d2 = abs(floorNum - curr_flr2);

		//call the Agent file (AgentLogic) and move elevator as per the logic
	    std::ifstream inFile ("AgentLogic.txt");
	    std::string eline;
	    std::string s1;
	    std::string s2;
	    int elevNum;
	    if (!inFile) {
	    	std::cout << "Unable to open file";
	        exit(1); // terminate with error
	    }
	    if(inFile.is_open()){
			while(getline(inFile, eline)) {
				s1 = eline.substr(0, eline.find(":"));
				s2 = eline.substr(eline.find(":")) + "f";
				s2 = s2.substr(2,2);
				//create logic_map based on the AgentBrain logic
				logic_map.insert(std::pair<std::string, std::string>(s1,s2));
			}
	    inFile.close();
	    }

	    if(d1 > d2){
	    	action = logic_map["d1 > d2 "];
	    }
	    else if(d1 < d2){
	    	action = logic_map["d1 < d2 "];
	    }
	    else if(d1 == d2){
	    	if(logic_map["d1 = d2 "] == "e1"){
	    		action = "e1";
	    	}else if(logic_map["d1 = d2 "] == "e2"){
	    		action = "e2";
	    	}
	    }

	    if(action == "e1"){
	    	elevNum = 10;	//portName[econtrol_elev1_out]
	    }
	    else if(action == "e2"){
	    	elevNum = 12;	//portName[econtrol_elev2_out]
	    }

		switch (_state){

			case PREP_STOP:
				_outputMessageM1 = MSG(portName[elevNum],TOKEN, STOP_PROC); //Stop Elevators
				return std::vector<MSG>{_outputMessageM1};

			case PREP_RX:
				_outputMessageM1 = MSG(portName[econtrol_start_out],TOKEN, START_PROC); //Start All Floor Buttons
				return std::vector<MSG>{_outputMessageM1};

			case TX_DATA:
				switch(floorNum){
					case 0:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 0); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 1:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 1); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 2:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 2); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 3:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 3); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 4:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 4); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 5:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 5); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 6:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 6); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
					case 7:_outputMessageM1 = MSG(portName[elevNum],FLOOR, 7); //Send feedback to Elevator1 or Elevator2
					return std::vector<MSG>{_outputMessageM1};
					break;
				}
		};

		return std::vector<MSG>{};
	}

	/**
	 * @brief external function
	 * @param msg external input message.
	 * @param t time the external input is received.
	 */
	void external(const std::vector<MSG>& mb, const TIME& t) noexcept {
		MSG msg = mb.back();
		if (msg.port == portName[econtrol_start_in]){
			if(_state == IDLE && msg.val.val == START_PROC){
				std::cout << "Elevator Start button is pressed." << std::endl;
				std::cout << "Both elevators are at floor0." << std::endl;
				_state = PREP_RX;
				_next = scRxPrepTime;
			}
			else if (msg.val.val == STOP_PROC) {
				_state = PREP_STOP;
				_next = TIME::Zero;
			}
		}
		else if (msg.port == portName[econtrol_floor0_in]){
			if(_state == WAIT_DATA) {
				floor0 = static_cast<int>(msg.val.val);
				if(floor0 == 1){
				floorNum = 0;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor1_in]){
			if(_state == WAIT_DATA) {
				floor1 = static_cast<int>(msg.val.val);
				if(floor1 == 1){
				floorNum = 1;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor2_in]){
			if(_state == WAIT_DATA) {
				floor2 = static_cast<int>(msg.val.val);
				if(floor2 == 1){
				floorNum = 2;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor3_in]){
			if(_state == WAIT_DATA) {
				floor3 = static_cast<int>(msg.val.val);
				if(floor3 == 1){
				floorNum = 3;
				_state = TX_DATA;
				_next = scTxTime;
				}

			}
		}
		else if (msg.port == portName[econtrol_floor4_in]){
			if(_state == WAIT_DATA) {
				floor4 = static_cast<int>(msg.val.val);
				if(floor4 == 1){
				floorNum = 4;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor5_in]){
			if(_state == WAIT_DATA) {
				floor5 = static_cast<int>(msg.val.val);
				if(floor5 == 1){
				floorNum = 5;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor6_in]){
			if(_state == WAIT_DATA) {
				floor6 = static_cast<int>(msg.val.val);
				if(floor6 == 1){
				floorNum = 6;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
		else if (msg.port == portName[econtrol_floor7_in]){
			if(_state == WAIT_DATA) {
				floor7 = static_cast<int>(msg.val.val);
				if(floor7 == 1){
				floorNum = 7;
				_state = TX_DATA;
				_next = scTxTime;
				}
			}
		}
	}

	/**
	 * @brief confluence function.
	 * Execute the internal.
	 *
	 * @param msg
	 * @param t time the external input is confluent with an internal transition.
	 */
	void confluence(const std::vector<MSG>& mb, const TIME& t)  noexcept  {
		internal();
		//external(mb,t);
	}

	  void print()  noexcept {/*SWO_PrintString("econtrol");*/}

};

}
}
}
}


#endif /* ElevatorController_HPP_ */
