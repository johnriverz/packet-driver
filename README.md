# PThread-based Packet Driver

This repository contains the implementation of a basic packet driver. It aims to demonstrate the ability to create a packet driver that functions as a multiplexer of packets from different applications onto a single network device and as a demultiplexer that delivers packets to the appropriate waiting callers.

In other words, the project involves developing a small but complex software system that sends and receives packets from the network. It makes use of pthreads, mutexes, and condition variables to provide concurrency in C.

## Table of Contents

- [Description](#description)
- [File Structure](#file-structure)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Authors](#authors)
- [Acknowledgments](#acknowledgments)

## Description

The Applications exchange packets with other Applications through a network. To facilitate this communication, a Network Device Driver has been provided for the selected network medium. However, the Applications and Network Device Driver operate at different speeds. Thus, the project seeks to develop a Packet Driver responsible for transmitting packets over the network and receiving incoming packets, without any packets being lost.

The project consists of three main component layers. These layers can be thought of as a simple architecture of the network:

- `Applications`: Sends packets to and receives packets from other applications on the network. These are provided in testharness.o, and each one is labeled with its own PID. To send packets, an application must allocate a PacketDescriptor with relevant data, which gets passed onto the packet driver.
- `Network Layer`: The Packet Driver and Packet Descriptor Store live here. This layer's purpose is to handle the speed mismatch between Applications and the Network Device Driver. 
- `Link Layer`: Where the Network Device Driver is located. It is full-duplex, meaning that data can be transmitted in both directions.

## File Structure

The repository contains the following files:

- `packetdescriptor.h`: Defines the packet descriptor data structure and manipulation functions.
- `destination.h`: Defines the destination data structure for routing packets to specific processes.
- `pid.h`: Defines the process ID type used for distinguishing between applications.
- `freepacketdescriptorstore.h`: Defines the Free Packet Descriptor Store data structure and associated functions.
- `networkdevice.h`: Provides an interface for the network device used by the packet driver.
- `BoundedBuffer.h`: Defines a generic bounded buffer data structure with thread-safe operations.
- `queue.h`: Defines a generic FIFO queue interface.
- `fakeapplications.h`: Contains functions for simulating fake applications used for testing the packet driver.
- `packetdriver.h`: Provides the main API for the packet driver, including functions for sending and receiving packets.
- `packetdriver.c`: Contains the implementation of the packet driver functions.
- `freepacketdescriptorstore_full.h`: Extends the `freepacketdescriptorstore.h` with additional functions for creating and destroying the store.
- `networkdevice_full.h`: Extends the `networkdevice.h` with additional functions for creating and initializing the network device.
- `packetdescriptorcreator.h`: Provides functions for creating and initializing packet descriptors.
- `diagnostics.h`: Defines macros and functions for enabling or disabling diagnostic messages.
- `testharness.o`: Implements the test harness for the packet driver exercise. It defines main().
- `libTH.a`: Contains object files required to build mydemo.
- `Makefile`: Builds mydemo from testharness.o, packetdriver.o, and libTH.a.

## Building the Project

To build the project, follow these steps:

1. Make sure you have a C compiler and pthread library installed.
2. Compile the project using the provided makefile:

~~~
make
~~~

This will generate an executable named mydemo.
To run the packet driver test demo, use the following command:

~~~
./mydemo
~~~

## Authors
- Joe Sventek
- Juan Rios Jr

## Acknowledgments
The project and its files have been revised and developed by Dr. Joe Sventek to serve as an assigmnet for his Operating Systems class at the University of Oregon. The implementation of the packetdriver.c functions, along with the README.md file, are written by Juan Rios Jr.