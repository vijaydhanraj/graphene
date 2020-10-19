/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 Intel Corporation */

/*!
 * \file
 *
 * This file contains the implementation of `/sys` pseudo-filesystem.
 */
#define _GNU_SOURCE
#include <string.h>
#include "shim_fs.h"

static int sys_info_mode(const char* name, mode_t* mode) {
    __UNUSED(name);
    *mode = FILE_R_MODE | S_IFREG;
    return 0;
}

static int sys_info_stat(const char* name, struct stat* buf) {
    __UNUSED(name);
    memset(buf, 0, sizeof(*buf));
    buf->st_dev  = 1;    /* dummy ID of device containing file */
    buf->st_ino  = 1;    /* dummy inode number */
    buf->st_mode = FILE_R_MODE | S_IFREG;
    return 0;
}

/* This function will extract 3 from "../cpu/cpu3/topology/core_siblings" */
static int extract_num_from_path(const char* path) {
    const char* str =path;
    int ret = -1;

    while(*str) {
        /* TODO: isdigit is complaining undefined reference to `__ctype_b_loc'
         * Need to revisit this
         */
        if ((*str >= '0') && (*str <= '9') ) {
            ret = strtol(str, NULL, 10);
            debug("%s: path=%s, num=%d\n", __func__, path, ret);
            break;
        }
        str++;
    }

    return ret;
}

static int sys_cpuinfo_open(struct shim_handle* hdl, const char* name, int flags) {
    __UNUSED(hdl);
    __UNUSED(flags);
    const char* filename      = name;
    char* str = malloc(SYSFS_FILESIZE);
    if (!str)
        return -ENOMEM;
    int len;

    /* Extract name of the file */
    /* strrchr doesn't seem to be supported so using strchr*/
     while (filename && *filename) {
        char* slash_ptr = strchr(filename, '/');
        if (!slash_ptr)
            break;

        filename = slash_ptr + 1; /* set it past '/' */
    }

    debug("%s: path=%s, filename=%s\n", __func__, name, filename);

    if (!strcmp_static(filename, "online")) {
        const char* online_info;
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 || (cpunum == 0))
            return -ENOENT;
        
        online_info = pal_control.topo_info.cpu_topology[cpunum].is_online;
        len = strlen(online_info) + 1;
        memcpy(str, online_info, len);
   
    } else {
        debug("Unsupported Filepath %s\n", name);
        return -ENOENT;
    }

    struct shim_str_data* data = malloc(sizeof(struct shim_str_data));
    if (!data) {
        free(str);
        return -ENOMEM;
    }

    memset(data, 0, sizeof(struct shim_str_data));
    data->str          = str;
    data->len          = len;
    hdl->type          = TYPE_STR;
    hdl->flags         = flags & ~O_RDONLY;
    hdl->acc_mode      = MAY_READ;
    hdl->info.str.data = data;

    return 0;
}

