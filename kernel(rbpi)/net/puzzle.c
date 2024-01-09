/* Created by YSH */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/types.h>
#include <net/puzzle.h>
#include <crypto/hash.h>

#define PLAIN_LENGTH 17
#define SHA256_LENGTH 32

static const u32 NOT_FOUND = 500;

LIST_HEAD(policy_head);
LIST_HEAD(cache_head);

static u8 puzzle_type = PZLTYPE_NONE;
static bool addlock = false;

static struct puzzel_dns_info {
    u32 ip;
    u32 port;
} puzzle_dns = {0, 0};

long do_set_puzzle(__u32 nonce, __u32 seed, __u32 dns_ip, __u32 client_ip, __u8 puzzle_type) {
    unsigned char plaintext[PLAIN_LENGTH];
    unsigned char hash_sha256[SHA256_LENGTH];
    struct crypto_shash *sha256;
    struct shash_desc *shash;
    __u32 size, result, offset, temp;
    __u32 i, j = 0;
    for(i = 0; i < 4; ++i, ++j) {
        plaintext[j] = nonce & 255;
        nonce >>= 8;
    }
    for(i = 0; i < 4; ++i, ++j) {
        plaintext[j] = seed & 255;
        seed >>= 8;
    }
    for(i = 0; i < 4; ++i, ++j) {
        plaintext[j] = dns_ip & 255;
        dns_ip >>= 8;
    }
    for(i = 0; i < 4; ++i, ++j) {
        plaintext[j] = client_ip & 255;
        client_ip >>= 8;
    }
    for(i = 0; i < 1; ++i, ++j) {
        plaintext[j] = puzzle_type & 255;
        puzzle_type >>= 8;
    }
    sha256 = crypto_alloc_shash("sha256", 0, 0);
    size = sizeof(struct shash_desc) + crypto_shash_descsize(sha256);
    shash = kmalloc(size, GFP_KERNEL);
    
    if(sha256 == NULL) {
        return 0;
    }
    shash -> tfm = sha256;
    
    crypto_shash_init(shash);
    crypto_shash_update(shash, plaintext, PLAIN_LENGTH);
    crypto_shash_final(shash, hash_sha256);
    crypto_free_shash(sha256);
    kfree(shash);
    result = 0;
    for(i = 0; i < 4; ++i) {
        result = result << 8;
        temp = 0;
        offset = i << 3;
        for(j = 0; j < 8; ++j) {
            temp = temp ^ hash_sha256[offset + j];
        }
        result = result + temp;
    }
    return 0;
}
SYSCALL_DEFINE5(set_puzzle, __u32, nonce, __u32, puzzle, __u32, dns_ip, __u32, client_ip, __u8, puzzle_type) {
    return do_set_puzzle(nonce, puzzle, dns_ip, client_ip, puzzle_type);
}

long do_solve_puzzle(__u32 threshold, __u32 puzzle, __u32 dns_ip, __u32 client_ip, __u8 puzzle_type) {
    __u32 nonce;
    for(nonce = 1; nonce > 0; ++nonce) {
        if(do_set_puzzle(nonce, puzzle, dns_ip, client_ip, puzzle_type) < threshold) {
            return nonce;
        }
    }
    return 0;
}
EXPORT_SYMBOL(do_solve_puzzle);
SYSCALL_DEFINE5(solve_puzzle, __u32, threshold, __u32, puzzle, __u32, target_ip, __u32, target_port, __u8, puzzle_type) {
    return do_solve_puzzle(threshold, puzzle, target_ip, target_port, puzzle_type);
}

bool find_puzzle_policy(u32 ip, struct puzzle_policy** ptr) {
    struct puzzle_policy* policy;
    struct list_head* head;
    switch(puzzle_type) {
    case PZLTYPE_EXT:
        list_for_each(head, &policy_head) {
            policy = list_entry(head, struct puzzle_policy, list);
            if(ip == policy->ip) {
                *ptr = policy;
                return true;
            }
        }
        return false;
    default:
        return false;
    }
}
EXPORT_SYMBOL(find_puzzle_policy);

bool find_puzzle_cache(u32 ip, struct puzzle_cache** ptr) {
    struct puzzle_cache* cache;
    struct list_head* head;
    list_for_each(head, &cache_head) {
        cache = list_entry(head, struct puzzle_cache, list);
        if(ip == cache->ip) {
            *ptr = cache;
            return true;
        }
    }

    return false;
}
EXPORT_SYMBOL(find_puzzle_cache);

