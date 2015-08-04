# stm32_test2

Receipt items compact storage and service routines + some tests

Discription:

Data structure and service routines for compact storage receipts
containing unknown number of items of goods, each of that has title
16 to 160 chars long. Instead of using dynamic allocation,
statically allocated heap buffer is used. Custom routines for memory
allocation and freeing are provided. All memory bank (heap buffer)
is divided to separate memory blocks 16 bytes size. The key feature is
that only whole block can be allocated and freed. The benefits is
less memory fragmentation and easy heap overflow management.


* MCU: stm32f100rb (stm32vldiscovery board)
* IDE: CooCox CoIDE
* Author: Pavel Cherstvov
