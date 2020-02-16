#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__
#include <stdint.h>

struct hash_entry {
	void *key;
	void *val;
	struct hash_entry *next;
};

struct hash_table {
	uint32_t size;
	struct hash_entry **table;

	uint64_t (*func_hash)(const void *key);
	int (*func_key_cmp)(const void *key1, const void *key2);
};

struct hash_table *hash_table_init(uint32_t size);
int hash_add(struct hash_table *ht, void *key, void *val);
void hash_table_destroy(struct hash_table *ht);

#endif
