#ifndef MEMORY_H
#define MEMORY_H


#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#define MEMSIZE 256 

SC_MODULE(Memory)
{
	tlm_utils::simple_target_socket<Memory> socket_s;
	int memsource[MEMSIZE];
	int memtarget[MEMSIZE];
	
	virtual void b_transport( tlm::tlm_generic_payload&, sc_time& );
	
	SC_CTOR(Memory) : socket_s("socket_s") 
	{
		socket_s.register_b_transport(this, &Memory::b_transport);
		// Initialize memory with random data
		for (int i = 0; i < MEMSIZE; i++)
		{
			memsource[i] = rand() % 256;
		}
		for (int i = 0; i < MEMSIZE; i++)
		{
			memtarget[i] = rand() % 256;
		}
	}
};

#endif
