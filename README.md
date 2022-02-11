# ESL Project2 - DMA_controller_in_TLM2_0 - vincent08tw
###### tags: `ESL` `SystemC`

## Outline

## Project Description
Develop a TLM 2.0 wrapped DMAC.

* Specification  
	* The DMA behavior is illustrated as:  
		![](./img/behavior_of_DMA.PNG)
```
1. The DMA has:
	i. One (1) 32-bit master port developed with TLM 2.0 blocking
		initiator port.
	ii. One (1) 32-bit slave port developed with TLM 2.0 blocking target
		port.
	iii. One (1) 1-bit Interrupt pin developed with sc_out.
	iv. One (1) 1-bit reset pin developed with sc_in.
	v. One static address register to store its base address.
2. There are also four (4) control registers in the DMA:
	i. SOURCE: 32-bit, the starting source address, at 0x0
	ii. TARGET: 32-bit, the starting target address, at 0x4
	iii. SIZE: 32-bit, the data size, at 0x8
	iv. START/CLEAR: 1-bit, the start and clear signal, at 0xC
3. Implement the DMA module with a SC_CTHREAD process and it:
	i. reads from the slave port the control data, i.e. source address,
		target address, size and start. These data are stored into control
		registers, respectively.
	ii. Once a
	iii. START/CLEAR = 1 value (0xC = 1) is received and stored,
		start moving data from the source address to the target address,
		until the SIZE is reached.
	iv. When the data movement is completed, first pull the interrupt
		signal (interrupt) to high and wait for the interrupt clear
		control (START/CLEAR, 0X0C = 0). Only after the CLEAR
		arrives then the DMA resets the interrupt to low and resume to
		the original state.
	v. The reset pin (reset) is synchronous and active low. At reset
		all 4 control registers are set to 0 and interrupt signal is pull
		to low.
```

## Project directory hierachy
```
DMA_controller_in_SystemC/
    +-- src/
    |   +-- CPU.cpp
    |   +-- CPU.h
    |   +--	DMA.cpp
    |   +--	DMA.h
    |   +--	main.cpp
    |   +--	makefile
    |   +--	Memory.cpp
    |   +--	Memory.h
    +-- img/
    |   +--	behavior_of_DMA.PNG
    |
    README.md
```























