#include "hashtable.h"

static int hash_str(char *key){/*{{{*/
	int hash = 0;
	char *cur = key;
	while (*cur != '\0'){
		hash += *cur;
		++cur;
	}
	return hash;
}/*}}}*/

// 使用这个宏来求得key在哈希表中的索引
#define HASH_INDEX(ht, key) (hash_str((key)) % (ht)->size)

int hash_init(HashTable *ht){ /*{{{*/
	ht->size = HASH_TABLE_INIT_SIZE;
	ht->elem_num = 0;
	ht->buckets = (Bucket **)calloc(ht->size, sizeof(Bucket*));
	if (NULL == ht->buckets){
		return FAILED;
	}
	return SUCCESS;
}/*}}}*/

int hash_insert(HashTable *ht, char *key, void *value){/*{{{*/
	// check if we need to resize the hashtable
	resize_hash_table_if_needed(ht);
	int index = HASH_INDEX(ht, key);
	Bucket *org_bucket = ht->buckets[index];
	Bucket *tmp_bucket = org_bucket;
	// check if the key exits already
	while (tmp_bucket){
		if (0 == strcmp(key, tmp_bucket->key)){
			LOG_MSG("[update]\tkey: %s\n", key);
			tmp_bucket->value = value;
			return SUCCESS;
		}
		tmp_bucket = tmp_bucket->next;
	}
	Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
	if (NULL == bucket){
		LOG_MSG("[error]\tmalloc error\n");
		return FAILED;
	}
	bucket->key = key;
	bucket->value = value;
	bucket->next = NULL;
	ht->elem_num += 1;

	if (NULL != org_bucket){
		LOG_MSG("[collision]\tindex: %d key: %d", index, key);
		bucket->next = org_bucket;
	}

	ht->buckets[index] = bucket;
	LOG_MSG("[insert]\tindex:%d key:%s\tht(num:%d)\n", index, key, ht->elem_num);

	return SUCCESS;
} /*}}}*/

/*
 *插入过程中可能会导致哈希表的元素个数比较多，如果超过了哈希表的容量， 则说明肯定会出现碰撞，出现碰撞则会导致哈希表的性能下降，为此如果出现元素容量达到容量则需要进行扩容
 * */
static void resize_hash_table_if_needed(HashTable *ht){/*{{{*/
	if (ht->size - ht->elem_num < 1){
		hash_resize(ht);
	}
}/*}}}*/

static int hash_resize(HashTable *ht){/*{{{*/
	//double size
	int org_size = ht->size;
	ht->size = ht->size * 2;
	ht->elem_num = 0;
	LOG_MSG("[resize]\torg size: %i\tnew size: %i\n", org_size, ht->size);

	Bucket **buckets = (Bucket **)calloc(ht->size, sizeof(Bucket *));
	Bucket **org_buckets = ht->buckets;
	ht->buckets = buckets;
	int i = 0;
	for (; i < org_size; i++){
		Bucket *cur = org_buckets[i];
		Bucket *tmp;
		while (cur){
			hash_insert(ht, cur->key, cur->value);
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(org_buckets);
	LOG_MSG("[resize]\t done\n");
	return SUCCESS;
}/*}}}*/

/*
 * 查找元素
 * */
int hash_lookup(HashTable *ht, char *key, void **result){/*{{{*/
	int index = HASH_INDEX(ht, key);
	Bucket *bucket = ht->buckets[index];
	while (bucket){
		if (0 == strcmp(bucket->key, key)){
			LOG_MSG("[lookup]\t found %s\tindex:%i value: %p\n",  key, index, bucket->value);
			*result = bucket->value;
			return SUCCESS;
		}
		bucket = bucket->next;
	}
failed:
	LOG_MSG("[lookup]\t key:%s\tfailed\t\n", key);
	return FAILED;
}/*}}}*/

int hash_remove(HashTable *ht, char *key){/*{{{*/
	int index = HASH_INDEX(ht, key);
	Bucket *bucket  = ht->buckets[index];
	Bucket *prev	= NULL;

	if(bucket == NULL) return FAILED;
	while (bucket){
		if (0 == strcmp(bucket->key, key)){
			LOG_MSG("[remove]\tkey:(%s) index: %d\n", key, index);
			if (NULL == prev){
				ht->buckets[index] = bucket->next;
			} else {
				prev->next = bucket->next;
			}
			free(bucket);
			return SUCCESS;
		}
		prev = bucket;
		bucket = bucket->next;
	}
	LOG_MSG("[remove]\t key:%s not found remove \tfailed\t\n", key);
	return FAILED;
}/*}}}*/

int hash_destroy(HashTable *ht){/*{{{*/
	Bucket *cur = NULL;
	Bucket *tmp = NULL;
	int i = 0;
	for (; i < ht->size; i++){
		cur = ht->buckets[i];
		while (cur){
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(ht->buckets);
	return SUCCESS;
}/*}}}*/
