/*
 *  tmemscan.c
 *
 *  Page scanning and related functions for tmem module.
 *
 */
#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/memcontrol.h>
#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/page-flags.h>
#include <linux/page_ref.h>
#include <linux/printk.h>
#include <linux/spinlock.h>

#include "tmemscan.h"


// static struct scan_control = { }


static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name"
};


/* need to aquire spinlock before calling this function */
static unsigned int scan_list(struct list_head *list, enum lru_list lru)
{
	struct folio *folio, *next;
	unsigned int folio_referenced_count;

	folio_referenced_count = 0;

	list_for_each_entry_safe(folio, next, list, lru)
	{
		//
		if(test_bit(PG_referenced, &folio->flags))
			folio_referenced_count++;
	}

	return folio_referenced_count;
}


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

	// define the kallsyms_lookup_name function with kprobe
	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
	kallsyms_lookup_name_t kallsyms_lookup_name;
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
	unregister_kprobe(&kp);


	cgroup_iter_addr = kallsyms_lookup_name("mem_cgroup_iter");

	if(cgroup_iter_addr)
	{
		pr_info("mem_cgroup_iter addr: %lu\n", cgroup_iter_addr);

		// get the mem_cgroup_iter() function from memory address
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
			unsigned int ref_count;
			unsigned long flags;
			struct list_head *list;
			list = &lruvec->lists[lru];

			pr_info("Scanning evictable LRU list: %d\n", lru);

			spin_lock_irqsave(&lruvec->lru_lock, flags);
			// call list_scan function
			ref_count = scan_list(list, lru);
			
			spin_unlock_irqrestore(&lruvec->lru_lock, flags);

			pr_info("Reference count: %d\n", ref_count);
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
