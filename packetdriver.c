/*
 * packetdriver.c
 * CS 415 Project 2
 *
 * The packet driver functions as a multiplexor of packets from
 * different applications onto the single network device and also
 * as a demultiplexor that takes the stream of packets from the device
 * and delivers them to the appropriate waiting callers.
 *
 * This is my own work, except for some of the comments/descriptions,
 * which were taken from the project handout.
 *
 * Author: Juan Rios Jr
 */

#include <pthread.h>
#include "BoundedBuffer.h"
#include "destination.h"
#include "diagnostics.h"
#include "fakeapplications.h"
#include "freepacketdescriptorstore__full.h"
#include "networkdevice__full.h"
#include "packetdriver.h"
#include "packetdescriptorcreator.h"
#include "packetdescriptor.h"
#include "pid.h"

#define UNUSED __attribute__((unused))
#define ATTEMPTS_TO_SEND 5

/* any global variables required for use by your threads and your driver routines */
FreePacketDescriptorStore *FPDS;
NetworkDevice *ND;

BoundedBuffer *inBB[MAX_PID + 1]; // for incoming packet descriptors (PDs)
BoundedBuffer *outBB;             // for outgoing PDs

pthread_t recv; // thread for receiving PDs
pthread_t send; // thread for sending PDs

/* definition[s] of function[s] required for your thread[s] */
void *sending(void *args);
void *receiving(void *args);

/* FUNCTION IMPLEMENTATIONS */
void init_packet_driver(NetworkDevice *nd, void *mem_start, unsigned long mem_length, FreePacketDescriptorStore **fpds)
{
    /* create Free Packet Descriptor Store */
    FPDS = FreePacketDescriptorStore_create(mem_start, mem_length);

    /* create buffers for incoming/outgoing PDs */
    if ((outBB = BoundedBuffer_create(MAX_PID)) == NULL)
    {
        DIAGNOSTICS("[DRIVER> Info: Failed to create sending buffer\n");
    }
    for (int i = 0; i < MAX_PID + 1; i++)
    {
        if ((inBB[i] = BoundedBuffer_create(MAX_PID + 1)) == NULL)
        {
            DIAGNOSTICS("[DRIVER> Info: Failed to create receiving buffer\n");
        }
    }

    /* creating 2 threads, one for sending PDs and one for receiving PDs */
    if (pthread_create(&send, NULL, sending, NULL) != 0)
    {
        DIAGNOSTICS("[DRIVER> Info: Failed to create sending thread\n");
    }

    if (pthread_create(&recv, NULL, receiving, NULL) != 0)
    {
        DIAGNOSTICS("[DRIVER> Info: Failed to create receiving thread\n");
    }

    /* return the FPDS to the code that called you */
    *fpds = FPDS;
    ND = nd;
}

void blocking_send_packet(PacketDescriptor *pd)
{
    /* queue up packet descriptor for sending */
    /* do not return until it has been successfully queued */
    outBB->blockingWrite(outBB, (void *)pd);
}

int nonblocking_send_packet(PacketDescriptor *pd)
{
    /* if you are able to queue up packet descriptor immediately, do so and return 1 */
    /* otherwise, return 0 */
    return outBB->nonblockingWrite(outBB, (void *)pd);
}

void blocking_get_packet(PacketDescriptor **pd, PID pid)
{
    /* wait until there is a packet for `pid’ */
    /* return that packet descriptor to the calling application */
    inBB[pid]->blockingRead(inBB[pid], (void **)pd);
}

int nonblocking_get_packet(PacketDescriptor **pd, PID pid) //
{
    /* if there is currently a waiting packet for `pid’, return that packet */
    /* to the calling application and return 1 for the value of the function */
    /* otherwise, return 0 for the value of the function */
    return inBB[pid]->nonblockingRead(inBB[pid], (void **)pd);
}

void *sending(UNUSED void *args)
{
    PacketDescriptor *PD; // outgoing data packet

    /* thread runs forever */
    while (1)
    {
        /* get a PD from outgoing buffer to send */
        outBB->blockingRead(outBB, (void **)&PD);

        /* send PD over network, stop if sendPacket keeps failing after some attempts */
        /* or after a packet was sent successfully */
        int tries = 1;
        for (int i = 0; i < ATTEMPTS_TO_SEND; i++)
        {
            if (ND->sendPacket(ND, PD) == 0)
            {
                DIAGNOSTICS("[DRIVER> Info: Failed to send a packet after %d tries\n", tries);
                tries++;
            }
            else
            {
                DIAGNOSTICS("[DRIVER> Info: Sent a packet after %d tries\n", tries);
                break;
            }
        }

        /* return PD to FPDS */
        FPDS->blockingPut(FPDS, PD);
    }
}

void *receiving(UNUSED void *args)
{
    PacketDescriptor *PD; // incoming data packet
    PID pid;              // pid of receiving application

    /* thread runs forever */
    while (1)
    {
        /* get a packet descriptor from FPDS and place it in PD */
        FPDS->blockingGet(FPDS, &PD);

        /* reset PD before registering with ND */
        initPD(PD);

        /* register PD and block thread until PD has data */
        ND->registerPD(ND, PD);
        ND->awaitIncomingPacket(ND);

        /* pass PD to the application specified by pid, located in incoming buffer */
        pid = getPID(PD);
        inBB[pid]->blockingWrite(inBB[pid], PD);
        DIAGNOSTICS("[DRIVER> Packet received for application %d\n", pid);

        /* at this point, it is the application's responsibility to return the */
        /* PD to the FPDS    */
    }
}