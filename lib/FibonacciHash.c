#include "FibonacciHash.h"

uint64_t fibonacci_hash(uint8_t* buf, uint8_t byte_len){
    uint64_t hash_output = 0;
    for(uint8_t byte_cnt = 0; byte_cnt < byte_len; byte_cnt++){
        hash_output += buf[byte_cnt];
        hash_output *= 11400714819323198485llu;
    }
    return hash_output;
}
uint64_t fibonacci_doublehash(uint64_t prev_hash){
    prev_hash ^= prev_hash >> 32;
    return prev_hash * 2654435769ULL;
}

