#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


double rollUniform() {
    return (double)rand() / RAND_MAX;
}

int rollPortNum(int d_flag, int N, int arrival_port) {

    if (d_flag) {
        
        if (rollUniform() < (2.0 / 3.0)) {
            // in 2/3 probability - output = input port
            return arrival_port;
        }
        else {
            // in 1/3 probability, output port = 1 + arrival % N
            return ( 1 + arrival_port) % N;
        }
    }

    else {
        // uniform
        return rand() % N + 1;
    }

}

int main(int argc, char** argv) {

    // -- initialization --
    if (argc != 5 && argc != 6) {
        fprintf(stderr, "Usage: %s tr_gen N T seed p [-d]\n", argv[0]);
        exit(1);
    }

    int N = atoi(argv[1]);      // num of ports
    int T = atoi(argv[2]);      // time steps
    int seed = atoi(argv[3]);   // seed
    double p = atof(argv[4]);   // probability
    
    int d_flag = 0;
    if(argc == 6 && strcmp(argv[5], "-d") == 0)
       d_flag = 1;

    else if (argc == 6) {
        fprintf(stderr, "Flag not recognized: Usage: %s N T seed p [-d]\n", argv[0]);
        exit(1);
    }   
    
    srand(seed);


    // Generate and output packets
    for (int t = 1; t <= T; t++) {

        for (int port = 1; port <= N; port++) {
            // Check if a packet arrives at the current port

            if ( rollUniform() < p) {
                // Generate a random destination port
                int dest_port = rollPortNum(d_flag, N, port);

                // Output the packet information
                printf("%d %d %d\n", t, port, dest_port);
            }
        }

    }

	exit(0);
}