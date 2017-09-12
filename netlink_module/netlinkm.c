
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#define MY_NETLINK 17

MODULE_LICENSE("GPL");

static struct sock *nl_sk = NULL;

static void nl_data_ready (struct sk_buff *skb)
{
	struct nlmsghdr *nlh;

	nlh = (struct nlmsghdr *)skb->data;
	printk(KERN_INFO "%s: received netlink message payload: %s\n",
	       __FUNCTION__, NLMSG_DATA(nlh));

	int pid;
	struct sk_buff *skb_out;
	int msg_size;
	int res;
	char buf[1024];

	strcpy(buf, "'Litwini? Jacy Litwini'");
	pid = nlh->nlmsg_pid;

	skb_out = nlmsg_new(msg_size, 0);
	if (!skb_out) {
		pr_err("[my_netlink]Failed to allocate new skb\n");
		return;
	}
	msg_size = strlen(buf);

	nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
	NETLINK_CB(skb_out).dst_group = 0;
	memcpy(NLMSG_DATA(nlh), buf, msg_size);
	res = nlmsg_unicast(nl_sk, skb_out, pid);
	if (res < 0)
		pr_info("Error while sending back to user\n");

	if(skb == NULL) {
	printk("skb is NULL \n");
	return;
	}
}

static void netltest(void)
{

	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready,
	};

	nl_sk = netlink_kernel_create(&init_net,MY_NETLINK, &cfg);

}

static int __init my_module_init(void)
{
	printk(KERN_INFO "Initializing netlink socket");
	netltest();
	return 0;
}

static void __exit my_module_exit(void)
{
	printk(KERN_INFO "Unloading module");
	netlink_kernel_release(nl_sk);
}

module_init(my_module_init);
module_exit(my_module_exit);
