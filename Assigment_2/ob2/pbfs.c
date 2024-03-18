
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
    int num_r,num_w;  // Number of vertices in S and T, respectively
    int *temp;        // Temporary pointer
    int deep;
    deep=0;

    int local_counter=0;
    int *local_T;

    local_T = (int*) malloc(n * sizeof(int));
    if(!local_T){
        printf("Error allocating\n");
        return;
    }

    const int thread_id = omp_get_thread_num();
    const int num_threads = omp_get_num_threads();
    // The array has n_threads+2 values
    const int offset = 2;

    T[thread_id+offset] = 0;      // Number of vertices in T for this thread

    #pragma omp single
        {
    T[0] = 1; // Num Vertexs of this round
    T[1] = 0; // Num Vertexs of the next round
    S[0] = 1; // Vertexs of this round

    for(i=1;i<=n;i++) {   // Set that every node is unvisited
        p[i] = -1;          // Using -1 to mark that a vertex is unvisited
        dist[i] = -1;
    }

    p[1] = 1;        // Set the parent of starting vertex to itself
    dist[1] = 0;     // Set the distance from the starting vertex to itself


    //printf("Master, thread: %d set everything :)\n", omp_get_thread_num());
    //printf("Number of threads: %d\n", num_threads);
        }
    
    //printf("Hi from thread %d\n", thread_id);
    T[thread_id+offset] = 0;
    #pragma omp barrier

    /* #pragma omp single
    {
        for(i=0; i< num_threads; i++){
            printf("i=%d, T[offset+i]= %d\n", i, T[offset+i]);
        }
    }
 */
    
    while(T[0]!= 0){ 
        T[offset+thread_id]=0; // NumVertex each threa has discovered
        local_counter=0;
        int flag=1;
        deep++;
        #pragma omp for
        for(i=0; i<T[0]; i++){
            /* if(flag==1){
                printf("  Thread %d has i=%d\n", thread_id, i);
                flag=0;
            } */
            
            v = S[i];
            for(j=ver[v];j<ver[v+1];j++) { // Go through the neighbors of v
                w = edges[j];                // Get next neighbor w of v
                if (p[w] == -1) {            // Check if w is undiscovered
                    p[w] = v;                  // Set v as the parent of w
                    dist[w] = dist[v]+1;       // Set distance of w
                    local_T[local_counter]= w; // Add w to local_T and increase number of vertices discovered
                    local_counter++;
                    T[offset+thread_id] = local_counter;
                    //printf("    Thread %d has discovered vertex i=%d\n", thread_id, w);
                }
            }
        }
        #pragma omp barrier
        #pragma omp single
        {
            int total=0;
            for(i=0; i<num_threads; i++){
                total += T[offset+i];
            }
            //printf("Round %d has been %d vertexs discovered in total\n", deep, total);
            T[0] = total; //Set the amount of vertexs for next round 

            //Prepare the index where each vertex should write in S
            int acumulator = 0; // Get the first amount of the frist thread
            int aux;

            for(i=0; i<num_threads; i++){
                aux = T[offset+i];
                T[offset+i] = acumulator;
                acumulator += aux;
            }
        }
        #pragma omp barrier

        // Get where each thread should start writting
        int start=T[offset+thread_id];

        // Copy to the global array
        for(i=0; i<local_counter; i++){
            S[start+i]= local_T[i];
        }

        #pragma omp barrier
    }

}
