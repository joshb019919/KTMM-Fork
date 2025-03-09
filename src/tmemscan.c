/*
 *  tmemscan.c
 *
 *  Page scanning and related functions for tmem module.
 *
 */
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/memcontrol.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/printk.h>
#include <linux/spinlock.h>

#include "tmemscan.h"


// static struct scan_control = { }


static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name"
};


/* need to aquire spinlock before calling this function */
static void scan_list(pg_data_t *pgdat, int nid, struct lruvec *lruvec)
{
	// code here
}


// (pg_data_t *pgdat, struct scan_control tsc)
/*
 * This is really hacky, but we had no other choice.
 */
static void scan_node(pg_data_t *pgdat, int nid)
{
	enum lru_list lru;
	struct mem_cgroup *memcg;
	struct mem_cgroup *root;
	struct lruvec *lruvec;
	unsigned long cgroup_iter_addr;
	unsigned long node_lru_folios;

	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
	kallsyms_lookup_name_t kallsyms_lookup_name;
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
	unregister_kprobe(&kp);


	cgroup_iter_addr = kallsyms_lookup_name("mem_cgroup_iter");

	if(cgroup_iter_addr)
	{
		pr_info("mem_cgroup_iter addr: %lu\n", cgroup_iter_addr);

		// sorry, this is a pile of shit
		struct mem_cgroup *(*cgroup_iter_fn)(struct mem_cgroup *root, 
			struct mem_cgroup *prev,
			struct mem_cgroup_reclaim_cookie *reclaim)
			= (struct mem_cgroup *(*)(struct mem_cgroup *, 
				struct mem_cgroup *, 
				struct mem_cgroup_reclaim_cookie *)
				)cgroup_iter_addr;

		root = NULL;

		memcg = cgroup_iter_fn(root, NULL, NULL);

		node_lru_folios = 0;

		// finally, get some!!
		lruvec = &memcg->nodeinfo[nid]->lruvec;

		for_each_evictable_lru(lru) 
		{
			pr_info("Scanning evictable LRU list: %d\n", lru);
			
			spin_lock_irqsave(&lruvec->lru_lock);
			// call list_scan function
			
			spin_lock_irqrestore(&lruvec->lru_lock);
		}
	}
	else
		pr_info("tmem unable to get LRU lists");
}


// static int mpromoted(void *p)


void avail_nodes(void) 
{
	int nid;
	
	for_each_online_node(nid)
	{
		pg_data_t *pgdat = NODE_DATA(nid);
		scan_node(pgdat, nid);
	}
	//return num_online_nodes();
}
