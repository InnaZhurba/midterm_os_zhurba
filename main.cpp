#include <iostream>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>

// task 13
// key-words: shared memory, segments

int main() {
    // check if segment with name "/my_shm" exists
    // if it exists - delete it
    /*if (shm_unlink("/my_shm") == -1) {
        std::cerr << "Error: cannot delete segment" << std::endl;
        return 1;
    } else {
        std::cout << "Segment deleted" << std::endl;
    }*/
    // via scmget get segment and deny access for other users

    if (shmget(0, 4096, IPC_CREAT | IPC_EXCL | 0666) == -1) {
        std::cerr << "Error: cannot create segment" << std::endl;
        return 1;
    }

    // shmctl(int shmid, int cmd, struct shmid_ds *buf)
    // lock segment

    if(shmctl(0, SHM_LOCK, NULL) == -1) {
        std::cerr << "Error: cannot lock segment" << std::endl;
        return 1;
    }

    // shared memory oblect elaw only one running process can access it
    // create shared memory object
    const int SIZE = 4096;
    const char* name = "/my_shm";
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error: cannot create shared memory object" << std::endl;
        return 1;
    }
    // configure the size of the shared memory object
    ftruncate(shm_fd, SIZE);
    // memory map the shared memory object
    void* ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error: cannot map shared memory object" << std::endl;
        return 1;
    }
    // write to the shared memory object
    sprintf((char*)ptr, "Hello, World!");
    // remove the shared memory object
    if (shm_unlink(name) == -1) {
        std::cerr << "Error: cannot delete segment" << std::endl;
        return 1;
    } else {
        std::cout << "Segment deleted" << std::endl;
    }
    return 0;
}
