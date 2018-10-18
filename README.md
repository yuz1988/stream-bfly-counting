# Counting Butterflies from a Large Bipartite Graph Stream.

We consider the estimation of properties on massive bipartite graph streams, where each edge represents a connection between entities in two different partitions. We present sub-linear-space one-pass algorithms for accurately estimating the number of butterflies in the graph stream. Our estimates have provable guarantees on their quality, and experiments show promising tradeoffs between space and accuracy. We also present extensions to sliding windows. While there are many works on counting subgraphs within unipartite graph streams, our work seems to be one of the few to effectively handle bipartite graph streams.

Use cmake to generate the exe file. The exe file is named __"stream_bfly"__.

You can run 5 different algorithms.

### Exact Algorithm (Incrementally updating the number of butterflies)
./stream_bfly exact 

### Batch Algorithm (Counting the number of butterflies using batch processing)
./stream_bfly batch


### Algorithm Res 
./stream_bfly Res 

### Algorithm IRes 
./stream_bfly IRes 

### Algorithm Ada 
./stream_bfly Ada 

Insert the input path, and other settings related to different algorithms. Finally, you will be able to see the results in the output path, you specified. As an example, see the output of graph "in.github".