static int cpu_cache_info_open (struct shim_handle* hdl, const char* name, int flags) {
    __UNUSED(hdl);
    __UNUSED(flags);
    const char* filename      = name;
    char* str = malloc(SYSFS_FILESIZE);
    if (!str)
        return -ENOMEM;
    int len;
    int idx = -1;

    /* Extract name of the file */
    /* strrchr doesn't seem to be supported so using strchr*/
     while (filename && *filename) {
        char* slash_ptr = strchr(filename, '/');
        if (!slash_ptr)
            break;

        filename = slash_ptr + 1; /* set it past '/' */
    }

    debug("%s: path=%s, filename=%s\n", __func__, name, filename);

    /* Identify cache index */
    if (strstr(name, "index0")) {
        idx = L1_DATA;
    } else if (strstr(name, "index1")) {
        idx = L1_INST;
    } else if (strstr(name, "index2")) {
        idx = L2;
    } else if (strstr(name, "index3")) {
        idx = L3;
    } else {
        return -ENOENT;
    }

    if (!strcmp_static(filename, "shared_cpu_map")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* cachemap;
        cachemap = pal_control.topo_info.cpu_topology[cpunum].cache[idx].shared_cpu_map;
        len =  strlen(cachemap) + 1;
        memcpy(str, cachemap, len);

    } else if (!strcmp_static(filename, "shared_cpu_map")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* cachemap;
        cachemap = pal_control.topo_info.cpu_topology[cpunum].cache[idx].shared_cpu_map;
        len =  strlen(cachemap) + 1;
        memcpy(str, cachemap, len);

    } else if (!strcmp_static(filename, "shared_cpu_map")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* cachemap;
        cachemap = pal_control.topo_info.cpu_topology[cpunum].cache[idx].shared_cpu_map;
        len =  strlen(cachemap) + 1;
        memcpy(str, cachemap, len);

    } else if (!strcmp_static(filename, "level")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* level;
        level = pal_control.topo_info.cpu_topology[cpunum].cache[idx].level;
        len =  strlen(level) + 1;
        memcpy(str, level, len);

    } else if (!strcmp_static(filename, "type")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* type;
        type = pal_control.topo_info.cpu_topology[cpunum].cache[idx].type;
        len =  strlen(type) + 1;
        memcpy(str, type, len);

    } else if (!strcmp_static(filename, "size")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* size;
        size = pal_control.topo_info.cpu_topology[cpunum].cache[idx].size;
        len =  strlen(size) + 1;
        memcpy(str, size, len);

    } else if (!strcmp_static(filename, "coherency_line_size")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* coherency;
        coherency = pal_control.topo_info.cpu_topology[cpunum].cache[idx].coherency_line_size;
        len =  strlen(coherency) + 1;
        memcpy(str, coherency, len);

    } else if (!strcmp_static(filename, "number_of_sets")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* sets;
        sets  = pal_control.topo_info.cpu_topology[cpunum].cache[idx].number_of_sets;
        len =  strlen(sets) + 1;
        memcpy(str, sets, len);

    } else if (!strcmp_static(filename, "physical_line_partition")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* partition;
        partition = pal_control.topo_info.cpu_topology[cpunum].cache[idx].physical_line_partition;
        len =  strlen(partition) + 1;
        memcpy(str, partition, len);

    } else {
        debug("Unsupported Filepath %s\n", name);
        return -ENOENT;
    }

    struct shim_str_data* data = malloc(sizeof(struct shim_str_data));
    if (!data) {
        free(str);
        return -ENOMEM;
    }

    memset(data, 0, sizeof(struct shim_str_data));
    data->str          = str;
    data->len          = len;
    hdl->type          = TYPE_STR;
    hdl->flags         = flags & ~O_RDONLY;
    hdl->acc_mode      = MAY_READ;
    hdl->info.str.data = data;

    return 0;
}

