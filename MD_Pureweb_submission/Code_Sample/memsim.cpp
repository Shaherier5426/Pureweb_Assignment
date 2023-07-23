
//Author: MD Shaherier Khan
#include "memsim.h"
#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>
#include <unistd.h>




struct Node {
    int64_t address; // page id
    int64_t pid; // -1 free processor id
    bool free;
    int64_t size;
};




using LI = std::list<struct Node>::iterator;

bool cmp(const LI & a, const LI & b) {
    if (a->size == b->size) return a->address < b->address;
    else{
    return a->size > b->size;
    }
}
using SN = std::set<LI, decltype(&cmp)>;

std::list<struct Node> chunks;
std::unordered_map<int64_t, std::vector<LI>> pro_chunks;
SN free_chunks(&cmp);

    
int64_t page_count=0;
int64_t page_size=0;

void print_set(SN & set) {
    std::cout << "\nFree chunks\n";
    for (auto n : set) {
        std::cout << "Index " << n->address << " " << n->size << std::endl;
    }
}

void print_node(std::list<struct Node> chunks){
  std::cout << "\n chunks\n";
    for (auto n : chunks) {
        std::cout << "| Index " << n.address << " size " << n.size <<" pid " << n.pid<<" free " << n.free <<"|"<< std::endl;
    }


}

void print_map(std::unordered_map<int64_t, std::vector<LI>> pro_chunks){
  int64_t process=0;
     std::cout << "\n Map:\n";
     for (auto n : pro_chunks[process]) {
         std::cout << "Index " << n->address << " " << n->size << std::endl;
         process++;
     }


}



struct Simulator {
  
   // constructor
  Simulator(int64_t page_siz)
  { page_size=page_siz;}

// Pseudocode for allocation request:
    // - search through the list of partitions from start to end, and
    //   find the largest partition that fits requested size
    //     - in case of ties, pick the first partition found
    // - if no suitable partition found:
    //     - get minimum number of pages from OS, but consider the
    //       case when last partition is free
    //     - add the new memory at the end of partition list
    //     - the last partition will be the best partition
    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free

