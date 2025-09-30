#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#define BUFLEN 1024

struct my_node_t{
	unsigned long key;
	struct rb_node node_;
};
struct rb_root my_tree = RB_ROOT;

static char print_buffer[BUFLEN];
static struct kparam_string print_string = {
	.maxlen = BUFLEN,
	.string = print_buffer
};
//---------------------------------------
// Print
//---------------------------------------
static int pget_print(char* buffer, const struct kernel_param* kp) {
	char* iter = print_buffer;
	struct rb_node* node;
	int i = 100;
	bool first = true;
	memset(print_buffer, 0, BUFLEN);
	iter += snprintf(iter, 10, "tree: {");
	for (node = rb_first(&my_tree); node && i-- > 0; node = rb_next(node)) {
		struct my_node_t *ptr = rb_entry(node, struct my_node_t, node_);
		if (!first) {
			*iter++ = ',';
			*iter++ = ' ';
		}
		first = false;
		iter += snprintf(iter, 250, "%ld", ptr->key);
	}
	*iter++ = '}';
	*iter++ = '\n';
	pr_info("%s", print_buffer);
	return param_get_string(buffer, kp);
}
static const struct kernel_param_ops pops_print = {
	.set = NULL,
	.get = pget_print,
};
module_param_cb(print, &pops_print, &print_string, S_IRUSR|S_IRGRP);
MODULE_PARM_DESC(print, "print tree");

//---------------------------------------
// Insert 
//---------------------------------------
void insert_node (int key) {
	struct my_node_t *data = kmalloc(sizeof(struct my_node_t), GFP_USER);
	struct rb_node **new = &my_tree.rb_node, *parent = NULL;
	memset(data, 0, sizeof(struct my_node_t));
	data->key = key;
	while (*new) {
		struct my_node_t *this = rb_entry(*new, struct my_node_t, node_);
		parent = *new;
		if (key <this->key) {
			new = &(*new)->rb_left;		
		}
		else {
			new = &(*new)->rb_right;
		}
	}
	rb_link_node(&data->node_, parent, new);
	rb_insert_color(&data->node_, &my_tree);
}

static int pset_insert(const char *val, const struct kernel_param *kp) {
	int res;
	int new_key;
	res = kstrtoint(val, 10, &new_key);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	insert_node(new_key);
	pr_info("inserting key %d\n", new_key);
	return res;
} 
static const struct kernel_param_ops pops_insert = {
	.set = pset_insert,
	.get = NULL,
};
module_param_cb(insert, &pops_insert, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(insert, "insert value in tree");

//---------------------------------------
// Remove
//---------------------------------------
struct my_node_t* search (int key) {
	struct rb_node *node = my_tree.rb_node;
	while (node) {
		struct my_node_t* ptr = rb_entry(node, struct my_node_t, node_);
		if (ptr->key == key) {
			return ptr;
		}
		if (ptr->key < key) {
			node = node->rb_right;
		}
		else {
			node = node->rb_left;
		}
	}
	return NULL;
}

bool remove_key (int key) {
	struct my_node_t* node = search(key);
	if (node == NULL) {
		return false;
	}
	rb_erase(&node->node_, &my_tree);
	kfree(node);
	return true;
}

static int pset_remove(const char *val, const struct kernel_param *kp) {
	int res;
	int old_key;
	res = kstrtoint(val, 10, &old_key);
	if (res) {
		pr_err("Parameter \"%s\" must be int(%d)\n", val, res);
		return res;
	}
	if (!remove_key(old_key)) {
		pr_info("key %d not found\n", old_key);
	}
	else {
		pr_info("removing key %d\n", old_key);
	}
	return strlen(val);
}
static const struct kernel_param_ops pops_remove = {
	.set = pset_remove,
	.get = NULL,
};
module_param_cb(remove, &pops_remove, NULL, S_IWUSR|S_IWGRP);
MODULE_PARM_DESC(remove, "trying remove element from tree");

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
	while (my_tree.rb_node) {
		struct my_node_t *ptr = rb_entry(my_tree.rb_node, struct my_node_t, node_); 
		remove_key(ptr->key);
	}
	pr_info("unloaded\n");
}
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kesha");
MODULE_DESCRIPTION("An example of using rbtree in kernel");
