//
//  MemoryManager.hpp
//  MemoryManager
//
//  Created by Dongxuan Li on 12/1/17.
//  Copyright © 2017 David. All rights reserved.
//

#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include <stdio.h>
namespace MemoryManager{
    void memView(int start, int finish); //show the contents of memory from start to finish
    void initializeMemoryManager(void); //initialize any data needed to manage the memory pool
    void *allocate(int); //return a pointer inside the memory pool
    void deallocate(void *); //free up a chunk previously allocated
    int freeMemory(void); //scan memory pool, return the total free space remaining
    int usedMemory(void); //scan memory pool, return the total used memory that has been dealloated
    int inUseMemory(void); //scan the memory pool and return the total memory being currently used
    void onOutOfMemory(void); //call if no space is left for the allocation request
    int size(void *); //return the size (in bytes) of the variable that lives at this address
};

#endif /* MemoryManager_hpp */
