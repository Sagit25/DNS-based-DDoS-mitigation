/* Created by YSH */

#ifndef _PUZZLE_H
#define _PUZZLE_H

#include <linux/list.h>

#define PZLTYPE_NONE 1
#define PZLTYPE_ORG 2

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

u32 do_set_puzzle(__u32 nonce, __u32 seed, __u32 dns_ip, __u32 client_ip, __u8 puzzle_type)
bool find_puzzle_policy(u32 ip, struct puzzle_policy** ptr)
bool find_puzzle_cache(u32 ip, struct puzzle_cache** ptr)
u32 update_to_new_seed(struct puzzle_policy* policy, u32 new_seed)
u32 generate_new_seed(u32 ip)
long add_policy(u32 ip, u32 threshold)
long update_policy(u32 ip, u32 seed, u32 threshold)
int do_check_puzzle(u8 type, u32 puzzle, u32 nonce, u32 policy_ip)
int do_set_puzzle_cache(u32 ip, u8 type, u32 puzzle, u32 threshold) 
u32 do_get_threshold(u32 ip) 
u32 do_set_threshold(u32 ip, u3s threshold)
u8 do_get_puzzle_type(void)
u8 do_set_puzzle_type(u8 type) 
long set_local_dns_info(u32* ip, u32* port)
u32 do_get_local_dns(void)
u32 do_solve_puzzle(__u32 threshold, __u32 puzzle, __u32 dns_ip, __u32 client_ip, __u8 puzzle_type)

#endif