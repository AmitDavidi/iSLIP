#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// packet is defined by time, port int port out
typedef struct Packet {
    int time;
    int arrivalPort;
    int destinationPort;
    struct Packet *nextPacket;
} Packet;

typedef struct queue {
    Packet* first_packet;
    Packet* last_packet;
    int size;

} Queue;


bool isEmpty(Queue *Q) { return Q->size == 0; }

/* Insert a new packet into the queue Q */
void Enqueue(Queue *Q, Packet* pkt)
{
    if (isEmpty(Q)) {
        Q->first_packet = pkt;
        Q->last_packet = pkt;
        Q->size = 1;
    }

    else {
        Q->last_packet->nextPacket = pkt;
        Q->last_packet = pkt;
        Q->size++;
    }
}

/* Pop a packet from the queue Q*/
Packet* Dequeue(Queue* Q)
{

    if (isEmpty(Q) == true) {
        return NULL;
    }

    else {
        Packet* first_pkt = Q->first_packet;

        Q->first_packet = Q->first_packet->nextPacket;
        Q->size--;

        return first_pkt;
    }
}


void islip(Queue** Queues, int N, int k, int r, int time_step, int * acceptRRptrs, int * grantRRptrs) {
    
    /* Allocate memory for arrays used by the algorithm */
    int **request = (int**)malloc(N * sizeof(int*)); //Array to maintain requests from input-port
    int* grant = malloc(N * sizeof(int));  //Array to mark Grants - grant[idx1] = idx2 marks that idx1 granted idx2
    int* accept = malloc(N * sizeof(int)); //Array to mark Accepts - same as ^^
    bool* is_output_port_matched = calloc(N, sizeof(bool)); //Array to mark whether the given output-port matched
    bool* is_input_port_matched = calloc(N, sizeof(bool)); //Array to mark whether given input port has matched

    /* initialize the arrays to the default value */
    for(int i = 0; i < N; i++) {
        request[i] = calloc(N, sizeof(int));
        grant[i] = -1;
        accept[i] = -1;
    }

    /* --- <k> iSLIP iterations ---*/
    for(int islipIter = 0; islipIter < k; islipIter++) {

        // ---- Handle Request Stage ----
        for (int input_port_index = 0; input_port_index < N; input_port_index++) {
        
            for (int output_port_index = 0; output_port_index < N; output_port_index++) {
                // check if packets from <input_port_index> to <output_port_index> are available
            
                // If there's a packet available, and the ports are free
                if (isEmpty(Queues[input_port_index * N + output_port_index]) == false && is_input_port_matched[input_port_index] == false && is_output_port_matched[output_port_index] == false  ) {
                    request[input_port_index][output_port_index] = 1; // mark as request sent from input to output
                }
            }
        }

        // ---- Handle Grant Stage ----
        for (int output_port_index = 0; output_port_index < N; output_port_index++) {

            // if #<output_port_index> output-port hasn't established connection:
            if (is_output_port_matched[output_port_index] == false) {
            
                int input_port_idx = grantRRptrs[output_port_index];
                int num_of_ports_checked = 0;
            
                // check for the first input port available for a match, starting at the RR index
                while (num_of_ports_checked < N && request[input_port_idx][output_port_index] != 1) {
                    input_port_idx = (input_port_idx + 1) % N;
                    num_of_ports_checked++;
                }
            
                // possible match found
                if (request[input_port_idx][output_port_index] == 1)
                    grant[output_port_index] = input_port_idx; // grant to <input_port_inx># port
    
            }
        }

        // ---- Handle Accept Stage ----
        for (int input_port_index = 0; input_port_index < N; input_port_index++) {
        
            // Look for the first free output port that is free
            if(is_input_port_matched[input_port_index] == false) { // skip already matched input ports
            
                int output_port_idx = acceptRRptrs[input_port_index];
                int num_of_ports_checked = 0;

                // check for the first output port available for a match -- one that has granted <input_idx>, starting at the RR index
                while (num_of_ports_checked < N &&  grant[output_port_idx] != input_port_index) {
                    output_port_idx = (output_port_idx + 1) % N;
                    num_of_ports_checked++;
                }
            
                // If found an output port that has granted input-port - Hurray ! Match found
                if (grant[output_port_idx] == input_port_index) {
                    // mark input, output ports as matched
                    is_input_port_matched[input_port_index] = true;
                    is_output_port_matched[output_port_idx] = true;
                    
                    // Increment round robin index

                    acceptRRptrs[input_port_index] = (output_port_idx + 1) % N; 
                    grantRRptrs[output_port_idx] = (input_port_index + 1) % N; 

                    // send packet
                    Packet *packet_to_send = Dequeue(Queues[input_port_index*N + output_port_idx ]);
                    printf("%d %d %d %d\n", packet_to_send->time, packet_to_send->arrivalPort, packet_to_send->destinationPort, time_step);

                }

            }

        }
    }

    /* Free allocated memory */
    free(grant);
    free(accept);
    free(is_output_port_matched);
    free(is_input_port_matched);
    for (int i = 0; i < N; i++) {
        free(request[i]);
    }
    free(request);
}