  void allocate(int64_t tag, int64_t size)
  {
    
    
    //int largest= largest_free->size;
    int64_t addr=0;
    LI largest_free;

    //Case 1: prog just started.
    if(chunks.empty() && free_chunks.empty()){
      int64_t pages_to_add_empty=0;
      int64_t new_page_size_empty=0;

      // determine the pages need 
      if( size % page_size==0){
        pages_to_add_empty = (size / page_size);

      }
      else{
        pages_to_add_empty = (size / page_size)+1;

      }
      // The total memory size based on the page so if my page_size is 50, the size i need is 100 thus I need 2 pages
      new_page_size_empty = pages_to_add_empty * page_size;
      chunks.push_back(Node{0, -1, true,new_page_size_empty });
      free_chunks.insert(std::prev(chunks.end()));
      largest_free= *free_chunks.begin();
      page_count+=pages_to_add_empty;
    }

    //Case 2 : free_chunks empty and chunks not empty.

      else if (free_chunks.empty() && !chunks.empty()){

        int64_t pages_to_add_case2=0;
        int64_t new_page_size_case2=0;

        if( size % page_size==0){
        pages_to_add_case2 = (size / page_size);

        }
        else{
          pages_to_add_case2 = (size / page_size)+1;

        }
        new_page_size_case2 = pages_to_add_case2 * page_size;

      
      
      
        LI last_element_case2 = std::prev(chunks.end());
        addr = last_element_case2->address + last_element_case2->size;
        chunks.push_back(Node{addr, -1, true,new_page_size_case2 });
        free_chunks.insert(std::prev(chunks.end()));
        largest_free= *free_chunks.begin();
        page_count+=pages_to_add_case2;

      }

    
    // Case 3: free chunks not empty and and chunks not empty.

    else if (!free_chunks.empty() && !chunks.empty()){

        largest_free= *free_chunks.begin();
        
        if (size > largest_free->size ){
          LI last_element = std::prev(chunks.end());
          int64_t pages_to_add=0;
          int64_t new_page_size=0;
          
          if (last_element->free == true){   

            int64_t total_size=last_element->size;
            int64_t requested_size= size - last_element->size;

            if( requested_size % page_size==0){
            pages_to_add = (requested_size / page_size);

            }
            else{
            pages_to_add = (requested_size / page_size)+1;

            }
            total_size += pages_to_add*page_size;
            free_chunks.erase(last_element);
            addr=last_element->address;
            last_element->size = total_size;
            free_chunks.insert(last_element);
            largest_free=*free_chunks.begin();
            page_count+=pages_to_add;

          }

          // last element is occupied 
          else{

              if( size % page_size==0){
                  pages_to_add = (size / page_size);

                }
              else{
                pages_to_add = (size / page_size)+1;

              }
            
            new_page_size = pages_to_add * page_size;                           
            addr=last_element->address+last_element->size;
            
            
            
            chunks.push_back(Node{addr, -1, true,new_page_size });    
            free_chunks.insert(std::prev(chunks.end()));
            largest_free=*free_chunks.begin();
            page_count+=pages_to_add;

          }


        }

        
        
    }

// get the largest unoccupied node from the set data structure
    int64_t old_size=largest_free->size;
    free_chunks.erase(largest_free);

    

    largest_free->pid=tag;
    largest_free->free=false;
    largest_free->size=size;

    pro_chunks[tag].push_back(largest_free);

    int64_t size_left = old_size - largest_free->size;
    int64_t address_left= largest_free->address + largest_free->size;
    if (size_left!=0){
      Node f = Node{address_left,-1,true,size_left};

    chunks.insert(std::next(largest_free),f);
    free_chunks.insert(std::next(largest_free));

    }

    







  }
  void deallocate(int64_t tag)
  {
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions

    if (pro_chunks.count(tag)==0){
    }
    else{


    for (auto n : pro_chunks[tag]) {
        n->pid = -1;
        n->free = true;

        // check if prev is free
        if (n != chunks.begin() && std::prev(n)->free==true) {
            // merge
            n->size+=std::prev(n)->size;
            n->address = std::prev(n)->address;
            free_chunks.erase(std::prev(n));
            chunks.erase(std::prev(n));
        }

        // check if next is free
        if (n != std::prev(chunks.end()) && std::next(n)->free==true) {
            // merge
            n->size+=std::next(n)->size;
            free_chunks.erase(std::next(n));
            chunks.erase(std::next(n));
        }

        free_chunks.insert(n);
    }
    pro_chunks[tag].clear();

    }


  }
  MemSimResult getStats()
  {
    // let's guess the result... :)
    MemSimResult result;
    
    if(free_chunks.size()==0){
      result.max_free_partition_size = 0;
      result.max_free_partition_address = 0;


    }
    else{
      LI max_free= *free_chunks.begin();

    
    result.max_free_partition_size = max_free->size;
    result.max_free_partition_address = max_free->address;
    
    }
    result.n_pages_requested = page_count ;
    return result;
  }
  
};

// re-implement the following function
// ===================================
// parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests
// return:
//    some statistics at the end of simulation
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{
  // if you decide to use the simulator class, you probably do not need to make
  // any changes to the code below, except to uncomment the call to check_consistency()
  // before submitting your code
  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
      print_set(free_chunks);
      print_node(chunks);
    } else {
      sim.allocate(req.tag, req.size);
      print_set(free_chunks);
      print_node(chunks);
    }
  }
  return sim.getStats();
}

int main(int argc, char** argv)
{
  std::vector<Request> requests; 
  requests.push_back(Request{1,10});
  requests.push_back(Request{2,1});
  requests.push_back(Request{1,20});
  requests.push_back(Request{1,30});
  requests.push_back(Request{1,9});
  requests.push_back(Request{2,1});
  requests.push_back(Request{-1,0});
  requests.push_back(Request{1,1});

  MemSimResult results = mem_sim(100, requests);
  

  return 0;
}
  
