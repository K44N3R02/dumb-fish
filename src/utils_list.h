#ifndef DF_UTILS_LIST_H
#define DF_UTILS_LIST_H

#include <stdio.h>
#include <stdlib.h>

#define DECLARE_LIST(name, type)                           \
	struct name##_list {                               \
		int size;                                  \
		int capacity;                              \
		type *array;                               \
	};                                                 \
                                                           \
	void init_##name##_list(struct name##_list *list); \
	void free_##name##_list(struct name##_list *list); \
	void push_##name##_list(struct name##_list *list, type elem);

#define DEFINE_LIST(name, type)                                               \
	void init_##name##_list(struct name##_list *list)                     \
	{                                                                     \
		list->size = 0;                                               \
		list->capacity = 0;                                           \
		list->array = NULL;                                           \
	}                                                                     \
                                                                              \
	void free_##name##_list(struct name##_list *list)                     \
	{                                                                     \
		free(list->array);                                            \
	}                                                                     \
                                                                              \
	void push_##name##_list(struct name##_list *list, type elem)          \
	{                                                                     \
		if (list->size >= list->capacity) {                           \
			type *tmp = list->array;                              \
			list->capacity = (list->capacity == 0) ?              \
						 8 :                          \
						 list->capacity * 2;          \
			list->array = realloc(list->array,                    \
					      list->capacity * sizeof(type)); \
			if (list->array == NULL) {                            \
				fprintf(stderr,                               \
					"[!] push_##name##_list realloc\n");  \
				exit(1);                                      \
			}                                                     \
			free(tmp);                                            \
		}                                                             \
                                                                              \
		list->array[list->size] = elem;                               \
		list->size++;                                                 \
	}

#endif // DF_UTILS_LIST_H
