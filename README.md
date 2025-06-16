# MinMaxC-IGplus

code for the ICDE paper 

## Requirements

* GCC == 9.4.0
* CMAKE

  
## Code Structure

- `IGL/run/run_IGLPlus.cpp`: Main function of the algorithm IG+.
- `IGL/run/run_IGHeap.cpp`: Main function of the algorithm IG.
- `IGL/run/run_IG.cpp`: Main function of the algorithm IG-B.
- `IGL/run/runIGLPlus_insert.cpp`: Main function of the algorithm CCRT.init(G) CCRT-ins + CCRT.solve(G').
- `IGL/run/runIGLPlus_remove.cpp`: Main function of the algorithm CCRT.init(G) CCRT-rem + CCRT.solve(G').
- `IGL/src/gsum.cpp`: Specific implementation of each algorithm.

## Build and Run

Build and run Algorithm with the code below in the project folder.

``` shell
mkdir build
cd build
cmake ..
make -j
#if b = 10
./IGLPlus 10 data_dir output_dir
#for insertion algorithm
./IGLPlusIns 1000 org_data_dir delta_data_dir output_dir
#for removal algorithm,if |E^-| = 1000
./IGLPlusRem 1000 org_data_dir delta_data_dir output_dir
```

The process for running other algorithms is similar.


