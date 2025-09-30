#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#define BITMAP_LEN 128
#define BUFLEN 1024
static unsigned long* bitmap_buff;
static char print_buffer[BUFLEN];
static struct kparam_string print_string = {
	.maxlen = BUFLEN,
	.string = print_buffer
};
//---------------------------------------
// Print
//---------------------------------------
static int pget_print(char* buffer, const struct kernel_param* kp) {
	int cnt;
	char* iter = print_buffer;
	pr_info("0b");
	memset(print_buffer, 0, BUFLEN);
	*iter++ = '0';
	*iter++ = 'b';
	for (cnt = 0; cnt < BITMAP_LEN; cnt++) {
		char val = '0' + test_bit(cnt, bitmap_buff);
		printk(KERN_CONT "%c", val);
		*iter++ = val;
	}
	printk(KERN_CONT "\n");
	*iter++ = '\n';
	return param_get_string(buffer, kp);
}
static const struct kernel_param_ops pops_print = {
	.set = NULL,
	.get = pget_print,
};
module_param_cb(print, &pops_print, &print_string, S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(print, "print bitmap");

//---------------------------------------
// Set
//---------------------------------------
static int pset_set(const char *val, const struct kernel_param *kp) {
	int res;
	int new_bit;
	res = kstrtoint(val, 10, &new_bit);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	if (new_bit < 0 || new_bit > BITMAP_LEN) { 
		pr_err("Parameter \"%s\" out of range\n", val);
		return -EINVAL;
	}
	set_bit(new_bit, bitmap_buff);
	pr_info("set bit %d\n", new_bit);
	return strlen(val);
} 
static const struct kernel_param_ops pops_set = {
	.set = pset_set,
	.get = NULL,
};
module_param_cb(set, &pops_set, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(set, "Set nth bit in bitmap");

//---------------------------------------
// Clear
//---------------------------------------
static int pset_clear(const char *val, const struct kernel_param *kp) {
	int res;
	int old_bit;
	res = kstrtoint(val, 10, &old_bit);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	if (old_bit < 0 || old_bit > BITMAP_LEN) {
		pr_err("Parameter \"%s\" out of range\n", val);
		return res;
	}
	clear_bit(old_bit, bitmap_buff);
	pr_info("clear bit %d\n", old_bit);
	return strlen(val);
}
static const struct kernel_param_ops pops_clear = {
	.set = pset_clear,
	.get = NULL,
};
module_param_cb(clear, &pops_clear, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(clear, "Remove element in the end of queue");

//---------------------------------------
// init
//---------------------------------------
static int __init mod_init(void) {
	bitmap_buff = bitmap_zalloc(BITMAP_LEN, GFP_USER);
	pr_info("loaded\n");
	return 0;
}
module_init(mod_init);

//---------------------------------------
// exit
//---------------------------------------
static void __exit mod_exit(void) {
	bitmap_free(bitmap_buff);
	pr_info("unloaded\n");
}
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kesha");
MODULE_DESCRIPTION("An example of using bitmat in kernel");
