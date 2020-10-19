#include <hwloc.h>
#include <stdio.h>

int main(void) {
  int ret;
  hwloc_topology_t topology;
  hwloc_cpuset_t   process_cpu_affinity_mask;
  hwloc_nodeset_t  process_node_affinity_mask;
  int nbnumanodes=0, numa_nodes_count=0, cpu_count=0;

  hwloc_topology_init(&topology);  // initialization
  hwloc_topology_load(topology);   // actual detection

  nbnumanodes = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE);
  printf("Num Nodes = %d\n", nbnumanodes);

  process_cpu_affinity_mask  = hwloc_bitmap_alloc();
  process_node_affinity_mask = hwloc_bitmap_alloc();

  ret = hwloc_get_cpubind(topology, process_cpu_affinity_mask, 0);
  if (ret < 0) {
    printf("hwloc_get_cpubind failed\n");
  }
  hwloc_cpuset_to_nodeset(topology, process_cpu_affinity_mask, process_node_affinity_mask);

#if 0
  ret = hwloc_cpuset_to_nodeset(topology, process_cpu_affinity_mask, process_node_affinity_mask);
  if (ret < 0) {
    printf("hwloc_cpuset_to_nodeset failed\n");
  }

  //Used in case of windows processor groups
  process_cpu_affinity_mask  = hwloc_bitmap_dup(hwloc_topology_get_complete_cpuset (topology));
  process_node_affinity_mask = hwloc_bitmap_dup(hwloc_topology_get_complete_nodeset(topology));
#endif
  cpu_count = hwloc_bitmap_weight(process_cpu_affinity_mask);
  numa_nodes_count = hwloc_bitmap_weight(process_node_affinity_mask);
   printf("cpu_count = %d, numa_nodes_count = %d\n", cpu_count, numa_nodes_count);

  hwloc_topology_destroy(topology);

  return 0;
}
