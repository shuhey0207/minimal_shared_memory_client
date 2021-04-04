#include <bits/stdc++.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
using namespace std;

class ShmClient{
public:
    enum class Type{
        kWriter,
        kReader,
    };

    ShmClient(Type type) : type_(type){}
    ShmClient(Type type, std::string path, int id, int size = 0) : 
        type_(type){ 
        Setup(path, id, size); 
    }
    ~ShmClient(){
        Cleanup();
    }
    operator    bool() const {return has_init_;}
    void        Setup(std::string path, int id, int size);
    void        Cleanup();
    void        Write(uint32_t offset, uint32_t value);
    void        Write(uint32_t offset, uint32_t* buf, int size);
    uint32_t    Read(uint32_t offset);
    void        Read(uint32_t offset, uint32_t* buf, int size);

private:
    bool        has_init_ = false;
    int         size_;
    Type        type_;
    int         segment_id_;
    uint32_t*   buf_ = nullptr;

    

};
