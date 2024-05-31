# Important Terms

* **Temporal locality and Spatial locality:**
  * Temporal locality: refers to the tendency of programs to reuse data elements that have recently been used. Thus recently used data elements are stored high up in the cache hierarchy.
  * Spatial locality: refers to the tendency of programs to use data elements that are stored close in memory to recently used elements. Thus the system loads not only an element that is to be used, but also elements that are stored close by (i.e. in the same cache line) and stores these high up in the cache hierarchy.
  * how one can exploit both of these concepts when writing sequential code for matrix multiplication?: In standard matrix multiplication, AxB, spatial locality is exploited by loading cache lines of A and B into cache. Unless B has been transposed this only helps the access to A. Temporal locality only has an effect if A (or the transposed B) is suf ciently small that an entire row can be stored in
    memory.
    To exploit both types of locality multiplication should be done by dividing A and B into smaller blocks. If the elements of each block is stored consecutively then two blocks of suf ciently small size n2 can be stored in the cache hierarchy and be used to perform n3 operations.

---

* **False sharing**
  * False sharing refers to when different threads are accessing elements on the same cache line. As the smallest unit of memory is a cache line this means that the same cache line will be moved between the caches of the different threads even though they are not writing to the same elements. This can be prevented by partitioning the data so that the cache lines containing data that a thread is writing to does not contain elements that other threads are writing to. A static partitioning will typically achieve this. It is also possible to pad an array with dummy elements to ensure that there is no overlap (measured in cache lines) between the working area of two threads.

---

* **Speedup**
  * Speedup is a measure of how much faster a parallel program is compared to a sequential one for solving the same problem. Speedup is a function of the number of threads used and also the size of the problem. It is calculated as S(n,p) = T(n)/Tp(n)
  * Measure: In a theoretical setting one would use the running time of the programs measured with O-notation (or a more exact expression). In a practical setting one would use the wall-clock time to calculate the speedup.

---

* **Latency and bandwidth in parallel computers with distributed memory**
  * Latency: When sending data from between two processes latency is the time from when the rst data item is sent until it starts arriving at its destination
  * Bandwidth: is the capacity of the communication channel used. This measures how much data (in bytes) can be sent per time unit once the communication channel is stablished

---

* **Schedules in pragma omp**
  * a
* **How can them affect speed?**

---

* **Reduction in pragma omp**
  * a

---

* **In practice it is possible to get higher speedup than p when using p processes?**
  * Yes, this can happen because we now get access to more memory and the sequential parts can
    execute faster.

---

* **How different languages stored rows/columns when loading a value**

  * C stores them in rows
  * a
  * a

---

* **Moore's law (Number of transistors)**
  * Moore´s law says that the number of transistors in a given area will double every 18th month

---

```

```

MPI

### Functions

### **MPI_Init**

### **MPI_Finalize**

### **MPI_Comm_rank**

### **MPI_Comm_size**

### MPI_Send

### MPI_Recv

### **MPI_Reduce**

### **MPI_Bcast**

### **MPI_Scatter**

### **MPI_Gather**

### **MPI_Barrier**

### Compile

mpicc -Wall -O2 -o prueba1 prueba1.c

### Execute

mpicc -o programa programa.c
