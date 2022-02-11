#include "DMA.h"
#define DEBUGMODE 1
using namespace sc_core;
using namespace sc_dt;
using namespace std;

//tlm::TLM_READ_COMMAND = 0
//tlm::TLM_WRITE_COMMAND = 1


//dma is slave
void DMA::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
{	
	tlm::tlm_command cmd_s = trans.get_command();
	sc_dt::uint64 addr_s = trans.get_address();
	unsigned char *data_s = trans.get_data_ptr();
	#ifdef DEBUGMODE
	cout << ">> now start transaction (CPU and DMA) " << endl;
	cout << setw(10) << setfill(' ') << sc_time_stamp() << endl;
	cout << setw(20) << setfill(' ') <<' ';
	cout << "Command(R/W) : " << (cmd_s ? "write" : "read") << endl;
	cout << setw(20) << setfill(' ') <<' ';
	cout << "Address : 0x" << setw(8) << setfill('0') << hex << addr_s << endl;
	#endif
	if (cmd_s == tlm::TLM_WRITE_COMMAND) 
	{
		addr_bais = addr_s - DMA_baseaddr.read();
		slave.data = *(reinterpret_cast<int*>(data_s));
		
		#ifdef DEBUGMODE
		cout << setw(20) << setfill(' ') <<' ';
		cout << "slave.data	   : 0x" << setw(2) << setfill('0');
		cout << hex << *(reinterpret_cast<int*>(data_s)) << endl;
		cout << "addr = " << addr_bais << endl;
		cout << "data = " << *(reinterpret_cast<int*>(data_s)) << endl;
		#endif
		
		s_rw = cmd_s;
	} 
	else if(cmd_s ==tlm::TLM_READ_COMMAND)
	{
		s_rw = cmd_s;
	}
	wait(delay);	
	trans.set_response_status(tlm::TLM_OK_RESPONSE);		
}

void DMA::dma() {
	sc_uint<32> temp_source, temp_target;
	unsigned char R_data;
	int i, count;
	
	R_SOURCE = 0;
	R_TARGET = 0;
	R_SIZE = 0;
	R_START = 0;
	temp_source = 0;
	temp_target = 0;
	interrupt.write(0);
	
	R_data = 0;
	while (1) {
		tlm::tlm_generic_payload *trans_m = new tlm::tlm_generic_payload;
		sc_time delay = sc_time(10,SC_NS);
		#ifdef DEBUGMODE
		cout << setw(10) << setfill(' ') << "DMA 68: " << sc_time_stamp() << endl;
		cout << setw(10) << setfill(' ') << "reset = " << reset.read() << endl;
		cout << setw(10) << setfill(' ') << "interrupt = " << interrupt.read() << endl;
		cout << setw(10) << setfill(' ') << "clear = " << clear.read() << endl;
		#endif
		if(reset.read() == 0 || interrupt.read() == 1)
		{
			R_SOURCE = 0;
			R_TARGET = 0;
			R_SIZE = 0;
			R_START = 0;
			R_SIZE = 0;
			temp_source = 0;
			temp_target = 0;
			if(clear.read() == 1)
			{
				interrupt.write(0);
			}
			#ifdef DEBUGMODE
			cout << setw(10) << setfill(' ') << "DMA 85: " << sc_time_stamp() << endl;
			#endif
		}
		else
		{	
			#ifdef DEBUGMODE
			cout << setw(10) << setfill(' ') << "DMA 89: " << sc_time_stamp() << endl;
			cout << setw(10) << setfill(' ') << "s_rw = " << s_rw << endl;
			cout << setw(10) << setfill(' ') << "R_START = " << R_START << endl;
			#endif
			if(s_rw == tlm::TLM_WRITE_COMMAND && R_START == 0) 
			{	// write control register
				#ifdef DEBUGMODE
				cout << setw(10) << setfill(' ') << "DMA 94: " << sc_time_stamp() << endl;
				#endif
				if(addr_bais == 0x0) 
				{
					R_SOURCE = slave.data;
				} 
				else if(addr_bais == 0x4) 
				{
					R_TARGET = slave.data;
				} 
				else if(addr_bais == 0x8) 
				{
					R_SIZE = slave.data;
				} 
				else if(addr_bais == 0xc) 
				{
					R_START = slave.data;
				}
				#ifdef DEBUGMODE
				cout << "Command : " << (s_rw ? "write" : "read") <<endl;
				cout << "R_SOURCE : " << R_SOURCE <<endl;
				cout << "R_TARGET : " << R_TARGET <<endl;
				cout << "R_SIZE : " << R_SIZE <<endl;
				cout << "R_START : " << R_START << endl; 
				#endif
			}
		}

		if(R_START == 1 && R_SIZE != 0) 
		{
			temp_source = R_SOURCE;
			temp_target = R_TARGET;
			
			#ifdef DEBUGMODE
			cout << endl;
			cout << "start mem transfer " << endl;
			cout << "R_SIZE : " << R_SIZE << endl;
			cout << "R_SOURCE : " << temp_source << endl;
			cout << "R_TARGET : " << temp_target << endl;
			#endif
			for(i = R_SIZE; i > 0; i = i - 4) 
			{
				count = 0;
				while(count < 2) 
				{
					if(count == 0) 
					{
						cout << ">> DMA read data from source memory" << endl;
						trans_m->set_command(tlm::TLM_READ_COMMAND);
						trans_m->set_address(temp_source);
						trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
						trans_m->set_data_length(4);
						trans_m->set_streaming_width(4);
						trans_m->set_byte_enable_ptr(0);
						trans_m->set_dmi_allowed(false);
						trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
						socket_m->b_transport(*trans_m, delay );
					} 
					else if(count == 1) 
					{
						cout << ">> DMA write data to target memory" << endl;
						trans_m->set_command(tlm::TLM_WRITE_COMMAND);
						trans_m->set_address(temp_target);
						trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
						trans_m->set_data_length(4);
						trans_m->set_streaming_width(4);
						trans_m->set_byte_enable_ptr(0);
						trans_m->set_dmi_allowed(false);
						trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
						socket_m->b_transport(*trans_m, delay);
					}
					count = count + 1;
					R_data = *(reinterpret_cast<int*>(&data_m));
					#ifdef DEBUGMODE
					cout << "addr_source = " << temp_source << ",	addr_target = " << temp_target <<endl;
					cout << "data = " << R_data << endl;
					#endif
					if(i == 4 && count == 1) interrupt.write(1);
					wait();
				}
				temp_source = temp_source + 4;
				temp_target = temp_target + 4;
				#ifdef DEBUGMODE
				cout << setw(10) << setfill(' ') << "i: " << i << "	   "<< sc_time_stamp() << endl;
				cout << setw(10) << setfill(' ') << "DMA 174: " << sc_time_stamp() << endl;
				#endif
			}
			#ifdef DEBUGMODE
			cout << setw(10) << setfill(' ') << "DMA 175: " << sc_time_stamp() << endl;
			#endif
			//interrupt.write(1);
		}
		if(clear.read() == 1)
		{
			interrupt.write(0);
		}
		wait();
	}
}



