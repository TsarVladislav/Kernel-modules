#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define MSG "procmodule"

int len, temp;
char *msg;

static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp )
{
	if (count > temp) {
		count = temp;
	}

	temp -= count;
	printk(KERN_ALERT "Send msg to user: %s (%d)\n", buf, (int)count);
	copy_to_user(buf, msg, count);

	if (count == 0)
		temp = len;

	return count;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {

	printk(KERN_ALERT "Recv msg from user: %s (%d)\n", buf, (int)count);
	strncpy_from_user(msg, buf, count);
	len = count;
	temp = len;
	return count;
}

struct file_operations proc_fops = {
	.read = read_proc,
	.write = write_proc
};

int init_module(void)
{
	proc_create(MSG, 0, NULL, &proc_fops);
	msg = kmalloc(strlen(MSG)* sizeof(char), GFP_KERNEL );
	return 0;
}

void cleanup_module(void) {
	kfree(msg);
	remove_proc_entry(MSG, NULL);
}

MODULE_LICENSE("GPL");

