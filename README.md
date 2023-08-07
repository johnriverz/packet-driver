# PThread-based Packet Driver

This repository contains the implementation of a basic packet driver. The project involves developing a small but complex software system using PThreads to provide concurrency in C that sends and receives packets from the network. 

It aims to demonstrate the ability to create a packet driver that functions as a multiplexer of packets from different applications onto a single network device (receive) and as a demultiplexer that delivers packets to the appropriate waiting callers (send).

## Table of Contents

- [Description](#description)
- [File Structure](#file-structure)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [Authors](#authors)
- [Acknowledgments](#acknowledgments)

## Description

The project consists of three main components:

- `packetdescriptor.h`: Defines the packet descriptor data structure and manipulation functions.
- `destination.h`: Defines the destination data structure for routing packets to specific processes.
- `pid.h`: Defines the process ID type used for distinguishing between applications.
- `freepacketdescriptorstore.h`: Defines the Free Packet Descriptor Store data structure and associated functions.
- `networkdevice.h`: Provides an interface for the network device used by the packet driver.
- `BoundedBuffer.h`: Defines a generic bounded buffer data structure with thread-safe operations.
- `fakeapplications.h`: Contains functions for simulating fake applications used for testing the packet driver.
- `packetdriver.h`: Provides the main API for the packet driver, including functions for sending and receiving packets.

## File Structure

The repository contains the following files:

- `packetdescriptor.h`: Defines the packet descriptor data structure and manipulation functions.
- `destination.h`: Defines the destination data structure for routing packets to specific processes.
- `pid.h`: Defines the process ID type used for distinguishing between applications.
- `freepacketdescriptorstore.h`: Defines the Free Packet Descriptor Store data structure and associated functions.
- `networkdevice.h`: Provides an interface for the network device used by the packet driver.
- `BoundedBuffer.h`: Defines a generic bounded buffer data structure with thread-safe operations.
- `fakeapplications.h`: Contains functions for simulating fake applications used for testing the packet driver.
- `packetdriver.h`: Provides the main API for the packet driver, including functions for sending and receiving packets.
- `packetdriver.c`: Contains the implementation of the packet driver functions.
- `freepacketdescriptorstore_full.h`: Extends the `freepacketdescriptorstore.h` with additional functions for creating and destroying the store.
- `networkdevice_full.h`: Extends the `networkdevice.h` with additional functions for creating and initializing the network device.
- `packetdescriptorcreator.h`: Provides functions for creating and initializing packet descriptors.
- `diagnostics.h`: Defines macros and functions for enabling or disabling diagnostic messages.
- `mydemo.c`: Contains the main function and entry point for the packet driver test demo.
- `testharness.o`: Implements the test harness for the packet driver exercise.

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
- Juan Rios (Implementation of the packet driver functions)

## Acknowledgments
The project files have been revised and developed by Dr. Joe Sventek as an assigmnet for his Operating Systems class at the University of Oregon. The implementation of the packetdriver.c functions, along with the README.md file, are written by Juan Rios Jr, a Computer Science student at UO.
