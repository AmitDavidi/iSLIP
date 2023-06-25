# iSLIP Packet Scheduler

This code implements an iSLIP packet scheduler. The iSLIP algorithm is a widely used algorithm for scheduling packet transmissions in high-speed networks. The code takes input packets, schedules them using the iSLIP algorithm, and outputs the scheduled packets.

## Compilation

To compile the code, use a C compiler with the following command:

```
gcc -o islip_scheduler islip_scheduler.c
```

## Usage

The compiled program can be run with the following command:

```
./islip_scheduler <N> <k> <r> < input_packets.txt
```

- `<N>`: The number of ports in the network.
- `<k>`: The number of iterations the iSLIP algorithm will run.
- `<r>`: The run ID, used to generate a log file (`<r>.log.txt`) that contains the state of the buffers at each time step.
- `<input_packets.txt>`: A file containing the input packets, where each line represents a packet with the format `<time> <arrivalPort> <destinationPort>`. The packets should be sorted by their arrival time.

## Algorithm

The code implements the iSLIP algorithm as follows:

1. Initialize data structures and allocate memory.
2. Read packets from the input and enqueue them in the appropriate queues based on their arrival and destination ports.
3. For each time step:
   a. Execute the iSLIP algorithm for `k` iterations.
   b. Log the state of the buffers in the log file.
   c. Move to the next time step.
4. Send the remaining packets in the queues.
5. Free allocated memory and close the log file.

The iSLIP algorithm consists of three stages: Request, Grant, and Accept. The algorithm iterates `k` times, and in each iteration, it performs the following steps:

1. Request Stage: Each input port sends requests to the corresponding output ports for available packets.
2. Grant Stage: Each output port grants a request to one of the input ports based on a round-robin selection. If an output port hasn't established a connection, it selects an input port that has requested it.
3. Accept Stage: Each input port accepts a grant from one of the output ports based on a round-robin selection. If an input port hasn't established a connection, it selects an output port that has granted it. Once a match is found, the input and output ports are marked as matched, and the corresponding packet is sent.

## Data Structures

The code uses the following data structures:

- `Packet`: Represents a packet with a time, arrival port, destination port, and a pointer to the next packet.
- `Queue`: Represents a queue of packets, consisting of a pointer to the first and last packets and the size of the queue.

## Functions

The code includes several functions:

- `isEmpty(Queue *Q)`: Checks if a queue is empty.
- `Enqueue(Queue *Q, Packet *pkt)`: Inserts a new packet into the queue.
- `Dequeue(Queue *Q)`: Removes and returns the first packet from the queue.
- `islip(Queue **Queues, int N, int k, int r, int time_step, int *acceptRRptrs, int *grantRRptrs)`: Implements the iSLIP algorithm using the provided parameters and the queues.
- `main(int argc, char** argv)`: The main function that handles program initialization, reads input packets, executes the iSLIP algorithm, and writes the log file.

## Log File

The code generates a log file named `<r>.log.txt`, where `<r>` is the run ID provided as a command-line argument. The
