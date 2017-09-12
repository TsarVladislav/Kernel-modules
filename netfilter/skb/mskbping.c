#include "packet.h"

#define MODULE_NAME "mskbping"
#define DEVICE "ens3"

unsigned int inet_addr(char *str)
{
	unsigned int a[4];
	sscanf(str, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]);
	return *a;
}

void icmp_skb(void)
{
	struct sk_buff    *skb         = NULL;
	struct iphdr      *iph         = NULL;
	struct icmphdr    *icmph       = NULL;
	struct net_device *net_dev     = NULL;
	struct rtable     *route_table = NULL;
	struct flowi4     fl4;
	size_t header_size;

	header_size = sizeof(struct ethhdr) +
		      sizeof(struct iphdr) +
		      sizeof(struct icmphdr);

	skb = alloc_skb(header_size, GFP_ATOMIC);
	if (skb == NULL) {
		printk(KERN_ALERT "alloc_skb\n");
		return;
	}

	skb_reserve(skb, header_size);
	skb_push(skb, sizeof(struct icmphdr));
	skb_push(skb, sizeof(struct iphdr));
/*
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, 0);
	skb_set_transport_header(skb, sizeof(struct iphdr));
*/
	iph = ip_hdr(skb);

	printk("IHL size %d\n", sizeof(struct iphdr));
	/* Размер заголовка пакета в 32-битных словах. Минимум - 5(20 байт) */
	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
	iph->id = htons(1000);
	iph->frag_off = 0;
	iph->ttl = 64;
	iph->protocol = IPPROTO_ICMP;
	iph->check = 0;
	iph->saddr = inet_addr(SOURCE_IP);
	iph->daddr = inet_addr(DEST_IP);

	icmph = icmp_hdr(skb);

	icmph->type = ICMP_ECHO;
	icmph->code = 0;
	icmph->checksum = 0;
	icmph->un.echo.sequence = 0;
	icmph->un.echo.id = 0;
	icmph->checksum = ip_compute_csum(icmph, sizeof(struct icmphdr));

	net_dev = dev_get_by_name(&init_net, DEVICE);
	skb->dev = net_dev;
	fl4.flowi4_oif = net_dev->ifindex;
	fl4.saddr = inet_addr(SOURCE_IP);
	fl4.daddr = inet_addr(DEST_IP);

	route_table = ip_route_output_key(&init_net, &fl4);

	skb_dst_set(skb, &route_table->dst);
	pr_info("ip_local_out: %d", ip_local_out(&init_net, NULL, skb));

}

int __init skb_ping_init(void)
{
	printk(KERN_INFO "+++ Загружаю модуль %s +++\n", MODULE_NAME);
	icmp_skb();
	return 0;
}

void __exit skb_ping_exit(void)
{
	printk(KERN_INFO "--- Выгружаю модуль %s ---\n", MODULE_NAME);
}

module_init(skb_ping_init);
module_exit(skb_ping_exit);

MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL v2");

