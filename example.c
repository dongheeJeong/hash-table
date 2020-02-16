#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

#if !defined(__FreeBSD__) || !defined(__OpenBSD__) || !defined(__APPLE__)
#include <bsd/stdlib.h>
#endif

/* http://www.cse.yorku.ca/~oz/hash.html */
uint64_t djb2(const unsigned char *str)
{
    int c;
    unsigned long hash = 5381;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

char* byte_to_charset(unsigned char *buf, size_t len_buf, const char *charset,
					  size_t len_charset)
{
	size_t i;

	for (i = 0; i < len_buf; i++) {
		buf[i] %= len_charset;
		buf[i] = charset[buf[i]];
	}
	return (char*) buf;
}

char* random_str(const char *charset, size_t len_charset,
							size_t len_str)
{
	unsigned char *buf;

	if ((buf = malloc((len_str+1) * sizeof(unsigned char))) == NULL)
		return NULL;

	arc4random_buf(buf, len_str);
	return byte_to_charset(buf, len_str, charset, len_charset);
}

int main(void)
{
	const char charset1[] =	"abcdefghijklmnopqrstuvwxyz"
							"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char charset2[] = "0123456789";
	const uint32_t TABLE_SIZE = 256 + 1;

	/* Init hash table */
	struct hash_table *ht = hash_table_init(TABLE_SIZE);
	ht->func_hash = (uint64_t(*)(const void*)) djb2;
	ht->func_key_cmp = (int(*)(const void*, const void*))strcmp;

	/* Add entries */
	uint32_t cnt = 0;
	uint32_t times = TABLE_SIZE / 2;
	while (times--) {
		char *key = random_str(charset1, sizeof(charset1) - 1, 16);
		char *val = random_str(charset2, sizeof(charset2) - 1, 8);
		if (hash_add(ht, key, val) == 1)
			cnt++;
	}

	/* Print hash table status */
	struct hash_entry *entry;
	for (uint32_t i = 0; i < ht->size; i++) {
		printf("[%u]:", i);
		entry = ht->table[i];

		while (entry != NULL) {
			printf(" -> [key: (%s), val: (%s)]",
					(char*) entry->key, (char*) entry->val);
			entry = entry->next;
		}
		puts("");
	}
	printf("hash_table_size:[%u] / entry:[%u] / collision cnt:[%u](%0.2f%)\n",
			TABLE_SIZE, TABLE_SIZE / 2, cnt, (float)cnt / (TABLE_SIZE/2));

	/* Destroy hash table */
	hash_table_destroy(ht);
	return 0;
}

