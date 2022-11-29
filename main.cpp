#include <iostream>
#include <sys/fcntl.h>
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

    // create shared memory object
    int fd = shm_open("/my_shm", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        std::cerr << "Error: cannot open shared memory object" << std::endl;
        return 1;
    }
    // check if segment is created
    if (EEXIST == errno) {
        std::cerr << "Error: segment is already created" << std::endl;
        return 1;
    }

    if (EACCES == errno) {
        std::cerr << "Error: EACCES" << std::endl;
        return 1;
    }

    // reject copy of the running it
    ftruncate(fd, 4096);
    // map shared memory object to the address space of the process
    char *ptr = (char *) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error: cannot map shared memory object" << std::endl;
        return 1;
    }
    // write to the shared memory object via write()
    write(fd, "Hello, world!", 14);
    // write to the shared memory object via ptr
   /* ptr[0] = 'H';
    ptr[1] = 'e';
    ptr[2] = 'l';
    ptr[3] = 'l';
    ptr[4] = 'o';
    ptr[5] = '\0';
    write (fd, ptr, 6);*/

    // synchronize changes
    msync(ptr, 4096, MS_SYNC);
    // unmap shared memory object
    //munmap(ptr, 4096);
    // close shared memory object
    //close(fd);
    // remove shared memory object
    //shm_unlink("/my_shm");

    return 0;
}
