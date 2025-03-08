/*
 *  tmemscan.c
 *
 *  Page scanning and related functions for tmem module.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/memcontrol.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>

#include "tmemscan.h"


// static struct scan_control = { }


// (pg_data_t *pgdat, struct scan_control tsc)
static void scan_node(pg_data_t *pgdat)
{
	enum lru_list lru;
	struct mem_cgroup *memcg;
	struct mem_cgroup *root;
	struct lruvec *lruvec;
	unsigned long node_lru_folios;

	root = NULL;
	memcg = mem_cgroup_iter(root, NULL, NULL);

	lruvec = mem_cgroup_lruvec(memcg, pgdat);

	node_lru_folios = 0;

	for_each_evictable_lru(lru) {
		// call list_scan function
	}
}


// static int mpromoted(void *p)


int avail_nodes(void) {
	return num_online_nodes();
}


int avail_pages(void) {
	unsigned long num_pages;
	int nid;
	
	for_each_online_node(nid) {
		pg_data_t *pgdat = NODE_DATA(nid);
		num_pages += pgdat->node_present_pages;
	}
	return num_pages;
}


