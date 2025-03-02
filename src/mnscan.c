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
	return num_online_nodes();
}


int avail_pages(void) {
	//unsigned int node_id_list[MAX_NUMNODES];
	unsigned long num_pages;
	int nid;
	
	for_each_online_node(nid) {
		pg_data_t *pgdat = NODE_DATA(nid);
		
		num_pages += pgdat->node_present_pages;
	}
	return num_pages;
}