u32 update_to_new_seed(struct puzzle_policy* policy, u32 new_seed) {
    policy->seed_old = policy->seed;
    policy->seed = new_seed;
    return new_seed;
}

long add_policy(u32 ip, u32 threshold) {
    struct puzzle_policy* policy;
    if(addlock)
        return -1;
    if(find_puzzle_policy(ip, &policy))
        return -1;
    
    policy = kmalloc(sizeof(*policy), GFP_KERNEL);
    memset(policy, 0, sizeof(*policy));

    policy->ip = ip;
    policy->threshold = threshold;


    list_add_tail(&(policy->list), &policy_head);

    return 0;
}
EXPORT_SYMBOL(add_policy);
SYSCALL_DEFINE2(puzzle_add_policy, __u32, ip, __u32, threshold)
{
    return add_policy(ip, threshold);
}

long update_policy(u32 ip, u32 seed, u32 threshold) {
    struct puzzle_policy* policy;

    if(unlikely(!find_puzzle_policy(ip, &policy)))
        return -1;
    if(threshold)
        policy->threshold = threshold;
    if(seed)
        update_to_new_seed(policy, seed);

    return 0;
}
EXPORT_SYMBOL(update_policy);

int do_check_puzzle(u8 type, u32 puzzle, u32 nonce, u32 policy_ip) {
    struct puzzle_policy* policy;
    printk(KERN_INFO "type : %u, puzzle : %u, nonce : %u", type, puzzle, nonce);
    if (puzzle_type == PZLTYPE_NONE)
        return 1;
    if (do_solve_puzzle(nonce, puzzle, puzzle_dns.ip, policy_ip, type) >= policy->threshold)
       return 1;
    return 0;
}
EXPORT_SYMBOL(do_check_puzzle);
SYSCALL_DEFINE4(check_puzzle, __u8, type, __u32, puzzle, __u32, nonce, __u32, policy_ip)
{
    return do_check_puzzle(type, puzzle, nonce, policy_ip);
}

long do_get_threshold(u32 ip) {
	struct puzzle_policy * policy;
	if(unlikely(!find_puzzle_policy(ip, &policy)))
		return 0;
	return policy->threshold;
	
}
SYSCALL_DEFINE1(get_threshold, __u32, ip) {
	return do_get_threshold(ip);
}

u32 do_set_threshold(u32 ip, u32 threshold) {
	struct puzzle_policy * policy;
	if(likely(find_puzzle_policy(ip, &policy)))
		return policy->threshold = threshold;
	return 0;
}
EXPORT_SYMBOL(do_set_threshold);
SYSCALL_DEFINE2(set_threshold, __u32, ip, __u32, threshold) {
	return do_set_threshold(ip, threshold);
}

long do_get_puzzle_type() {
    return puzzle_type;
}
EXPORT_SYMBOL(do_get_puzzle_type);
SYSCALL_DEFINE0(get_puzzle_type) {
    return (long)do_get_puzzle_type();
}
long do_set_puzzle_type(u8 type) {
    struct puzzle_policy * policy;

    if (type == PZLTYPE_NONE) 
        return puzzle_type;
    
    puzzle_type = type;
    addlock = true;
    while (!list_empty(&policy_head)) {
        policy = list_first_entry(&policy_head, struct puzzle_policy, list);
        list_del(&(policy->list));
        kfree(policy);
    }
    addlock = false;
    return puzzle_type;
}
EXPORT_SYMBOL(do_set_puzzle_type);
SYSCALL_DEFINE1(set_puzzle_type, __u8, type) {
    return do_set_puzzle_type(type);
}

long do_get_local_dns() {
    return puzzle_dns.ip;
}
SYSCALL_DEFINE0(get_local_dns) {
    return do_get_local_dns();
}

long do_set_local_dns(u32 ip, u32 port) {
    puzzle_dns.ip = ip;
    puzzle_dns.port = port;
    return 0;
}
EXPORT_SYMBOL(do_set_local_dns);
SYSCALL_DEFINE2(set_local_dns, __u32, ip, __u32, port) {
    return do_set_local_dns(ip, port);
}

