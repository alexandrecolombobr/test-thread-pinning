## test-thread-pinning

In this sample program, we execute several dummy hash operations in a single thread pinned to a specific core.

The goal is to understand thread affinity and differences between P-cores and E-cores.

Performance cores and efficient cores are part of the BigLittle architecture develop by Intel since 11th Core generation.

My setup is a Intel Core i3 1215u with 8 threads (2+2 P-cores/Hyperthreading and 4 E-cores). Core ids 0 to 3 refer to P-cores, and core ids 4 to 7 refer to E-cores.

Elapsed times using this program in my setup are as follows:

* Elapsed time: 3.24408 seconds for thread 0
* Elapsed time: 3.29134 seconds for thread 1
* Elapsed time: 3.34289 seconds for thread 2
* Elapsed time: 3.31582 seconds for thread 3
* Elapsed time: 5.17637 seconds for thread 4
* Elapsed time: 5.18358 seconds for thread 5
* Elapsed time: 5.20615 seconds for thread 6
* Elapsed time: 5.17723 seconds for thread 7

This sample program is interesting of use if you may want to check core isolation benefits (check Tuned and Tuna).