int main(int argc, char** argv) {

    // -- initialization --
    if (argc < 4) {
        fprintf(stderr, "Usage: %s N k r\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]);      // num of ports
    int k = atoi(argv[2]);      // # iterations
    int r = atoi(argv[3]);      // run ID

    char filename[30] = {0};
    sprintf(filename, "%d.log.txt", r);
    FILE *logfile = fopen(filename, "w");


    int* accept_RoundRobin_Pointers = calloc(N, sizeof(int)); // an Array to keep track of Round-Robin Pointers for Accept
    int* grant_RoundRobin_Pointers = calloc(N, sizeof(int)); // an Array to keep track of Round-Robin Pointers for Grant

   // Allocate memory for the N*N array of Queue structures
    Queue** queues = (Queue**)malloc(N * N * sizeof(Queue*));
    for (int i = 0; i < N*N; i++) {
        queues[i] = (Queue*)malloc(sizeof(Queue));
        queues[i]->first_packet = NULL;
        queues[i]->last_packet = NULL;
        queues[i]->size = 0;
    }



    // Read packets from stdin and place them in input queues
    char line[100];

    int current_time = 1;
    bool packets_are_waiting = 0;

    while (fgets(line, sizeof(line), stdin) != NULL ) {
        
        Packet *packet = malloc(sizeof(Packet)); // make new Packet in heap and populate the fields
        sscanf(line, "%d %d %d\n", &packet->time, &packet->arrivalPort, &packet->destinationPort);
        
        
        if (packet->time != current_time) { 
            
            // time step is done, schedule ~!
            islip(queues, N, k, r, current_time, accept_RoundRobin_Pointers, grant_RoundRobin_Pointers);
            
            // -- log buffers' state --
            for (int bufferIdx = 0; bufferIdx < N * N; bufferIdx++) {
                fprintf(logfile, "%d %d %d %d\n", current_time, (bufferIdx/N)+1 , (bufferIdx%N)+1, queues[bufferIdx]->size);
                
                packets_are_waiting = 0;
                packets_are_waiting |= queues[bufferIdx]->size; // if packets are waiting in previous buffer or this buffer, mark it
            }

            current_time++;

        }
        // enqueue packet in the corresponding queue 'Row' Idx = Arrival Port, 'Column' = Destination Port
        Enqueue(queues[(packet->arrivalPort - 1) * N + packet->destinationPort - 1], packet);

    }

    // send the remaining packets in the queue
    while(packets_are_waiting) {
        islip(queues, N, k, r, current_time, accept_RoundRobin_Pointers, grant_RoundRobin_Pointers);
        packets_are_waiting = 0;

        for (int bufferIdx = 0; bufferIdx < N * N; bufferIdx++) {
            fprintf(logfile, "%d %d %d %d\n", current_time, (bufferIdx / N) + 1, (bufferIdx % N) + 1, queues[bufferIdx]->size);
            packets_are_waiting = packets_are_waiting || (queues[bufferIdx]->size > 0);
        }

        current_time++;
    }



    free(accept_RoundRobin_Pointers);
    free(grant_RoundRobin_Pointers);
    fclose(logfile);
    return 0;
}