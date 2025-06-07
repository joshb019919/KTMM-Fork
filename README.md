# Work In Progress!! 


# Tiered Memory Management Improvement for Linux 5.3
Uses inspiration from Adnan Maruf and Sylab's Multi-Clock built-in TMM to create an external Linux kernel module which does much the same.

The idea of tiered memory management is to allow secondary or even tertiary tiered of memory below the fastest-access DRAM for memory pages.  Linux already has functionality for this where more recently used pages are attempted to be moved to and kept in the faster tier(s) and less recently used pages are kept in lower tiers or reclaimed, altogether.  This is accomplished via two page lists being kept per tier.  An "active list" keeps pages which have been found to be accessed recently and an "inactive list" keeps pages which have not.  Normally, pages are promoted to a higher tier from the active list and demoted or reclaimed from the inactive list.  Multi-Clock and this module use a third, "promote list" for every tier beneath DRAM.  Pages in the active list which are found to be accessed are moved to the promote list, and only promoted to a higher tier if found to be accessed, again.

In this way, we effectively add a check for frequency of page use along with the check for recency of use without incurring the overhead associated with keeping track of every page access.

---

This fork works to implement this at the page level before Linux 6 while the original works at the folio level of Linux 6.1 and above.
