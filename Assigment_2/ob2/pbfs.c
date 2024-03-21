
// Parallel Breadth First Search
// -----------------------------
// Performs a BFS starting from vertex 1
// The parent of each vertex in the BFS tree along with its distance from the starting
// vertex is computed.
//
// The algorithm should gather all discovered vertices from round i, so that they can be 
// distributed among the threads before the search in round i+1.
//
// Parameters:
// n     : number of vertices
// ver   : array of length n. ver[i] points to the start of the neighbor list of vertex i in edges
// edges : array containing lists of neighbors for each vertex, each edge is listed in both direction
// p     : array of length n used for parent pointers
// dist  : array of length n used for distance from starting vertex
// S     : array of length n used for maintaining queue of vertices to be processed 
// T     : array of length n where n >> number of threads. 
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.

void pbfs(int n,int *ver,int *edges,int *p,int *dist,int *S,int *T) {

    int i,j;          // Loop indices
    int v,w;          // Pointers to vertices

    int *local_T;        // Local array to store the new vertices locally
    int local_counter=0; // Counter of where to write

    local_T = (int *) malloc(sizeof(int)*(n+2)); // Locate memory
    if(!local_T){
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    const int thread_id = omp_get_thread_num();     // id of each thread
    const int num_threads = omp_get_num_threads();  // Total number of threads
    // The array has n_threads+2 values
    const int offset = 2; // First to index positions would be used for other purposes                 

    #pragma omp single
        {
    T[0] = 1; // Num vertices of this round
    S[0] = 1; // vertices of this round

    for(i=1;i<=n;i++) {   // Set that every node is unvisited
        p[i] = -1;          // Using -1 to mark that a vertex is unvisited
        dist[i] = -1;
    }

    p[1] = 1;        // Set the parent of starting vertex to itself
    dist[1] = 0;     // Set the distance from the starting vertex to itself
        }
    
    #pragma omp barrier  // Wait before everything is ready
    
    // Start the graph search
    while(T[0]!= 0){  
        T[offset+thread_id]=0; // Number of discovered vertices of this thread for this round
        local_counter=0;       // Also store them locally

        //Start the searching in parallel
        #pragma omp for
        for(i=0; i<T[0]; i++){
            v = S[i]; // The value of i is unique for each thread
            for(j=ver[v]; j<ver[v+1]; j++) {  // Go through the neighbors of v
                w = edges[j];                 // Get next neighbor w of v
                if (p[w] == -1) {             // Check if w is undiscovered
                    p[w] = v;                  // Set v as the parent of w
                    dist[w] = dist[v]+1;       // Set distance of w
                    local_T[local_counter]= w; // Add w to local_T and increase number of vertices discovered
                    local_counter++;           // Increase the counter, in the shared value and the local value
                    T[offset+thread_id]++;
                }
            }
        }
        #pragma omp barrier  // Wait for all threads to finish this round before moving to the next one

        #pragma omp single   // Only one thread prepares the copy of S
        {
            int total=0;     // Total amount of vertices discovered by all the threads
            for(i=0; i<num_threads; i++){
                total += T[offset+i];
            }
            T[0] = total; //Set the amount of new vertices for next round 

            // Prepare the index where each vertex should write in S
            // For example thread_0 writes at 0, thread_1 writes where thread_0 finishes...
            int acumulator = 0; // Get the first amount of the frist thread
            int aux;

            for(i=0; i<num_threads; i++){
                aux = T[offset+i];
                T[offset+i] = acumulator;
                acumulator += aux;
            }
        }
        #pragma omp barrier // Before making parallel copy of the array wait until is ready

        // Get where each thread should start writing
        int start=T[offset+thread_id];

        // Copy to the global array in parallel
        for(i=0; i<local_counter; i++){
            S[start+i]= local_T[i];
        }

        #pragma omp barrier // Wait until every thread has finished copying to S before moving to the next round
    }

    free(local_T); // Free the allocated memory
}
