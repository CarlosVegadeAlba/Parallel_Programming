
// Parallel Breadth First Search
// -----------------------------
// Berforms a BFS starting from vertex 1
// The parent of each vertex in the BFS tree along with its distance from the starting
// vertex is computed.
//
// The algorithm should first perform some rounds of sequential BFS before starting a parallel
// execution. In the parallel part each thread should be allocated a part of the vertices from the
// last round of the sequential algorithm. Any discovered vertices in the parallel part should 
// remain with the thread that discovered them. This continues until the entire graph has been
// explored.
//
//
// Parameters:
// n     : number of vertices
// ver   : ver[i] points to the start of the neighbor list of vertex i in edges
// edges : lists of neighbors of each vertex, each edge is listed in both direction
// p     : array of length n used for parent pointers
// dist  : array of length n used for distance from starting vertex
// S     : array of length n used for maintaining queue of vertices to be processed, only used in the 
//         sequential part. 
// T     : array of length n where n >> number of threads. 
//
// Note that the vertices are numbered from 1 to n (inclusive). Thus there is
// no vertex 0.


void abfs(int n,int *ver,int *edges,int *p,int *dist,int *S,int *T) {

    int i,j,k;         // Loop indices
    int v,w;          // Pointers to vertices
    
    int *temp;        // Temporary pointer
    int sequentialRounds=100; // Rounds before parallel
    int rounds_k=2;         // Rounds before copying to S  

    int *local_current_T;        // Vertexs of this round
    int current_counter=0;       // Counter of vertexs of this round

    int *local_new_T;            // Vertexs of next round
    int newVertexs_counter=0;    // Counter of vertexs of next round
     
    local_current_T = (int *) malloc(sizeof(int)*(n+2));
    local_new_T = (int *) malloc(sizeof(int)*(n+2));

    if(!local_current_T || !local_new_T){
        fprintf(stderr, "Memory allocation failed\n");
        free(local_current_T);
        free(local_new_T);
        return;
    }

    const int thread_id = omp_get_thread_num();
    const int num_threads = omp_get_num_threads();
    // The array has n_threads+2 values
    const int offset = 2;

    T[thread_id+offset] = 0;      // Number of vertices in T for this thread


    // FIRST SEQUENTIAL
    #pragma omp single
        {

    for(i=1;i<=n;i++) {     // Set that every node is unvisited
        p[i] = -1;          // Using -1 to mark that a vertex is unvisited
        dist[i] = -1;
    }

    p[1] = 1;        // Set the parent of starting vertex to itself
    dist[1] = 0;     // Set the distance from the starting vertex to itself
    T[0] = 1; // Num Vertexs of this round
    S[0] = 1; // Vertexs of this round

    current_counter=1;     // Amount of vertexs we are exploring at this time
    k=0;
    while(current_counter != 0 && k<sequentialRounds){ 
        newVertexs_counter=0;  // Save how many vertexs will be in the next round
        for(i=0; i<current_counter; i++){
            v = S[i];

            for(j=ver[v];j<ver[v+1];j++) { // Go through the neighbors of v
                w = edges[j];                // Get next neighbor w of v
                if (p[w] == -1) {            // Check if w is undiscovered
                    p[w] = v;                  // Set v as the parent of w
                    dist[w] = dist[v]+1;       // Set distance of w
                    local_current_T[newVertexs_counter]= w; // Add w to local_T and increase number of vertices discovered
                    newVertexs_counter++;
                }
            }
        }
        current_counter = newVertexs_counter;
        k++;
        // Update S
        temp = S;
        S = local_current_T; 
        local_current_T = temp;
        T[0] = newVertexs_counter;
    }
        }


    //NOW GO PARRALLEL
    T[thread_id+offset] = 0;
    #pragma omp barrier
    
    while(T[0]!= 0){ 
        T[offset+thread_id]=0; // NumVertex each thread has discovered
        current_counter=0;     // Amount of vertexs we are exploring at this time
        newVertexs_counter=0;  // Save how many vertexs will be in the next round

        #pragma omp for
        for(i=0; i<T[0]; i++){
            local_current_T[newVertexs_counter] = S[i]; // first store localy the partition of the vertexs
            newVertexs_counter++;
        }

        // Then make each thread in parallel process each of is own vertexs
        for(i=0; i<rounds_k; i++){
            current_counter = newVertexs_counter;  // Change the new vertexs to current ones
            newVertexs_counter=0;                  // Reset the new ones to 0
            for(k=0; k<current_counter; k++){  // Each round
                v = local_current_T[k];
                for(j=ver[v];j<ver[v+1];j++) {     // Go through the neighbors of v
                    w = edges[j];                  // Get next neighbor w of v
                    if (p[w] == -1) {              // Check if w is undiscovered
                        p[w] = v;                  // Set v as the parent of w
                        dist[w] = dist[v]+1;       // Set distance of w
                        local_new_T[newVertexs_counter]= w; // Add w to local_T d
                        newVertexs_counter++;      // Increase the num of vertexs discovered
                    }
                }
            }
            // Update the local_current_T
            temp = local_current_T;
            local_current_T = local_new_T;
            local_new_T = temp;
            #pragma omp barrier
        }

        // Now has been already k iterations we make the threads put the info together
        T[offset+thread_id] = newVertexs_counter;

        #pragma omp barrier

        // Make one thread prepare where each thread should write in the S array
        #pragma omp single
        {
            int total=0;
            for(i=0; i<num_threads; i++){
                total += T[offset+i];
            }
            T[0] = total; //Set the amount of vertexs for next round 

            //Prepare the index where each vertex should write in S
            int acumulator = 0; // Get the first amount of the frist thread
            int aux;

            for(i=0; i<num_threads; i++){
                aux = T[offset+i];
                T[offset+i] = acumulator;
                acumulator += aux;
            }
            //fflush(stdout);
        }
        #pragma omp barrier

        // Get where each thread should start writting
        int start=T[offset+thread_id];

        // Copy to the global array
        for(i=0; i<newVertexs_counter; i++){
            S[start+i]= local_current_T[i];
        }

        #pragma omp barrier
    }
    
    // Free variables
    free(local_current_T);
    free(local_new_T);
}


