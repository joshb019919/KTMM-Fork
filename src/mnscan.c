/*
 *  mnscan.c
 *
 *  Scan pages for each node in the system.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>

#include "mnscan.h"

int avail_nodes(void) {
	int online_numa_nodes = num_online_nodes();
	//int node_id_list[online_numa_nodes];
	
	return online_numa_nodes;
}
