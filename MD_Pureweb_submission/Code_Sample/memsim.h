// =============================================================================
//                        !!! DO NOT EDIT THIS FILE !!!
// =============================================================================


// Author: MD Shaherier Khan
#pragma once
#include <cstdint>
#include <vector>

struct Request {
  // negative tag indicates "deallocate request", otherwise "allocation request"
  int tag; 
  // size of the request (ignored for deallocate requests)
  int size;
};

struct MemSimResult {
  // total number of pages requested
  int64_t n_pages_requested;
  // size of the largest free partition at the end of simulation
  // if no free partitions exist, set this to "0"
  int64_t max_free_partition_size;
  // address of the largest free partition at the end of simulation
  // if not free partitions exist, set this to "0"
  // in case of ties for size, return the smallest address
  int64_t max_free_partition_address;
};

MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests);