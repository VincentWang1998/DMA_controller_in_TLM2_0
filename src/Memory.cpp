#include "Memory.h"

#define DEBUGMODE 1
using namespace sc_core;
using namespace sc_dt;
using namespace std;

void Memory::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
{
	tlm::tlm_command cmd = trans.get_command();
	sc_dt::uint64	 adr = trans.get_address();
	unsigned char*	 ptr = trans.get_data_ptr();
	unsigned int	 len = trans.get_data_length();
	unsigned char*	 byt = trans.get_byte_enable_ptr();
	unsigned int	 wid = trans.get_streaming_width();

	if((adr & 0xff000000) == 0xaa000000){ //ifchoose source mem
		if(cmd == tlm::TLM_READ_COMMAND)
			memcpy(ptr, &memsource[(adr - 0xaa000000)/4], len);
		else if(cmd == tlm::TLM_WRITE_COMMAND )
			memcpy(&memsource[(adr - 0xaa000000)/4], ptr, len);
	}
	else if((adr & 0xff000000) == 0xbb000000){ //ifchoose target mem
		if(cmd == tlm::TLM_READ_COMMAND)
			memcpy(ptr, &memtarget[(adr - 0xbb000000)/4], len);
		else if(cmd == tlm::TLM_WRITE_COMMAND)
			memcpy(&memtarget[(adr - 0xbb000000)/4], ptr, len);
	}
	trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
