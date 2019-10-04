/*
 * Elevator1.hpp
 *
 *  Dated: Mar 2019
 *  Authors: Navneet Kaushal
 */

#ifndef Elevator1_HPP_
#define Elevator1_HPP_

#include <boost/simulation/pdevs/atomic.hpp>
#include "commonH.h"

namespace boost {
namespace simulation {
namespace pdevs {
namespace basic_models {
/**
 * @brief Elevator1 PDEVS Model
 *
 * Elevator1 PDEVS Model():
 * - X = {(elev1_econtrol_in, {0,1,2,3,4,5,6,7});}
 * - Y = {(elev1_motor_out, {UP, DOWN, STOP});}
 * - S = {"PREP_START", "WAIT_DATA", "PREP_MOVE_UP", "MOVE_UP", "PREP_MOVE_DOWN", "MOVE_DOWN", "PREP_STOP", "IDLE"}
 * - internal -> See DEVS Spec Doc
 * - external -> See DEVS Spec Doc
 * - out      -> See DEVS Spec Doc
 * - advance  -> See DEVS Spec Doc
*/

template<class TIME, class MSG>
class Elevator1 : public atomic<TIME, MSG>
{
    //initialize time for next state and message that will be outputted
    TIME _next;
    std::vector<MSG> _outvalue;
private:
    //initialize states, ports,
    enum State {PREP_START, WAIT_DATA, PREP_MOVE_UP, PREP_MOVE_DOWN, MOVE_UP, MOVE_DOWN, PREP_STOP, IDLE};
    enum Port {elev1_econtrol_in, elev1_econtrol_out, elev1_motor_out};
    std::string portName[3];
    const TIME infinity = TIME(99,00,00,00);

    State _state;
    int econtrol_input = 0;
    TIME startPrepTime = TIME(00,00,00,20);
    TIME startTime = TIME(00,00,01,0);


public:
    /**
     * @brief Elevator1 constructor.
     *
     */
    explicit Elevator1(const std::string &n = "elev1") noexcept : _next(infinity), _outvalue(std::vector<MSG>{}),_state(State::WAIT_DATA) {
    	portName[0] = "ECONTROL_OUT1";
    	portName[1] = "ELEV1_OUT";
    	portName[2] = "elevator1";
    	_next = infinity;
    	printf("elev1 CREATED \n");
    }

    /**
     * @brief internal function.
     */
    void internal() noexcept {
    	switch (_state){
			case IDLE:
				_state = WAIT_DATA;
				_next = infinity;
				break;

			case MOVE_UP:
				curr_flr1=curr_flr1+1;
				if(curr_flr1< econtrol_input){
					_state = PREP_MOVE_UP;
					_next = startPrepTime;
				}
				else{
					_state = WAIT_DATA;
					_next = infinity;
					std::cout << "Elevator 1 stopped at floor " << curr_flr1 << ". Door opens." << std::endl;
				}
				std::cout << "current floor of elevator1 is: " << curr_flr1 << std::endl;
				break;

			case PREP_MOVE_UP:
				_state = MOVE_UP;
				_next = startTime;
				break;

			case MOVE_DOWN:
				curr_flr1=curr_flr1-1;
				if(curr_flr1 > econtrol_input){
					_state = PREP_MOVE_DOWN;
					_next = startPrepTime;
				}
				else{
					_state = WAIT_DATA;
					_next = infinity;
					std::cout << "Elevator 1 stopped at floor " << curr_flr1 << ". Door opens." << std::endl;
				}
				std::cout << "current floor of elevator1 is: " << curr_flr1 << std::endl;
				break;

			case PREP_MOVE_DOWN:
				_state = MOVE_DOWN;
				_next = startTime;
				break;

			case PREP_STOP:
				_state = WAIT_DATA;
				_next = infinity;
				break;

			default:
				break;
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
     * @return MSG defined below.
     */
    std::vector<MSG> out() const noexcept {
      std::vector<MSG> _out;
      _out.clear();
      if (_state==PREP_MOVE_UP)
    	{
			_out.push_back(MSG(portName[elev1_motor_out],econtrol_input,UP));
    	}
    	else if (_state== PREP_MOVE_DOWN)
    	{
    		_out.push_back(MSG(portName[elev1_motor_out],econtrol_input,DOWN));
    	}
    	else if(_state== PREP_STOP || _state== MOVE_UP || _state== MOVE_DOWN )
    	{
    		_out.push_back(MSG(portName[elev1_motor_out],econtrol_input,STOP));
    	}
      	return _out;
    }

    /**
     * @brief external function will set the new state depending on the value of the input.
     * @param msg external input message.
     * @param t time the external input is received.
     */
    void external(const std::vector<MSG>& mb, const TIME& t) noexcept {
    	MSG msg = mb.back();

    	if (msg.port == portName[elev1_econtrol_in]) {
    		if(msg.val.cmd == STOP_PROC){
    			std::cout << "Elevator Stop button is pressed." << std::endl;
    		}else{
    			std::cout << "Elevator call button is pressed from floor" << msg.val.cmd << std::endl;
    			std::cout << "..Calling elevator1.." << std::endl;
    		}

			if(_state == WAIT_DATA ) {
				econtrol_input = static_cast<int>(msg.val.cmd);
				if (econtrol_input - curr_flr1 > 0) {
					_state = PREP_MOVE_UP;
					_next = startPrepTime;
				}
				else if (econtrol_input - curr_flr1 < 0) {
					_state = PREP_MOVE_DOWN;
					_next = startPrepTime;
				}
				  else {
					  std::cout << "Elev 1 is already at the same floor. Door opens." << std::endl;
				}
			}
			if(msg.val.cmd == STOP_PROC){
				printf("Elevator controller stopped. \n");
				_state = PREP_STOP;
				_next = TIME::Zero;
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
    }

    void print()  noexcept {
    }

};

}
}
}
}


#endif /* Elevator1_HPP_ */
