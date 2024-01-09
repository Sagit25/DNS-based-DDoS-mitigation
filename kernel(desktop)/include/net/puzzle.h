/* Created by shyang */

#ifndef _PUZZLE_H
#define _PUZZLE_H

#include <linux/list.h>

#define PZLTYPE_NONE 1
#define PZLTYPE_EXT 2

#define PZLTYPE_MAX 63

#define MAX_ASSIGNED_LENGTH 255
#define MAX_SPARE_GAP 63

struct puzzle_policy {
	u32 ip;
	u32 threshold;
	u32 seed;
	u32 seed_old;
	struct list_head list;
};

struct puzzle_cache {
	u32 ip;
	u8 puzzle_type;
	u32 puzzle;
	u32 threshold;
	struct list_head list;
};

long do_set_puzzle(u32 nonce, u32 seed, u32 dns_ip, u32 client_ip, u8 puzzle_type);
long do_solve_puzzle(u32 threshold, u32 puzzle, u32 dns_ip, u32 client_ip, u8 puzzle_type);
bool find_puzzle_policy(u32 ip, struct puzzle_policy** ptr);
bool find_puzzle_cache(u32 ip, struct puzzle_cache** ptr);
u32 update_to_new_seed(struct puzzle_policy* policy, u32 new_seed);
long add_policy(u32 ip, u32 threshold);
long update_policy(u32 ip, u32 seed, u32 threshold);
int do_check_puzzle(u8 type, u32 puzzle, u32 nonce, u32 policy_ip);
long do_get_threshold(u32 ip);
u32 do_set_threshold(u32 ip, u32 threshold);
long do_get_puzzle_type(void);
long do_set_puzzle_type(u8 type);
long do_get_local_dns(void);
long do_set_local_dns(u32 ip, u32 port);

#endif