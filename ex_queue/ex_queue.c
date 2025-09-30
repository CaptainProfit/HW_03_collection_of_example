#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

#include <linux/kfifo.h>
#include <linux/slab.h>

#define BUFLEN 1024

DECLARE_KFIFO (my_fifo, int, 16);
static int mock_add;
static char remove_buffer[BUFLEN];
static struct kparam_string remove_string = {
	.maxlen	= BUFLEN,
	.string	= remove_buffer
};
//---------------------------------------
// Print
//---------------------------------------
static int Print(void) {
	bool first = true;
	int value;
	pr_info("{");
	while (!kfifo_is_empty(&my_fifo)) {
		int __maybe_unused res = kfifo_out(&my_fifo, &value, 1);
		if (!first) {
			printk(KERN_CONT ", ");
		}
		first = false;
		printk(KERN_CONT "%d", value);
	}
	printk(KERN_CONT "}\n");
	return 0;
}

//---------------------------------------
// Add
//---------------------------------------
static int Add(int value) {
	if (kfifo_is_full(&my_fifo)) {
		pr_warn("cant add, queue is full\n");
		return -1;
	}
	return kfifo_in(&my_fifo, &value, 1);
}

static int pset_add(const char *val, const struct kernel_param *kp) {
	int res;
	int new_value;
	int add_res;
	res = kstrtoint(val, 10, &new_value);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	add_res = Add(new_value);
	if (add_res > 0) {
		pr_info("added %d\n", new_value);
	}
	return strlen(val);
}
static const struct kernel_param_ops pops_add = {
	.set = pset_add,
	.get = NULL,
};
module_param_cb(add, &pops_add, &mock_add, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(add, "Add element in queue");

//---------------------------------------
// Remove
//---------------------------------------
static int Remove(int* old_value) {
	if(kfifo_is_empty(&my_fifo)) {
		pr_warn("empty queue, cant remove\n");
		return -1;
	}
	return kfifo_out(&my_fifo, old_value, 1); 
}

static int pget_remove(char *buffer, const struct kernel_param *kp) {
	int res;
	int old_value;
	res = Remove(&old_value);
	memset(remove_buffer, 0, BUFLEN);
	if (res > 0) {
		pr_info("removed %d\n", old_value);
		snprintf(remove_buffer, BUFLEN, "%d", old_value);
	}
	return param_get_string(buffer, kp);
}
static const struct kernel_param_ops pops_remove = {
	.set = NULL,
	.get = pget_remove
};
module_param_cb(remove, &pops_remove, &remove_string, S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(remove, "Remove element in the end of queue");

//---------------------------------------
// init
//---------------------------------------
static int __init mod_init(void) {
	INIT_KFIFO(my_fifo);
	pr_info("loaded\n");
	return 0;
}
module_init(mod_init);

//---------------------------------------
// exit
//---------------------------------------
static void __exit mod_exit(void) {
	Print();
	pr_info("unloaded\n");
}
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kesha");
MODULE_DESCRIPTION("An example of using queue in kernel");
