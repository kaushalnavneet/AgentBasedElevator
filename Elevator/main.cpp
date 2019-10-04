#include <stdio.h>

#include <assert.h>
#include <chrono>
#include <math.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <boost/simulation/pdevs/atomic.hpp>
#include <boost/simulation.hpp>
#include <Elevator1.hpp>
#include <Elevator2.hpp>
#include <ElevatorController.hpp>
#include "eTime.h"
#include "eMessage.hpp"
#include "internal/CmdValue.hpp"
// #include "user_drivers/stop_driver.hpp"
// #include "user_drivers/stop_port.hpp"
// #include "user_drivers/wheels_driver.hpp"
// #include "user_drivers/wheels_port.hpp"
#include "vendor/input_event_stream.hpp"


using namespace boost::simulation::pdevs;

using namespace boost::simulation;
using namespace boost::simulation::pdevs::basic_models;
using namespace std;

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Wmissing-declarations"
// #pragma GCC diagnostic ignored "-Wreturn-type"
string readInput(string fname);

int main() {
	printf("Nucleo Board - Embedded CD-Boost \n\n");

	// Atomic models definition
	printf("Creating atomic models Econtrol, elev1, elev2 ... \n");
	auto econtrol = make_atomic_ptr<ElevatorController<Time, Message>>();
	auto elev1 = make_atomic_ptr<Elevator1<Time, Message>>();
	auto elev2 = make_atomic_ptr<Elevator2<Time, Message>>();

	//Coupled model definition
	printf("Creating Coupled model - CU ... \n");
	shared_ptr<flattened_coupled<Time, Message>> ControlUnit(new flattened_coupled<Time, Message>(
		{econtrol,elev1,elev2},
		{econtrol},
		{{econtrol,elev1}, {econtrol,elev2}},
		{elev1, elev2}
	));


	//Top I/O port definition
	printf("Creating input atomic models ... \n");

	shared_ptr<istringstream> pointer_iss1{ new istringstream{} };
	  pointer_iss1->str(readInput("input/start_ip.txt"));
	  auto start = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss1, Time(0),
	                [](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
	        m_next.clear();
	        istringstream ss;
	        ss.str(s);
	        ss >> t_next;
	        ss >> m_next.port;
	        ss >> m_next.val.val;
	        string thrash;
	        ss >> thrash;
	        if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss2{ new istringstream{} };
	  pointer_iss2->str(readInput("input/floor0_ip.txt"));
	  auto floor0 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss2, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss3{ new istringstream{} };
	  pointer_iss3->str(readInput("input/floor1_ip.txt"));
	  auto floor1 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss3, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss4{ new istringstream{} };
	  pointer_iss4->str(readInput("input/floor2_ip.txt"));
	  auto floor2 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss4, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss5{ new istringstream{} };
	  pointer_iss5->str(readInput("input/floor3_ip.txt"));
	  auto floor3 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss5, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss6{ new istringstream{} };
	  pointer_iss6->str(readInput("input/floor4_ip.txt"));
	  auto floor4 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss6, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss7{ new istringstream{} };
	  pointer_iss7->str(readInput("input/floor5_ip.txt"));
	  auto floor5 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss7, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss8{ new istringstream{} };
	  pointer_iss8->str(readInput("input/floor6_ip.txt"));
	  auto floor6 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss8, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });
	shared_ptr<istringstream> pointer_iss9{ new istringstream{} };
	  pointer_iss9->str(readInput("input/floor7_ip.txt"));
	  auto floor7 = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss9, Time(0),
					[](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
			m_next.clear();
			istringstream ss;
			ss.str(s);
			ss >> t_next;
			ss >> m_next.port;
			ss >> m_next.val.val;
			string thrash;
			ss >> thrash;
			if ( 0 != thrash.size()) throw exception();
	  });

	  //Top model definition
	  		printf("Creating Main Coupled model - Elevator Control ... \n");
	  		shared_ptr<flattened_coupled<Time, Message>> ElevControl (new flattened_coupled<Time, Message>{
	  			{start, floor0, floor1, floor2, floor3, floor4, floor5, floor6, floor7, ControlUnit},
	  			{},
	  			{{start, ControlUnit}, {floor0, ControlUnit}, {floor1, ControlUnit}, {floor2, ControlUnit}, {floor3, ControlUnit}, {floor4, ControlUnit}, {floor5, ControlUnit}, {floor6, ControlUnit}, {floor7, ControlUnit}},
	  			{ControlUnit}
	  			});
	  //Output File and Runner
		Time initial_time = Time();
		ofstream out_data("ElevatorOutput.txt");
		printf("Creating runner ... \n");
		runner<Time, Message> root(ElevControl, initial_time, out_data, [](ostream& os, Message m){ os << m;});
		Time end_time = Time(80,0,0,0);
		//Time end_time = Time(1000);
		printf("Calling run endtime... \n");
		end_time = root.runUntil(end_time);
		printf("Application executed successfully. \n");
}

//Function to read the content of input file
string readInput(string fname){
	  ifstream file(fname);
	  string str;
	  string file_contents;
	  while (getline(file, str)){
	    file_contents += str;
	    file_contents.push_back('\n');
	  }
	  return file_contents;
}

// ----------------------------------------------------------------------------
;
