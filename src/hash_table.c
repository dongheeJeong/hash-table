#include <stdlib.h>
#include "hash_table.h"

struct hash_table* hash_table_init(uint32_t size)
{
	uint32_t i;
	struct hash_table *ht;

	if (size == 0)
		return NULL;

	if ((ht = malloc(sizeof(struct hash_table))) == NULL)
		return NULL;

	if ((ht->table = malloc(sizeof(struct hash_entry*) * size)) == NULL) {
		free(ht);
		return NULL;
	}

	ht->size = size;
	for (i = 0; i < ht->size; i++)
		ht->table[i] = NULL;
	return ht;
}

int hash_add(struct hash_table *ht, void *key, void *val)
{
	uint32_t idx;
	uint64_t hash_val;

	/**
	 * https://www.youtube.com/watch?v=o8NPllzkFhE
	 * [15:40]
	 */
	struct hash_entry **indirect;


	hash_val = ht->func_hash(key);
	idx = hash_val % ht->size;
	indirect = &ht->table[idx];

	while ((*indirect) != NULL) {
		/* check same key exists */
		if (ht->func_key_cmp(key, (*indirect)->key) == 0)
			return -1;
		indirect = &(*indirect)->next;
	}

	if ((*indirect = malloc(sizeof(struct hash_entry))) == NULL)
		return -1;
	(*indirect)->key = key;
	(*indirect)->val = val;
	(*indirect)->next = NULL;

	return 0;
}

void hash_table_destroy(struct hash_table *ht)
{
	uint32_t i;
	struct hash_entry *ent;

	if (ht == NULL)
		return;

	for (i = 0; i < ht->size; i++) {
		ent = ht->table[i];
		while (ent != NULL) {
			/*
			 * TODO: Free key, val properly
			 */
			ent = ent->next;
		}
	}

	free(ht->table);
	free(ht);
	ht = NULL;
}
