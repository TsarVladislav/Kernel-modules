#include <linux/kernel.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#define PORT 80
MODULE_LICENSE("GPL");
static struct nf_hook_ops netfilter_ops_in; /* NF_IP_PRE_ROUTING */
unsigned int main_hook(unsigned int hooknum, struct sk_buff *skb,
                  const struct net_device *in,const struct net_device *out,
                  int (*okfn)(struct sk_buff*))
{
	struct iphdr *ip_header;
	struct udphdr *udp_header;
	struct tcphdr *tcp_header;


	unsigned src_ip;
	unsigned dest_ip;
	unsigned src_port;

	ip_header = (struct iphdr *)skb_network_header(skb);
	src_ip  = (unsigned)ip_header->saddr;
	dest_ip = (unsigned)ip_header->daddr;
	src_port = 0;
	if (ip_header->protocol == IPPROTO_TCP) {
		udp_header = (struct udphdr *)skb_transport_header(skb);
		src_port = (unsigned)ntohs(udp_header->source);
		if(src_port == PORT)
			return NF_DROP;
	} else if (ip_header->protocol == IPPROTO_UDP) {
		tcp_header = (struct tcphdr *)skb_transport_header(skb);
		src_port = (unsigned)ntohs(tcp_header->source);
		if(src_port == PORT)
			return NF_DROP;
	}

	return NF_ACCEPT;
}

int init_module(void)
{
	printk("+++ mprortblock loading +++\n");
	netfilter_ops_in.hook      = main_hook;
	netfilter_ops_in.hooknum   = NF_INET_PRE_ROUTING;
	netfilter_ops_in.pf        = PF_INET;
	netfilter_ops_in.priority  = NF_IP_PRI_FIRST;
	nf_register_hook(&netfilter_ops_in);  /* register NF_IP_PRE_ROUTING hook */
	return 0;
}
void cleanup_module()
{
	printk("+++ mprortblock unloading +++\n");
	nf_unregister_hook(&netfilter_ops_in); /*unregister NF_IP_PRE_ROUTING hook*/
}