static int sys_info_open(struct shim_handle* hdl, const char* name, int flags) {
    __UNUSED(hdl);
    __UNUSED(flags);
    const char* filename      = name;
    //char* str = malloc(SYSFS_FILESIZE);
    char* str = malloc(2048);
    if (!str)
        return -ENOMEM;
    int len;

    /* Extract name of the file */
    /*strrchr doesn't seem to be supported so using strchr*/
     while (filename && *filename) {
        char* slash_ptr = strchr(filename, '/');
        if (!slash_ptr)
            break;

        filename = slash_ptr + 1; /* set it past '/' */
    }

    debug("%s: path=%s, filename=%s\n", __func__, name, filename);

    if (!strcmp_static(filename, "online")) {
        const char* online_info;
        if (strstr(name, "node")) {
            online_info = pal_control.topo_info.node_online;
        } else {
            online_info = pal_control.topo_info.cpu_online;
        }
        len = strlen(online_info) + 1;
        memcpy(str, online_info, len);

    } else if (!strcmp_static(filename, "possible")) {
        const char* possible_info;
        if (strstr(name, "node")) {
            possible_info = pal_control.topo_info.node_possible;
        } else {
            possible_info = pal_control.topo_info.cpu_possible;
        }
        len = strlen(possible_info) + 1;
        memcpy(str, possible_info, len);

    } else if (!strcmp_static(filename, "core_id")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* core_id;
        core_id = pal_control.topo_info.cpu_topology[cpunum].core_id;
        len = strlen(core_id) + 1;
        memcpy(str, core_id, len);

    } else if (!strcmp_static(filename, "physical_package_id")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* physical_package_id;
        physical_package_id = pal_control.topo_info.cpu_topology[cpunum].physical_package_id;
        len = strlen(physical_package_id) + 1;
        memcpy(str, physical_package_id, len);

    } else if (!strcmp_static(filename, "core_siblings")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* coresiblings_info;
        coresiblings_info = pal_control.topo_info.cpu_topology[cpunum].core_siblings;
        len = strlen(coresiblings_info) + 1;
        memcpy(str, coresiblings_info, len);

    } else if (!strcmp_static(filename, "thread_siblings")) {
        int cpunum = extract_num_from_path(name);
        if (cpunum < 0 )
            return -ENOENT;
        
        const char* threadsiblings_info;
        threadsiblings_info = pal_control.topo_info.cpu_topology[cpunum].thread_siblings;
        len = strlen(threadsiblings_info) + 1;
        memcpy(str, threadsiblings_info, len);

    } else if (!strcmp_static(filename, "cpumap")) {
        int nodenum = extract_num_from_path(name);
        if (nodenum < 0 )
            return -ENOENT;
        
        const char* cpumap_info;
        cpumap_info = pal_control.topo_info.numa_topology[nodenum].cpumap;
        len =  strlen(cpumap_info) + 1;
        memcpy(str, cpumap_info, len);

    } else if (!strcmp_static(filename, "meminfo")) {
        int nodenum = extract_num_from_path(name);
        if (nodenum < 0 )
            return -ENOENT;
        
        const char* meminfo;
        meminfo = pal_control.topo_info.numa_topology[nodenum].meminfo;
        len =  strlen(meminfo) + 1;
        memcpy(str, meminfo, len);

    } else if (!strcmp_static(filename, "distance")) {
        int nodenum = extract_num_from_path(name);
        if (nodenum < 0 )
            return -ENOENT;
        
        const char* distance;
        distance = pal_control.topo_info.numa_topology[nodenum].distance;
        len =  strlen(distance) + 1;
        memcpy(str, distance, len);

    } else if (strstr(name, "hugepages-2048kB/nr_hugepages")) {
        int nodenum = extract_num_from_path(name);
        if (nodenum < 0 )
            return -ENOENT;
        
        const char* nr_hugepages_2M;
        nr_hugepages_2M = pal_control.topo_info.numa_topology[nodenum].hugepages[0].nr_hugepages;
        len =  strlen(nr_hugepages_2M) + 1;
        memcpy(str, nr_hugepages_2M, len);

    } else if (strstr(name, "hugepages-1048576kB/nr_hugepages")) {
        int nodenum = extract_num_from_path(name);
        if (nodenum < 0 )
            return -ENOENT;
        
        const char* nr_hugepages_1G;
        nr_hugepages_1G = pal_control.topo_info.numa_topology[nodenum].hugepages[1].nr_hugepages;
        len =  strlen(nr_hugepages_1G) + 1;
        memcpy(str, nr_hugepages_1G, len);

    } else {
        debug("Unsupported Filepath %s\n", name);
        return -ENOENT;
    }

    struct shim_str_data* data = malloc(sizeof(struct shim_str_data));
    if (!data) {
        free(str);
        return -ENOMEM;
    }

    memset(data, 0, sizeof(struct shim_str_data));
    data->str          = str;
    data->len          = len;
    hdl->type          = TYPE_STR;
    hdl->flags         = flags & ~O_RDONLY;
    hdl->acc_mode      = MAY_READ;
    hdl->info.str.data = data;

    return 0;
}

static const struct pseudo_fs_ops fs_topoinfo = {
    .mode = &sys_info_mode,
    .stat = &sys_info_stat,
    .open = &sys_info_open,
};

static const struct pseudo_fs_ops cpu_cache_info = {
    .mode = &sys_info_mode,
    .stat = &sys_info_stat,
    .open = &cpu_cache_info_open,
};

static const struct pseudo_fs_ops fs_cpu_topoinfo = {
    .mode = &sys_info_mode,
    .stat = &sys_info_stat,
    .open = &sys_cpuinfo_open,
};

static int sys_info_dir_open(struct shim_handle* hdl, const char* name, int flags) {
    __UNUSED(hdl);
    __UNUSED(name);

    if (flags & (O_WRONLY | O_RDWR))
        return -EISDIR;

    return 0;
}

static int sys_info_dir_mode(const char* name, mode_t* mode) {
    __UNUSED(name);

    *mode = 0500;
    return 0;
}

