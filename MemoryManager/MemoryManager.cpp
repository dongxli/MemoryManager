//
//  MemoryManager.cpp
//  MemoryManager
//
//  Created by Dongxuan Li on 12/1/17.
//  Copyright © 2017 David. All rights reserved.
//

#include "MemoryManager.hpp"

#include <iomanip>
#include <iostream>
using namespace std;

namespace MemoryManager {
    const int MM_POOL_SIZE = 65536;
    char MM_pool[MM_POOL_SIZE];
    
    void memView(int start, int end)
    {
        const unsigned int SHIFT = 8;
        const unsigned int MASK = 1 <<SHIFT - 1;
        
        unsigned int value; //used to facilitate bit shifting and masking
        cout<<"         Pool                     Unsigned   Unsigned "<<endl;
        cout<<"Mem ADD  indx   bits   chr  ASCII#  short      int    "<<endl;
        cout<<"--- ---  ----  ------  ---  ------- ------  ----------"<<endl;
        
        for(int i = start; i<= end; i++)
        {
            cout<< (long*)(MM_pool +1)<< ':'; // the actuall address in hexadecimal
            cout<< '[' <<setw(2) << i << ']'; // the index into MM_pool
            
            value = MM_pool[i];
            cout<<" ";
            for(int j=1; j<= SHIFT; j++) // the bit sequence for this byte (8 bits)
            {
                cout<< ((value & MASK)? '1':'0');
                value <<=1;
            }
            cout<<" ";
            cout<<'|'<< *(char *)(MM_pool+i)<< "|("; // the ASCII character of the 8 bits (1 byte)
            cout<< setw(4)<<((int)(*((unsigned char*)(MM_pool +i)))) << ")"; // the ASCII number of the character
            cout<< "("<<setw(5)<<(*(unsigned short*)(MM_pool +i)) << ")"; // the unsigned short value of 16 bits (2 bytes)
            cout<< "(" <<setw(10)<<(*(unsigned int*)(MM_pool+i))<<")"; // the unsigned int value of 32 bits (4 bytes)
            
            cout<<endl;
        }
    }

    //initialize set up any data needed to manage the memory pool
    void initializeMemoryManager(void)
    {
        *(unsigned short*)(MM_pool)=6; //next
        *(unsigned short*)(MM_pool+2)=0; //inUse list
        *(unsigned short*)(MM_pool+4)=0; //used list

    }

    //return a pointer inside the memory pool
    //if no chunk can accommodate aSize call onOutOfMemory()
    void* allocate (int aSize)
    {
        if(freeMemory()<aSize){
            onOutOfMemory();
        }
        unsigned short avaMem = *(unsigned short*)MM_pool;
        unsigned short iU = *(unsigned short*)(MM_pool+2);

        *(unsigned short*) (MM_pool+avaMem)= aSize; //store size
        *(unsigned short*) (MM_pool+avaMem+2)= iU; //store next
        *(unsigned short*) (MM_pool+avaMem+4)=0; //store prev

        if(avaMem!=6)
        *(unsigned short*) (MM_pool+iU+4)=avaMem;

        *(unsigned short*)MM_pool = avaMem+6+aSize;
        *(unsigned short*)(MM_pool+2) = avaMem;

        return (void*)(MM_pool+avaMem+6);
    }

    //free up a chunk previously allocated memory
    void deallocate(void* aPointer)
    {
        unsigned short prev = *(unsigned short*)(((char*)aPointer)-2); //prev
        unsigned short next = *(unsigned short*)(((char*)aPointer)-4);   //next
        unsigned short index = *(unsigned short*)(MM_pool+prev+2); //index

        *(unsigned short*)(MM_pool+prev+2) = next; // prev's next = aPointer's next
        *(unsigned short*)(MM_pool+next+4)= prev; // next's prev = aPointer's prev


        unsigned short used = *(unsigned short*)(MM_pool+4);
        *(unsigned short*)(MM_pool+index+2)= used; //next
        *(unsigned short*)(MM_pool+index+4)=0; //prev

        if(used!=0){
            *(unsigned short*)(MM_pool+used+4)=index;
        }

        *(unsigned short*)(MM_pool+4)= index;
    }

//    //scan the memory pool and return the total free space remaining in bytes
    int freeMemory(void)
    {
        return MM_POOL_SIZE-*(unsigned short*)MM_pool;
    }

    //Scan the memory pool and return the total deallocated memory in bytes
    int usedMemory(void)
    {
        int total=0;
        int next=0;
        int used = *(unsigned short*)(MM_pool+4);
        if(used!=0)
            next=used;
        while(next!=0){
            total += *(unsigned short*)(MM_pool+next)+6;
            next= *(unsigned short*)(MM_pool+next+2);
        }
        return total;
    }

    //Scan the memory pool and return the total in use memory
    int inUseMemory(void)
    {
        int total=0;
        int next=0;
        int temp=0;
        int iU = *(unsigned short*)(MM_pool+2);
        if(iU!=0)
            next=iU;
        while(next!=0){
            total += (*(unsigned short*)(MM_pool+next))+6;
            next= *(unsigned short*)(MM_pool+next+2);
        }
        return total;
    }

    //return the size (in bytes) associated with this memory address
    int size(void* aPointer)
    {
        return *(unsigned short*)((char *)aPointer-6);
    }

    //This is called when no more memory is available to allocate
    void onOutOfMemory(void)
    {
        std:: cerr << "/nMemory pool out of memory"<<std::endl;
        cin.get();
        exit(1);
    }

    
}
