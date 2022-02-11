#include "CPU.h"
#include "DMA.h"
#include "Memory.h"
#define DEBUGMODE 1

using namespace sc_core;
using namespace sc_dt;
using namespace std;

int sc_main(int argc, char* argv[]) 
{	
	sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
	//testbench
	sc_clock clock("clock", 10, SC_NS); // timer clock
	sc_signal<bool> reset;				// rst
	sc_signal<bool> clear;
	sc_signal<bool> interrupt;
	//DMA
	DMA	dmac("dmac");
	dmac.clock(clock);
	dmac.reset(reset);
	dmac.clear(clear);
	dmac.interrupt(interrupt);
	//CPU
	CPU	cpu("cpu");
	cpu.clock(clock);
	cpu.reset(reset);
	cpu.socket_m(dmac.socket_s);
	cpu.clear(clear);
	cpu.interrupt(interrupt);
	//memory
	Memory memory("memory");
	memory.socket_s(dmac.socket_m);
	//VCD	
	sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");	
	sc_trace(tf, clock, "clock");
	sc_trace(tf, reset, "reset");
	sc_trace(tf, clear, "clear");
	sc_trace(tf, interrupt, "interrupt");
	
	sc_trace(tf, dmac.R_SOURCE, "SOURCE");
	sc_trace(tf, dmac.R_TARGET, "TARGET");
	sc_trace(tf, dmac.R_SIZE, "SIZE");
	sc_trace(tf, dmac.R_START, "START");
	sc_trace(tf, dmac.slave.data, "dmac_slave_data");///
	sc_trace(tf, memory.memsource[0], "mem_SOURCE(0)");
	sc_trace(tf, memory.memtarget[0], "mem_TARGET(0)");
	sc_trace(tf, memory.memsource[1], "mem_SOURCE(1)");
	sc_trace(tf, memory.memtarget[1], "mem_TARGET(1)");
	sc_trace(tf, memory.memsource[2], "mem_SOURCE(2)");
	sc_trace(tf, memory.memtarget[2], "mem_TARGET(2)");
	sc_trace(tf, memory.memsource[3], "mem_SOURCE(3)");
	sc_trace(tf, memory.memtarget[3], "mem_TARGET(3)");
	sc_trace(tf, memory.memsource[4], "mem_SOURCE(4)");
	sc_trace(tf, memory.memtarget[4], "mem_TARGET(4)");
	sc_trace(tf, memory.memsource[5], "mem_SOURCE(5)");
	sc_trace(tf, memory.memtarget[5], "mem_TARGET(5)");
	sc_trace(tf, memory.memsource[6], "mem_SOURCE(6)");
	sc_trace(tf, memory.memtarget[6], "mem_TARGET(6)");
	sc_trace(tf, memory.memsource[7], "mem_SOURCE(7)");
	sc_trace(tf, memory.memtarget[7], "mem_TARGET(7)");
	//simulate
	reset.write(0);
	sc_start(20,SC_NS);
	#ifdef DEBUGMODE
	cout << "data in Memory :\n";
	for(int i = 0; i < 8; i++)
	{
		cout << std::dec << "source[" << i << "]: " << std::hex << memory.memsource[i] << "	   ";
		cout << std::dec << "target[" << i << "]: " << std::hex << memory.memtarget[i] << endl;
	}
	#endif
	reset.write(1);
	sc_start(200,SC_NS);
	#ifdef DEBUGMODE
	cout << "data in Memory (After DMA):\n";
	for(int i = 0; i < 8; i++)
	{
		cout << std::dec << "source[" << i << "]: " << std::hex << memory.memsource[i] << "	   ";
		cout << std::dec << "target[" << i << "]: " << std::hex << memory.memtarget[i] << endl;
	}
	#endif
	
	return(0);
}

