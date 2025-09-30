#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/bsearch.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/sort.h>
#define BUFLEN 1024

struct my_struct {
	u32 val;
};


#define NR_VAL 1000

static struct my_struct arr[NR_VAL];

static char find_buffer[BUFLEN];
static struct kparam_string find_string = {
	.maxlen = BUFLEN,
	.string = find_buffer
};

static void init_arr(void)
{
	int i;

	for (i = 0; i < NR_VAL; ++i) {
		get_random_bytes(&arr[i].val, sizeof(u32));
		arr[i].val = arr[i].val % NR_VAL;
	}
}

static void fn_swap_arr(void *a, void *b, int size)
{
	u32 tmp;
	struct my_struct *struct_a, *struct_b;
	struct_a = (struct my_struct *)a;
	struct_b = (struct my_struct *)b;

	tmp = struct_b->val;
	struct_b->val = struct_a->val;
	struct_a->val = tmp;
}

static int fn_cmp_arr(const void *a, const void *b)
{
	struct my_struct *struct_a, *struct_b;

	struct_a = (struct my_struct *)a;
	struct_b = (struct my_struct *)b;
	return (int)(struct_a->val - struct_b->val);
}

static struct my_struct* srh_in_arr(struct my_struct *target)
{
	struct my_struct *result = NULL;
	result = bsearch(target, arr, NR_VAL, sizeof(struct my_struct),
			 fn_cmp_arr);
	return result;
}

//---------------------------------------
// find
//---------------------------------------
static int pset_find(const char *val, const struct kernel_param *kp) {
	int res;
	struct my_struct target, *ptr;
	res = kstrtoint(val, 10, &target.val);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	pr_info("search %d\n", target.val);
	ptr = srh_in_arr(&target); 
	memset(find_buffer, 0, BUFLEN);

	if (ptr) {
		int pos = ptr - arr;
		snprintf(find_buffer, BUFLEN, "%u found in pos %d\n", target.val, pos);

	}
	else {
		snprintf(find_buffer, BUFLEN, "%u not found\n", target.val);
	}
	pr_info("%s", find_buffer);
	
	return strlen(val);
}
static int pget_find(char *buffer, const struct kernel_param *kp) {
	return param_get_string(buffer, kp);
}
static const struct kernel_param_ops pops_find = {
	.set = pset_find,
	.get = pget_find,
};
module_param_cb(find, &pops_find, &find_string, S_IWUSR|S_IWGRP|S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(find, "Remove element in the end of queue");

//---------------------------------------
// init
//---------------------------------------
static int __init my_module_init(void)
{
	int i;
	bool first = true;
	pr_info("start\n");

	init_arr();
	for (i = 0; i < NR_VAL; ++i) {
		if (!first) {	
			printk(KERN_CONT ", ");
		}
		first = false;
		printk(KERN_CONT "%u", arr[i].val);
	}

	pr_info("sorting\n");
	sort((void *)arr, NR_VAL, sizeof(struct my_struct), fn_cmp_arr,
	     fn_swap_arr);

	pr_info("[");
	first = true;
	for (i = 0; i < NR_VAL; ++i) {
		if (!first) {	
			printk(KERN_CONT ", ");
		}
		first = false;
		printk(KERN_CONT "%u", arr[i].val);
	}
	printk(KERN_CONT "]\n");
	return 0;
}
module_init(my_module_init);

//---------------------------------------
// exit
//---------------------------------------
static void __exit my_module_exit(void)
{
	pr_info("end\n");
}
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KGD");
MODULE_DESCRIPTION("A kernel module that sorts array using the kernel's sort function");
