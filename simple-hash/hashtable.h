#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAILED (int)0
#define SUCCESS (int)1

#if defined(DEBUG)
#  define LOG_MSG printf
#else
#  define LOG_MSG(...)
#endif

#define HASH_TABLE_INIT_SIZE 6

typedef struct _Bucket {
	char *key;
	void *value;
	struct _Bucket *next;
} Bucket;

typedef struct _HashTable {
	int size;
	int elem_num;
	Bucket **buckets;
} HashTable;

int hash_init(HashTable *ht);                               // 初始化哈希表
int hash_lookup(HashTable *ht, char *key, void **result);   // 根据key查找内容
int hash_insert(HashTable *ht, char *key, void *value);     // 将内容插入到哈希表中
int hash_remove(HashTable *ht, char *key);                  // 删除key所指向的内容
int hash_destroy(HashTable *ht);
static void resize_hash_table_if_needed(HashTable *ht);
static int hash_resize(HashTable *ht);
