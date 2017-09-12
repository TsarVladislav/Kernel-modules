#ifndef H_MICMPPACKET
#define H_MICMPPACKET
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <net/net_namespace.h>
#include <net/route.h>
#include <net/ip.h>

#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#define SOURCE_IP "192.168.4.2"
#define DEST_IP "8.8.8.8"
#endif