static int sys_info_dir_stat(const char* name, struct stat* buf) {
    __UNUSED(name);

    /* Both fstat64 and fstat implementations can end up here.
     * commenting the memset below to avoid any memory corruption.
     */
    debug("%s: name=%s\n", __func__, name);
    memset(buf, 0, sizeof(*buf));
    buf->st_mode  = 0500 | S_IFDIR;
    buf->st_nlink = 4;  //dummy FIXME: need atleast 2 or more links for hwloc

    return 0;
}

static const struct pseudo_fs_ops fs_topodir_info = {
    .mode = &sys_info_dir_mode,
    .stat = &sys_info_dir_stat,
    .open = &sys_info_dir_open,
};

static const struct pseudo_dir cpu_cache_index_dir = {
    .size = 7,
    .ent  = {
              { .name   = "shared_cpu_map",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "level",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "type",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "size",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "coherency_line_size",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "number_of_sets",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
              { .name   = "physical_line_partition",
                .fs_ops = &cpu_cache_info,
                .type   = LINUX_DT_REG },
            }
};

static const struct pseudo_dir sys_cpu_cache_dir = {
    .size = 4,
    .ent  = {
              { .name   = "index0",
                .fs_ops = &fs_topodir_info,
                .dir    = &cpu_cache_index_dir,  
                .type   = LINUX_DT_DIR },
              { .name   = "index1",
                .fs_ops = &fs_topodir_info,
                .dir    = &cpu_cache_index_dir,  
                .type   = LINUX_DT_DIR },
              { .name   = "index2",
                .fs_ops = &fs_topodir_info,
                .dir    = &cpu_cache_index_dir,  
                .type   = LINUX_DT_DIR },
              { .name   = "index3",
                .fs_ops = &fs_topodir_info,
                .dir    = &cpu_cache_index_dir,  
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_cpu_topo_dir = {
    .size = 4,
    .ent  = {
              { .name   = "core_id",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "physical_package_id",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "core_siblings",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "thread_siblings",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
            }
};

static const struct pseudo_dir sys_hugepage_info_dir = {
    .size = 1,
    .ent  = {
              { .name   = "nr_hugepages",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG }, 
            }
};

static const struct pseudo_dir sys_node_topo_dir = {
    .size = 2,
    .ent  = {
              { .name   = "hugepages-2048kB",
                .fs_ops = &fs_topodir_info,
                .dir    = &sys_hugepage_info_dir, 
                .type   = LINUX_DT_DIR }, 
              { .name   = "hugepages-1048576kB",
                .fs_ops = &fs_topodir_info,
                .dir    = &sys_hugepage_info_dir,
                .type   = LINUX_DT_DIR },
            }
};

static int sys_match_cpu_node_num(const char* name) {
    int num, totalcnt;
    //Extract cpu/node num from name
    num = extract_num_from_path(name);
    if (num < 0)
        return 0;

    //Check if num falls within num online
    if (strstr(name, "node")) {
        totalcnt = pal_control.topo_info.num_nodes;
    } else {
        totalcnt = pal_control.topo_info.num_cpus;
    }

    if (num > totalcnt) {
        debug("Incorrect cpu/node num=%d in path=%s. Max online cpu/node is %d\n", num, name, totalcnt);
        return 0;
    }

    return 1;
}

static int sys_list_cpu_node_num(const char* name, struct shim_dirent** buf, int len) {
    bool is_node;
    int totalcnt;
    char ent_name[32];
    struct shim_dirent* dirent_in_buf = *buf;
    size_t buf_size = len;
    size_t total_size = 0;

    //Check if num falls within num online
    if (strstr(name, "node")) {
        is_node = true;
        totalcnt = pal_control.topo_info.num_nodes;

    } else {
        is_node = false;
        totalcnt = pal_control.topo_info.num_cpus;
    }

    for (int i = 0; i < totalcnt; i++) {
        if (is_node) {
            snprintf(ent_name, sizeof(ent_name), "node%d", i);
        } else {
            snprintf(ent_name, sizeof(ent_name), "cpu%d", i);
        }

        size_t name_size   = strlen(ent_name) + 1;
        size_t dirent_size = sizeof(struct shim_dirent) + name_size;

        total_size += dirent_size;
        if (total_size > buf_size)
            return -ENOMEM;

        memcpy(dirent_in_buf->name, ent_name, name_size);
        dirent_in_buf->next = (void*)dirent_in_buf + dirent_size;
        dirent_in_buf->ino  = 1;
        dirent_in_buf->type = LINUX_DT_DIR;

        dirent_in_buf  = dirent_in_buf->next;
    }

    *buf = dirent_in_buf;
    return 1;
}

static const struct pseudo_name_ops cpu_num = {
    .match_name = &sys_match_cpu_node_num,
    .list_name = &sys_list_cpu_node_num,
};

static const struct pseudo_name_ops node_num = {
    .match_name = &sys_match_cpu_node_num,
    .list_name = &sys_list_cpu_node_num,
};

static const struct pseudo_dir sys_cpu_num_dir = {
    .size = 3,
    .ent  = {
              { .name   = "online",
		        .fs_ops = &fs_cpu_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "topology",
		        .fs_ops = &fs_topodir_info,
                .dir    = &sys_cpu_topo_dir,
                .type   = LINUX_DT_DIR },
              { .name   = "cache",
		        .fs_ops = &fs_topodir_info,
                .dir    = &sys_cpu_cache_dir,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_cpu_dir = {
    .size = 3,
    .ent  = {
              { .name   = "online",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "possible",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name_ops = &cpu_num,
                .dir      = &sys_cpu_num_dir,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_node_num_dir = {
    .size = 4,
    .ent  = {
              { .name   = "cpumap",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "meminfo",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "distance",
                .fs_ops = &fs_topoinfo,
                .type   = LINUX_DT_REG },
              { .name   = "hugepages",
                .fs_ops = &fs_topodir_info,
                .dir    = &sys_node_topo_dir,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_node_dir = {
    .size = 3,
    .ent  = {
              { .name     = "online",
                .fs_ops   = &fs_topoinfo,
                .type     = LINUX_DT_REG },
              { .name     = "possible",
                .fs_ops   = &fs_topoinfo,
                .type     = LINUX_DT_REG },
              { .name_ops = &node_num,
                .dir      = &sys_node_num_dir,
                .type     = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_sys_dir = {
    .size = 2,
    .ent  = {
              { .name   = "cpu",
                .dir    = &sys_cpu_dir,
		        .fs_ops = &fs_topodir_info,
                .type   = LINUX_DT_DIR },
              { .name   = "node",
                .dir    = &sys_node_dir,
		        .fs_ops = &fs_topodir_info,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_dev_dir = {
    .size = 1,
    .ent  = {
              { .name   = "system",
                .dir    = &sys_sys_dir,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_dir sys_root_dir = {
    .size = 1,
    .ent  = {
              { .name   = "devices",
                .dir    = &sys_dev_dir,
                .type   = LINUX_DT_DIR },
            }
};

static const struct pseudo_fs_ops sys_root_fs = {
    .open = &pseudo_dir_open,
    .mode = &pseudo_dir_mode,
    .stat = &pseudo_dir_stat,
};

static const struct pseudo_ent sys_root_ent = {
    .name   = "",
    .fs_ops = &sys_root_fs,
    .dir    = &sys_root_dir,
};

static int sys_mode(struct shim_dentry* dent, mode_t* mode) {
    return pseudo_mode(dent, mode, &sys_root_ent);
}

static int sys_lookup(struct shim_dentry* dent) {
    return pseudo_lookup(dent, &sys_root_ent);
}

static int sys_open(struct shim_handle* hdl, struct shim_dentry* dent, int flags) {
    return pseudo_open(hdl, dent, flags, &sys_root_ent);
}

static int sys_readdir(struct shim_dentry* dent, struct shim_dirent** dirent) {
    return pseudo_readdir(dent, dirent, &sys_root_ent);
}

static int sys_stat(struct shim_dentry* dent, struct stat* buf) {
    return pseudo_stat(dent, buf, &sys_root_ent);
}

static int sys_hstat(struct shim_handle* hdl, struct stat* buf) {
    return pseudo_hstat(hdl, buf, &sys_root_ent);
}

struct shim_fs_ops sys_fs_ops = {
    .mount   = &pseudo_mount,
    .unmount = &pseudo_unmount,
    .close   = &str_close,
    .read    = &str_read,
   /*.write   = &str_write,
    .flush   = &str_flush,*/
    .seek    = &str_seek,
    .hstat   = &sys_hstat,
};

struct shim_d_ops sys_d_ops = {
    .open        = &sys_open,
    .stat        = &sys_stat,
    .mode        = &sys_mode,
    .lookup      = &sys_lookup,
    .readdir     = &sys_readdir,
};
