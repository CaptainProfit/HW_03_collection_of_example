#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

#include <linux/list.h>
#include <linux/slab.h>

#define BUFLEN 1024
struct my_list_t {
	struct list_head list_;
	int value;
};
LIST_HEAD(my_list);
static struct list_head* pointer = &my_list;
static char list_buffer[BUFLEN];
static char remove_buffer[BUFLEN];
static struct kparam_string print_string = {
	.maxlen	= BUFLEN,
	.string	= list_buffer
};
static struct kparam_string remove_string = {
	.maxlen	= BUFLEN,
	.string	= remove_buffer
};

//---------------------------------------
// Print
//---------------------------------------
static int pget_print(char *buffer, const struct kernel_param *kp) {
	bool first = true;
	struct my_list_t *entry;
	char* iter = list_buffer;
	pr_info("forming list to file\n");
	*iter++ = '{';
	if (pointer == &my_list) {
		*iter++ = '[';
		*iter++ = ']';
		first = false;
	}
	list_for_each_entry(entry, &my_list, list_) {
		if (!first) {
			*iter++ = ',';
			*iter++ = ' ';
		}
		first = false;
		if (pointer == &entry->list_) {
			*iter++ = '[';
		}
		iter += snprintf(iter, (250 - (iter - list_buffer)), "%d", entry->value);
		if (pointer == &entry->list_) {
			*iter++ = ']';
		}
		if (iter - list_buffer > 240) {
			break;
		}
	}
	*iter++ = '}';
	*iter++ = '\n';
	*iter++ = '\0';
	pr_info("%s", list_buffer);
	return param_get_string(buffer, kp);
}
static const struct kernel_param_ops pops_print = {
	.set = NULL, //pset_print,
	.get = pget_print,
};
module_param_cb(print, &pops_print, &print_string, S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(print, "Prints list");

//---------------------------------------
// Add
//---------------------------------------
static int Add(int value) {
	struct my_list_t* new_node = kmalloc(sizeof(struct my_list_t), GFP_USER);
	if (new_node == NULL) {
		pr_err("Cant alloc memory\n");
		return -1;
	}
	new_node->value = value;
	list_add(&new_node->list_, pointer);
	return 0;
}

static int pset_add(const char *val, const struct kernel_param *kp) {
	int res;
	int new_value;
	res = kstrtoint(val, 10, &new_value);
	if (res) {
		pr_err("Parameter must be int\n");
		return res;
	}
	res = Add(new_value);
	if (res == 0) {
		pr_info("added %d\n", new_value);
	}
	return res;
}
static const struct kernel_param_ops pops_add = {
	.set = pset_add,
	.get = NULL,
};
module_param_cb(add, &pops_add, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(add, "Insert element pointed");

//---------------------------------------
// Remove
//---------------------------------------
static int Remove(int* old_value) {
	struct my_list_t* old_node;
	if(list_empty(&my_list)) {
		return -1;
	}

	if (pointer == &my_list) {
		old_node = container_of(my_list.next, struct my_list_t, list_);
	}
	else {
		old_node = container_of(pointer, struct my_list_t, list_);
		if (pointer->next != &my_list) {
			pointer = pointer->next;
		}
		else {
			pointer = pointer->prev;
		}
	}
	list_del(&old_node->list_);
	if (old_value != NULL) {
		*old_value = old_node->value;
	}
	kfree(old_node);
	return 0;
}

static int pget_remove(char *buffer, const struct kernel_param *kp) {
	int res;
	int old_value;
	memset(remove_buffer, 0, BUFLEN);
	res = Remove(&old_value);
	if (res == 0) {
		pr_info("removed %d\n", old_value);
		snprintf(buffer, BUFLEN, "%d\n", old_value);
	}
	return param_get_string(remove_buffer, kp);
}
static const struct kernel_param_ops pops_remove = {
	.set = NULL, //pset_remove,
	.get = pget_remove//NULL,
};
module_param_cb(remove, &pops_remove, &remove_string, S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(remove, "Remove element after pointer");

//---------------------------------------
// Next
//---------------------------------------
static int pset_next(const char *val, const struct kernel_param *kp) {
	int value;
	pointer = pointer->next;

	if (pointer == &my_list) {
		pr_info("pointer on list head, no value\n");
	}
	else {
		value = container_of(pointer, struct my_list_t, list_)->value;
		pr_info("pointer on %d\n", value);
	}
	return 0;
}
static const struct kernel_param_ops pops_next = {
	.set = pset_next,
	.get = NULL,
};
module_param_cb(next, &pops_next, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(next, "Move pointer to next element");

///---------------------------------------
// Prev
//---------------------------------------
static int pset_prev(const char *val, const struct kernel_param *kp) {
	int value;
	pointer = pointer->prev;

	if (pointer == &my_list) {
		pr_info("pointer on list head, no value\n");
	}
	else {
		value = container_of(pointer, struct my_list_t, list_)->value;
		pr_info("pointer on %d\n", value);
	}
	return 0;
}
static const struct kernel_param_ops pops_prev = {
	.set = pset_prev,
	.get = NULL,
};
module_param_cb(prev, &pops_prev, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(prev, "Move pointer to prev element");

//---------------------------------------
// init
//---------------------------------------
static int __init mod_init(void) {
	pr_info("loaded\n");
	return 0;
}
module_init(mod_init);

//---------------------------------------
// exit
//---------------------------------------
static void __exit mod_exit(void) {
	int res;
	do {
		res = Remove(NULL);
	} while (res == 0);
	pointer = &my_list;
	pr_info("unloaded\n");
}
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kesha");
MODULE_DESCRIPTION("An example of using list in kernel");
