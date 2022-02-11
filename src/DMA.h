#ifndef DMA_H
#define DMA_H

#include <iomanip>
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#include <iomanip> //for debug

//port
struct port{
	sc_uint<32> data;
};


SC_MODULE(DMA)
{
	sc_in<bool> clock;	
	sc_in<bool> reset;
	sc_in<bool> clear;
	sc_out<bool> interrupt;
	/*---- Define master port ---- */
	struct port master;
	/*---- Define slave port ---- */
	struct port slave;
	
	tlm_utils::simple_target_socket<DMA> socket_s;
	tlm_utils::simple_initiator_socket<DMA> socket_m;

	/*---- Contorl register ----*/
	sc_uint<32> R_SOURCE;
	sc_uint<32> R_TARGET;
	sc_uint<32> R_SIZE;
	bool R_START;
	/*---- ----------------- ----*/
	
	sc_signal<sc_uint<32> > DMA_baseaddr;
	int addr_bais;
	unsigned char data_m;
	bool s_rw;
	
	void dma();
	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(DMA) : socket_s("socket_s"), socket_m("socket_m") {
		socket_s.register_b_transport(this, &DMA::b_transport);

		SC_CTHREAD(dma,clock.pos());
		reset_signal_is(reset,false);

		DMA_baseaddr = 0xa0000000;
	}
};

#endif
