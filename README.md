
##ForkMergeSort


__Description__: 

    C implementation of Mergesort. Each process is forked (with the child doing the divide step) 
    and numbers are communicated via pipes. 

__Usage__: 

    >> 5 4 3 2 1 (limited input of 50 numbers)
    -> 1 2 3 4 5 (sorted output)
