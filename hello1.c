// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Artem Saveliev <saveliev.artem@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training - Module 1");
MODULE_LICENSE("Dual BSD/GPL");

struct print_time_data {
	struct list_head list;
	ktime_t before;
	ktime_t after;
};

static LIST_HEAD(print_time_list);

void print_hello(void)
{
	struct print_time_data *data = kmalloc(sizeof(*data), GFP_KERNEL);

	if (!data)
		return;

	data->before = ktime_get();
	pr_info("Hello, world!\n");
	data->after = ktime_get();

	list_add(&data->list, &print_time_list);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
	struct print_time_data  *data, *tmp;
	int count = 0;

	list_for_each_entry_safe(data, tmp, &print_time_list, list) {
		pr_info("print %d took %lld ns\n",
			count++,
			ktime_to_ns(ktime_sub(data->after, data->before)));
		list_del(&data->list);
		kfree(data);
	}
}

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

module_init(hello1_init);
module_exit(hello1_exit);
