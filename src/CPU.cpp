#include "CPU.h"
#define DEBUGMODE 1
using namespace sc_core;
using namespace sc_dt;
using namespace std;

void CPU::cpu() 
{
	int addrBias;
	while (1) 
	{
		// addrBias = 0;
		// state = 0;
		if(reset.read() == 0)
		{
			baseAddr_of_DMA = 0xa0000000;
			addrBias = 0;
			state = 0;
		}
		else
		{
			tlm::tlm_generic_payload *cpu_m = new tlm::tlm_generic_payload;
			sc_time delay = sc_time(10,SC_NS);
			unsigned int source = 0xaa000000;
			unsigned int target = 0xbb000000;
			unsigned int size	= 0x00000020; // 32byte
			unsigned int start	= 0x00000001;
			while(state < 4) 
			{
				if(state == 0) 
				{
					cout << "--- state 0 CPU write source to DMA" << endl;
					cpu_m->set_command(tlm::TLM_WRITE_COMMAND);
					cpu_m->set_address(baseAddr_of_DMA + addrBias);
					cpu_m->set_data_ptr(reinterpret_cast<unsigned char*>(&source) );
					cpu_m->set_data_length(4);
					cpu_m->set_streaming_width(4);
					cpu_m->set_byte_enable_ptr(0);
					cpu_m->set_dmi_allowed(false);
					cpu_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE );
					socket_m->b_transport(*cpu_m, delay);
				}
				else if(state == 1) 
				{
					cout << "--- state 1 CPU write target to DMA" << endl;
					cpu_m->set_command(tlm::TLM_WRITE_COMMAND);
					cpu_m->set_address(baseAddr_of_DMA + addrBias);
					cpu_m->set_data_ptr(reinterpret_cast<unsigned char*>(&target) );
					cpu_m->set_data_length(4);
					cpu_m->set_streaming_width(4);
					cpu_m->set_byte_enable_ptr(0);
					cpu_m->set_dmi_allowed(false);
					cpu_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE );
					socket_m->b_transport(*cpu_m, delay);
				}
				else if(state == 2) 
				{
					cout << "--- state 2 CPU write size to DMA" << endl;
					cpu_m->set_address(baseAddr_of_DMA + addrBias);
					cpu_m->set_data_ptr(reinterpret_cast<unsigned char*>(&size) );
					cpu_m->set_data_length(4);
					cpu_m->set_streaming_width(4);
					cpu_m->set_byte_enable_ptr(0);
					cpu_m->set_dmi_allowed(false);
					cpu_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE );
					socket_m->b_transport(*cpu_m, delay);
				}
				else if(state == 3) 
				{
					cout << "--- state 3 CPU write start to DMA" << endl;
					cpu_m->set_command(tlm::TLM_WRITE_COMMAND);
					cpu_m->set_address(baseAddr_of_DMA + addrBias);
					cpu_m->set_data_ptr(reinterpret_cast<unsigned char*>(&start) );
					cpu_m->set_data_length(4);
					cpu_m->set_streaming_width(4);
					cpu_m->set_byte_enable_ptr(0);
					cpu_m->set_dmi_allowed(false);
					cpu_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE );
					socket_m->b_transport(*cpu_m, delay);
				}
				addrBias = addrBias + 4;
				state = state + 1;
			}
		}
		if(interrupt.read() == 1)
		{
			clear.write(1); 
		}		
		wait();
	}
}
