/**
 * @file: kernel/lib/list.h
 * @author: LinhengXilan
 * @data: 2025-7-29
 * @version: build11
 **/

#ifndef _KERNEL_LIB_LIST_H_
#define _KERNEL_LIB_LIST_H_

struct List
{
	struct List* prev;
	struct List* next;
};

inline void list_init(struct List* list)
{
	list->prev = list;
	list->next = list;
}

inline struct List* list_next(struct List* list)
{
	if (list->next != nullptr)
	{
		return list->next;
	}
	return nullptr;
}

inline struct List* list_prev(struct List* list)
{
	if (list->prev != nullptr)
	{
		return list->prev;
	}
	return nullptr;
}

inline void list_push_front(struct List* list, struct List* node)
{
	node->next = list;
	list->prev->next = node;
	node->prev = list->prev;
	list->prev = node;
}

inline void list_push_back(struct List* list, struct List* node)
{
	node->next = list->next;
	node->prev = list;
	list->next->prev = node;
	list->next = node;
}

#endif