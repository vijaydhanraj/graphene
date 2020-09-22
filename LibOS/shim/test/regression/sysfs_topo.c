#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <string.h>

int main(int argc, char** argv) {
    char buf[24576];
    int ret, count = 0;
    FILE* fd;
    DIR* dir;
    struct dirent64* dirent;
    struct stat64 sb;

#if 0
    int sys_dirfd = open("/", O_DIRECTORY | O_PATH | O_RDONLY);
    if (sys_dirfd < 0) {
        perror("open /");
        return 1;
    }
    
    int numberOfProcessors = sysconf(_SC_NPROCESSORS_CONF);
    printf("Number of processors: %d\n", numberOfProcessors);
    
    printf("===== faccess of sys/devices/system/cpu\n");
    ret = faccessat(sys_dirfd, "/sys/devices/system/cpu", R_OK, 0);
    if (ret) {
        perror("faccessat /sys/devices/system/cpu");
        return 1;
    }

    if (close(sys_dirfd) < 0) {
        perror("close sys_dirfd");
        return 1;
    }

    printf("===== Open Dir of sys/devices/system/node\n");
    fd = fopen("/sys/devices/system/node", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/node");
        return 1;
    }

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/node");
        return 1;
    }

    printf("===== Contents of /sys/devices/system/cpu/cpu2/online\n");
    fd = fopen("/sys/devices/system/cpu/cpu2/online", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/cpu/cpu2/online");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("fread /sys/devices/system/cpu/cpu2/online");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/cpu/cpu2/online");
        return 1;
    }

    printf("===== Contents of /sys/devices/system/cpu/cpu2/topology/core_id\n");
    fd = fopen("/sys/devices/system/cpu/cpu2/topology/core_id", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/cpu/cpu2/topology/core_id");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("fread /sys/devices/system/cpu/cpu2/topology/core_id");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/cpu/cpu2/topology/core_id");
        return 1;
    }
    
    printf("===== Contents of /sys/devices/system/cpu/cpu2/topology/physical_package_id\n");
    fd = fopen("/sys/devices/system/cpu/cpu2/topology/physical_package_id", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/cpu/cpu2/topology/physical_package_id");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("fread /sys/devices/system/cpu/cpu2/topology/physical_package_id");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/cpu/cpu2/topology/physical_package_id");
        return 1;
    }

    printf("===== Contents of /sys/devices/system/cpu/cpu3/topology/core_siblings\n");
    fd = fopen("/sys/devices/system/cpu/cpu3/topology/core_siblings", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/cpu/cpu3/topology/core_siblings");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("fread /sys/devices/system/cpu/cpu3/topology/core_siblings");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/cpu/cpu3/topology/core_siblings");
        return 1;
    }
    
    printf("===== /sys/devices/system/node/node0/meminfo\n");
    fd = fopen("/sys/devices/system/node/node0/meminfo", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/node/node0/meminfo");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("/sys/devices/system/node/node0/meminfo");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/node/node0/meminfo");
        return 1;
    }

    printf("===== /sys/devices/system/node/node1/distance\n");
    fd = fopen("/sys/devices/system/node/node1/distance", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/node/node1/distance");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("/sys/devices/system/node/node1/distance");
        return 1;
    }
    buf[ret] = 0;

    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages");
        return 1;
    }

    printf("===== /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages\n");
    fd = fopen("/sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages", "r");
    if (!fd) {
        perror("fopen /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages");
        return 1;
    }

    ret = fread(buf, 1, sizeof(buf) - 1, fd);
    if (ferror(fd)) {
        perror("/sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages");
        return 1;
    }
    printf("%s\n", buf);

    ret = fclose(fd);
    if (ret) {
        perror("fclose /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages");
        return 1;
    }

    printf("===== Contents of /sys/devices/system/cpu\n");
    dir = opendir("/sys/devices/system/cpu");
    if (!dir) {
        perror("opendir /sys/devices/system/cpu");
        return 1;
    }

    errno = 0;
    while ((dirent = readdir(dir))) {
        printf("/sys/devices/system/cpu/%s\n", dirent->d_name);
    }
    if (errno) {
        perror("readdir /sys/devices/system/cpu");
        return 1;
    }

    ret = closedir(dir);
    if (ret < 0) {
        perror("closedir /sys/devices/system/cpu");
        return 1;
    }
#endif
    
    printf("===== /sys/devices/system/cpu/cpu3/cache/index\n");
    
    for (int lvl = 0; lvl < 4; lvl++) {
        char name[128];
        snprintf(name, sizeof(name),"/sys/devices/system/cpu/cpu3/cache/index%d/type", lvl); 
        fd = fopen(name, "r");
        if (!fd) {
            perror("fopen /sys/devices/system/cpu/cpu3/cache/index/type");
            return 1;
        }

        ret = fread(buf, 1, sizeof(buf) - 1, fd);
        if (ferror(fd)) {
            perror("fread /sys/devices/system/cpu/cpu3/cache/index/type");
            return 1;
        }
        printf("%s\n", buf);

        ret = fclose(fd);
        if (ret) {
            perror("fclose /sys/devices/system/cpu/cpu3/cache/index/type");
            return 1;
        }
    }

    int sys_dirfd1 = open("/", O_DIRECTORY | O_PATH | O_RDONLY);
    if (sys_dirfd1 < 0) {
        perror("open /");
        return 1;
    }

    printf("===== fstatat of /sys/devices/system/node/node0/hugepages/\n");
    ret = fstatat64(sys_dirfd1, "/sys/devices/system/node/node0/hugepages", &sb, 0);
    if (ret) {
        perror("faccessat /sys/devices/system/cpu");
        return 1;
    }

    printf("ID of containing device:  [%lx,%lx]\n",
                (long) major(sb.st_dev), (long) minor(sb.st_dev));

    printf("File type:                ");

    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

    printf("I-node number:            %ld\n", (long) sb.st_ino);

    printf("Mode:                     %lo (octal)\n",
                   (unsigned long) sb.st_mode);

    printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
                   (long) sb.st_uid, (long) sb.st_gid);

    printf("Preferred I/O block size: %ld bytes\n",
                   (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
                   (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
                   (long long) sb.st_blocks);


    if (close(sys_dirfd1) < 0) {
        perror("close sys_dirfd");
        return 1;
    }
    
    printf("===== Count num of CPUs from /sys/devices/system/cpu\n");
    dir = opendir("/sys/devices/system/cpu");
    if (!dir) {
        perror("opendir /sys/devices/system/cpu");
        return 1;
    }

    errno = 0;
    while ((dirent = readdir64(dir))) {
        printf("/sys/devices/system/cpu/%s, type=%d\n", dirent->d_name, dirent->d_type);
        if (dirent->d_type == DT_DIR && strncmp (dirent->d_name, "cpu", 3) == 0)
        {
            char *endp;
            unsigned long int nr = strtoul (dirent->d_name + 3, &endp, 10);
            if (nr != _SC_ULONG_MAX && endp != dirent->d_name + 3 && *endp == '\0') {
                printf("CPU num =%ld\n", nr);
                ++count;
            }
        }
    }

    printf("Total CPU count=%d\n", count);
    if (errno) {
        perror("readdir /sys/devices/system/cpu");
        return 1;
    }

    ret = closedir(dir);
    if (ret < 0) {
        perror("closedir /sys/devices/system/cpu");
        return 1;
    }
    return 0;
}
