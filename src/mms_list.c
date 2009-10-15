/*
 * File:    mms_list.c
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   
 *
 * Copyright (c) 2006  topwise, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2006-12-13 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include "mms_list.h"

typedef struct _ListNode
{
	struct _ListNode* prev;
	struct _ListNode* next;
	void* data;
}ListNode;

struct _MmsList
{
	ListNode* head;
	ListNode* tail;
	mms_destroy_func destroy;
};

static void mms_list_default_destroy(void* data)
{
	return;
}

static int mms_list_default_compare(void* data1, void* data2)
{
	return (int)data1 - (int)data2;
}

MmsList* mms_list_create(mms_destroy_func destroy)
{
	MmsList* thiz = (MmsList*)mms_calloc(1, sizeof(MmsList));

	if(thiz != NULL)
	{
		thiz->destroy = destroy != NULL ? destroy : mms_list_default_destroy;
	}

	return thiz;
}

void     mms_list_destroy(MmsList* thiz)
{
	if(thiz != NULL)
	{
		ListNode* iter = NULL;

		while(thiz->head != NULL)
		{
			iter = thiz->head;
			thiz->head = thiz->head->next;
			thiz->destroy(iter->data);
			mms_free(iter);
		}

		thiz->head = thiz->tail = NULL;

		mms_free(thiz);
	}

	return;
}

MmsResult mms_list_prepend(MmsList* thiz, void* data)
{
	ListNode* node = NULL;

	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	node = (ListNode*)mms_calloc(1, sizeof(ListNode));

	mms_return_val_if_fail(node != NULL, MMS_RESULT_FAIL);

	node->data = data;
	
	if(thiz->head == NULL)
	{
		thiz->head = thiz->tail = node;
	}
	else
	{
		node->next = thiz->head;
		thiz->head->prev = node;
		thiz->head = node;
	}

	return MMS_RESULT_OK;
}

MmsResult mms_list_append(MmsList* thiz, void* data)
{
	ListNode* node = NULL;

	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	node = (ListNode*)mms_calloc(1, sizeof(ListNode));

	mms_return_val_if_fail(node != NULL, MMS_RESULT_FAIL);

	node->data = data;

	if(thiz->tail == NULL)
	{
		thiz->head = thiz->tail = node;
	}
	else
	{
		thiz->tail->next = node;
		node->prev = thiz->tail;
		thiz->tail = node;
	}

	return MMS_RESULT_OK;
}

MmsResult mms_list_remove(MmsList* thiz, mms_compare_func compare, void* data, void** ret_data)
{
	ListNode* iter = NULL;
	mms_return_val_if_fail(thiz != NULL, MMS_RESULT_FAIL);

	if(compare == NULL)
	{
		compare = mms_list_default_compare;
	}

	iter = thiz->head;

	while(iter != NULL)
	{
		if(compare(iter->data, data) == 0)
		{
			if(ret_data != NULL)
			{
				*ret_data = iter->data;
			}
			else
			{
				thiz->destroy(iter->data);	
			}

			if(iter->prev != NULL)
			{
				iter->prev->next = iter->next;
			}
			else
			{
				thiz->head = iter->next;
			}
			
			if(iter->next != NULL)
			{
				iter->next->prev = iter->prev;
			}
			else
			{
				thiz->tail = iter->prev;
			}

			iter->next = NULL;
			iter->prev = NULL;

			mms_free(iter);

			return MMS_RESULT_OK;
		}

		if(iter == thiz->tail)
		{
			break;
		}
		else
		{
			iter = iter->next;
		}
	}

	return MMS_RESULT_FAIL;
}

MmsResult mms_list_erase(MmsList* thiz, mms_compare_func compare, void* data)
{
	return mms_list_remove(thiz, compare, data, NULL);
}


size_t   mms_list_length(MmsList* thiz)
{
	size_t count = 0;
	ListNode* iter = NULL;
	
	mms_return_val_if_fail(thiz != NULL, 0);
	
	iter = thiz->head;
	while(iter != NULL)
	{
		count++;

		if(iter == thiz->tail)
		{
			break;
		}
		else
		{
			iter = iter->next;
		}
	}

	return count;
}

void*    mms_list_nth(MmsList* thiz, size_t index)
{
	size_t i = 0;
	ListNode* iter = NULL;
	
	mms_return_val_if_fail(thiz != NULL, NULL);
	
	iter = thiz->head;
	while(iter != NULL)
	{
		if(i == index)
		{
			return iter->data;
		}
		i++;

		if(iter == thiz->tail)
		{
			break;
		}
		else
		{
			iter = iter->next;
		}
	}

	return NULL;
}

void*     mms_list_first(MmsList* thiz)
{
	mms_return_val_if_fail(thiz != NULL && thiz->head != NULL, NULL);

	return thiz->head->data;
}

void*     mms_list_last(MmsList* thiz)
{
	mms_return_val_if_fail(thiz != NULL && thiz->tail != NULL, NULL);

	return thiz->tail->data;
}

void*    mms_list_find(MmsList* thiz, mms_compare_func compare, void* data)
{
	ListNode* iter = NULL;
	
	mms_return_val_if_fail(thiz != NULL, NULL);

	if(compare == NULL)
	{
		compare = mms_list_default_compare;
	}
	
	iter = thiz->head;
	while(iter != NULL)
	{
		if(compare(iter->data, data) == 0)
		{
			return iter->data;
		}
		
		if(iter == thiz->tail)
		{
			break;
		}
		else
		{
			iter = iter->next;
		}
	}

	return NULL;
}

MmsResult mms_list_foreach(MmsList* thiz, mms_visit_func visit, void* context)
{
	ListNode* iter = NULL;
	
	mms_return_val_if_fail(thiz != NULL && visit != NULL, MMS_RESULT_FAIL);

	iter = thiz->head;
	while(iter != NULL)
	{
		if(visit(iter->data, context) != MMS_RESULT_OK)
		{
			break;
		}

		if(iter == thiz->tail)
		{
			break;
		}
		else
		{
			iter = iter->next;
		}
	}

	return MMS_RESULT_OK;
}


#ifdef MMS_LIST_TEST
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 2000;
	int ret = 0;
	MmsList* thiz = mms_list_create(NULL);

	for(i = 0; i < n; i++)
	{
		mms_list_append(thiz, (void*)i);
		mms_assert(mms_list_length(thiz) == (i + 1));
		mms_assert((int)mms_list_nth(thiz, i) == i);
	}
	
	for(i = 0; i < n; i++)
	{
		mms_assert(mms_list_length(thiz) == (n - i));
		mms_assert((int)mms_list_nth(thiz, 0) == i);
		mms_list_erase(thiz, NULL, (void*)i);
	}
	
	for(i = 0; i < n; i++)
	{
		mms_list_prepend(thiz, (void*)i);
		mms_assert(mms_list_length(thiz) == (i + 1));
		mms_assert((int)mms_list_nth(thiz, 0) == i);
	}
	
	for(i = 0; i < n; i++)
	{
		mms_assert(mms_list_length(thiz) == (n - i));
		mms_list_remove(thiz, NULL, (void*)i, (void**)&ret);
		mms_assert(ret == i);
	}
	
	mms_list_destroy(thiz);

	return 0;
}
#endif/*MMS_LIST_TEST*/

