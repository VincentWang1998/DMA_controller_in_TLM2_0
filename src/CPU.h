#ifndef CPU_H
#define CPU_H

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#include <iomanip> //for debug

SC_MODULE(CPU)
{
	sc_uint<32>		baseAddr_of_DMA;
	sc_in<bool>		clock;
	sc_in<bool>		reset;
	sc_in<bool>		interrupt;
	sc_out<bool>	clear;
	//enum state{writeSource, writeTarget, writeSize, writeStart};
	//state CPUstate;
	int state;
	tlm_utils::simple_initiator_socket<CPU> socket_m;
	
	void cpu();
	
	SC_CTOR(CPU) : socket_m("socket_m") {
		SC_CTHREAD(cpu,clock.pos());
		reset_signal_is(reset,false);
	}
};

#endif