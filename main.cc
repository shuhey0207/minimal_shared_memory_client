#include <bits/stdc++.h>
#include "shared_memory.h"
using namespace std;

int main(int argc, char** argv)
{
    if(argc < 2)
        return 0;
    
    string file = "key.dat";
    int id = 42;
    
    if(argv[1][0] == 's'){
        ShmClient shm(ShmClient::Type::kWriter, file, id, 0x1000);
        if(!shm) abort();

        shm.Write(0, 0x1234);
        cin.get();
    }

    if(argv[1][0] == 'r'){
        ShmClient shm(ShmClient::Type::kReader, file, id);
        if(!shm) abort();

        uint32_t val = shm.Read(0);
        cout << val << endl;
    }

    return 0;
}