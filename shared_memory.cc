#include <cstring>
#include "shared_memory.h"

void ShmClient::Setup(std::string path, int id, int size){
    if(has_init_) return;

    // Create key.
    key_t key = ftok(path.c_str(), id);
    if(key == -1){
        printf("Failed to create key./n");
        return;
    }

    // Acquire segment id.
    segment_id_ = (type_ == Type::kReader) ? shmget(key, 0, 0) : shmget(key, size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if(segment_id_ == -1){
        printf("Failed to acquire segment id/n");
        return;
    }

    // Get size information.
    struct shmid_ds shm_stat;
    shmctl(segment_id_, IPC_STAT, &shm_stat);
    size_ = static_cast<uint32_t>(shm_stat.shm_segsz / sizeof(uint32_t)); 

    // Attach segment.
    buf_ = reinterpret_cast<uint32_t*>(shmat(segment_id_, 0, 0));
    if(buf_ == (void*)-1){
        printf("Failed to attach segment./n");
        return;
    }

    has_init_ = true;
}

void ShmClient::Cleanup(){
    if(!has_init_) return;
    shmdt(buf_);
    if(type_ == Type::kWriter) shmctl (segment_id_, IPC_RMID, 0);
    has_init_ = false;
}

void ShmClient::Write(uint32_t offset, uint32_t value){
    if(!has_init_){
        printf("Has not set up./n");
        return;
    }

    if(offset >= size_){
        printf("Out of range./n");
        return;
    }

    buf_[offset] = value;
}

void ShmClient::Write(uint32_t offset, uint32_t* buf, int size){
    if(!has_init_){
        printf("Has not set up./n");
        return;
    }

    if(offset + size >= size_){
        printf("Out of range./n");
        return;
    }

    memcpy(buf_, buf, size/sizeof(uint32_t));
}

uint32_t ShmClient::Read(uint32_t offset){
    if(!has_init_){
        printf("Has not set up./n");
        return 0;
    }

    if(offset >= size_){
        printf("Out of range./n");
        return 0;
    }

    return buf_[offset];
}

void ShmClient::Read(uint32_t offset, uint32_t* buf, int size){
    if(!has_init_){
        printf("Has not set up./n");
        return;
    }

    if(offset + size >= size_){
        printf("Out of range./n");
        return;
    }

    memcpy(buf, buf_, size/sizeof(uint32_t));
}

