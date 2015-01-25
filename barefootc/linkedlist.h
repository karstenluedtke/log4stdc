/**
 * @file        linkedlist.h
 * @brief       Macros for maintaining linked lists.
 * @author      Karsten Luedtke
 * @date        2014-06-21
 */

#ifndef _BFC_LINKEDLIST_H_
#define _BFC_LINKEDLIST_H_

#define BFC_LIST_HEAD(elemptrT)					\
	elemptrT	first;					\
	elemptrT	last;

#define BFC_LIST_INIT(l)					\
	(l)->first = (l)->last = NULL

#define BFC_LIST_FIRST(l)	((l)->first)
#define BFC_LIST_EMPTY(l)	((l)->first == NULL)

#define BFC_LIST_FOREACH(v,l,next)					\
	for (v = (l)->first; (v) != NULL; v = (v)->next)

#define BFC_SLIST_INSERT_FIRST(l,e,next)				\
do {									\
	if (((e)->next = (l)->first) != NULL) {				\
		(l)->first = (e);					\
	} else {							\
		(l)->first = (l)->last = (e);				\
	}								\
} while (0 /* just once */)

#define BFC_SLIST_INSERT_AFTER(l,p,e,next)				\
do {									\
	if (((e)->next = (p)->next) == NULL) {				\
		(l)->last = (e);					\
	}								\
	(p)->next = (e);						\
} while (0 /* just once */)

#define BFC_SLIST_INSERT_LAST(l,e,next)					\
do {									\
	(e)->next = NULL;						\
	if ((l)->first && (l)->last) {					\
		(l)->last->next = (e);					\
		(l)->last = (e);					\
	} else {							\
		(l)->first = (l)->last = (e);				\
	}								\
} while (0 /* just once */)

#define BFC_SLIST_APPEND(l,e)	BFC_SLIST_INSERT_LAST(l,e,next)

#define BFC_SLIST_REMOVE_FIRST(l,e,next)				\
	if ((e)->next) {						\
		(l)->first = (e)->next;					\
	} else {							\
		(l)->first = (l)->last = NULL;				\
	}
	
#define BFC_SLIST_REMOVE_AFTER(l,p,e,next)				\
	if ((p->next = (e)->next) == NULL) {				\
		(l)->last = p;						\
	}

#define BFC_SLIST_REMOVE(l,e,elemptrT,next)				\
do {									\
	if ((l)->first == (e)) {					\
		BFC_SLIST_REMOVE_FIRST(l,e,next);			\
	} else {							\
		elemptrT p;						\
		for (p = (l)->first; p; p = p->next) {			\
			if (p == (e)) {					\
				BFC_SLIST_REMOVE_AFTER(l,p,e,next);	\
				break;					\
			}						\
		}							\
	}								\
} while (0 /* just once */)



#define BFC_DLIST_INSERT_FIRST(l,e,next,prev)				\
do {									\
	(e)->prev = (e)->next = NULL;					\
	if ((l)->first) {						\
		(e)->next = (l)->first;					\
		(l)->first->prev = (e);					\
		(l)->first = (e);					\
	} else {							\
		(l)->first = (l)->last = (e);				\
	}								\
} while (0 /* just once */)

#define BFC_DLIST_INSERT_BEFORE(l,e,n,next,prev)			\
do {									\
	(e)->next = (n);						\
	(e)->prev = (n)->prev;						\
	if (((n) != (l)->first) && ((n)->prev != NULL)) {		\
		(n)->prev->next = (e);					\
		(n)->prev = (e);					\
	} else {							\
		(e)->prev = NULL;					\
		(n)->prev = (e);					\
		(l)->first = (e);					\
	}								\
} while (0 /* just once */)

#define BFC_DLIST_INSERT_AFTER(l,p,e,next,prev)				\
do {									\
	(e)->prev = (p);						\
	if (((e)->next = (p)->next) != NULL) {				\
		(p)->next->prev = (e);					\
		(p)->next = (e);					\
	} else {							\
		(p)->next = (e);					\
		(l)->last = (e);					\
	}								\
} while (0 /* just once */)

#define BFC_DLIST_INSERT_LAST(l,e,next,prev)				\
do {									\
	(e)->prev = (e)->next = NULL;					\
	if ((l)->first && (l)->last) {					\
		(e)->prev = (l)->last;					\
		(l)->last->next = (e);					\
		(l)->last = (e);					\
	} else {							\
		(l)->first = (l)->last = (e);				\
	}								\
} while (0 /* just once */)

#define BFC_DLIST_APPEND(l,e)	BFC_DLIST_INSERT_LAST(l,e,next,prev)

#define BFC_DLIST_REMOVE(l,e,next,prev)					\
do {									\
	if ((e)->prev != NULL) {					\
		(e)->prev->next = (e)->next;				\
	} else if ((l)->first == (e)) {					\
		(l)->first = (e)->next;					\
	}								\
	if ((e)->next != NULL) {					\
		(e)->next->prev = (e)->prev;				\
	} else if ((l)->last == (e)) {					\
		(l)->last = (e)->prev;					\
	}								\
	(e)->prev = (e)->next = NULL;					\
} while (0 /* just once */)

#endif /* _BFC_LINKEDLIST_H_ */
