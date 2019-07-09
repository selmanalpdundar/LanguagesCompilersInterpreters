/**
 * @file utils.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdlib.h>
#include <string.h>

/**
 * @brief 
 * 
 * @param s 
 * @return size_t 
 */
static size_t hash(const char *s) {
  size_t r = 0xcbf29ce484222325;
  while (*s) {
    r *= 0x100000001b3;
    r ^= *s;
    s++;
  }

  return r;
}

/**
 * @brief 
 * It is vector that can take any kind of data.
 */
struct vector {
  size_t capacity;
  void **data;
};


/**
 * @brief Test
 *  It initiates a vector with capacity 16
 * @param v 
 */
void vector_init(struct vector *v) {
  v->capacity = 16;
  v->data = calloc(v->capacity, sizeof(v->data[0]));
}

/**
 * @brief 
 * It takes a vector and try to grow it. If the capatity of vector bigger then asked one, it does not do anything.
 * It grows capacity of vector and assaign all them null and update capacity.
 * @param v is a vector
 * @param n is a size that we want to resize vector.
 */
void vector_grow(struct vector *v, size_t n) {
  if (n < v->capacity) {
    return;
  }
  v->data = realloc(v->data, n * sizeof(v->data[0]));
  for (size_t i = v->capacity; i < n; i++) {
    v->data[0] = NULL;
  }
  v->capacity = n;
}

/**
 * @brief 
 * It takes a vector and free it from memory.
 * @param v is a vector.
 */

void vector_fini(struct vector *v) {
  free(v->data);
}


/**
 * @brief 
 * It return an element of vector 
 * @param v is an vector.
 * @param idx is an id of the asked element.
 * @return void* 
 */
void *vector_get(struct vector *v, size_t idx) {
  vector_grow(v, idx + 1); // it is uncesseray because we do not put new element just get it from vector.
  return v->data[idx]; 
}

/**
 * @brief 
 *  It takes a data to put it given index.
 * @param v is a vector.
 * @param idx  is an index of element.
 * @param x is a data or given index.
 */
void vector_set(struct vector *v, size_t idx, void *x) {
  vector_grow(v, idx + 1); // This is correct because instead of increas id in vector_get we should do it when put new one. 
  v->data[idx] = x;
}


/**
 * @brief 
 * String initilization. 
 */
struct string_int {
  struct vector rev;
  size_t count;
  size_t capacity;
  struct kv {
    char *key;
    int id;
  } *data;
};



/**
 * @brief 
 * 
 * @param v 
 */
void string_int_init(struct string_int *v) {
  vector_init(&v->rev);
  v->count = 0;
  v->capacity = 16;
  v->data = calloc(v->capacity, sizeof(v->data[0]));
}

/**
 * @brief 
 * 
 * @param v 
 */
void string_int_fini(struct string_int *v) {
  vector_fini(&v->rev);
  for (size_t i = 0; i < v->capacity; i++) {
    free(v->data[i].key);
  }

  free(v->data);
}

/**
 * @brief 
 * 
 * @param v 
 * @param n 
 */
void string_int_resize(struct string_int *v, size_t n) {
  struct kv *newdata = calloc(n, sizeof(v->data[0]));

  for (size_t i = 0; i < v->capacity; i++) {
    char *key = v->data[i].key;
    if (key) {
      size_t idx = hash(key);
      while (newdata[idx % n].key) {
        idx++;
      }
      newdata[idx % n].key = v->data[i].key;
      newdata[idx % n].id = v->data[i].id;
    }
  }
  free(v->data);
  v->data = newdata;
  v->capacity = n;
}

/**
 * @brief 
 * 
 * @param v 
 * @param key 
 * @return size_t 
 */
size_t string_int_get(struct string_int *v, const char *key) {
  size_t idx;

  if (2 * v->count >= v->capacity) {
    string_int_resize(v, 2 * v->count);
  }

  for (idx = hash(key) % v->capacity; v->data[idx].key; idx = (idx + 1) % v->capacity) {
    if (!strcmp(v->data[idx].key, key)) {
      return v->data[idx].id;
    }
  }

  size_t id = v->count++;
  char *k = strdup(key);

  v->data[idx].id = id;
  v->data[idx].key = k;
  vector_set(&v->rev, id, k);

  return id;
}

/**
 * @brief 
 * 
 * @param v 
 * @param id 
 * @return const char* 
 */
const char *string_int_rev(struct string_int *v, size_t id) {
  return vector_get(&v->rev, id);
}

/**
 * @brief 
 * 
 */
struct string_int global_ids;

/**
 * @brief 
 * 
 */
struct vector global_types;
