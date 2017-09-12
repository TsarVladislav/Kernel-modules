#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define NETLINK_NITRO 17
#define MAX_PAYLOAD 2048

#define MSG "Skąd Litwini wracali?"
int main()
{
	struct sockaddr_nl s_nladdr, d_nladdr;
	struct msghdr msg;
	struct nlmsghdr *nlh = NULL;
	struct iovec  iov;
	int fd;
	fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_NITRO );

	/* source address */
	memset(&s_nladdr, 0 ,sizeof(s_nladdr));
	s_nladdr.nl_family= AF_NETLINK ;
	s_nladdr.nl_pad=0;
	s_nladdr.nl_pid = getpid();
	bind(fd, (struct sockaddr*)&s_nladdr, sizeof(s_nladdr));

	/* destination address */
	memset(&d_nladdr, 0 ,sizeof(d_nladdr));
	d_nladdr.nl_family = AF_NETLINK ;
	d_nladdr.nl_pad = 0;
	d_nladdr.nl_pid = 0;

	/* Fill the netlink message header */
	nlh = (struct nlmsghdr *)malloc(1024);
	memset(nlh , 0 , 1024);
	strcpy(NLMSG_DATA(nlh), MSG);
	nlh->nlmsg_len = 1024;
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 1;
	nlh->nlmsg_type = 0;

	/*iov structure */
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;

	/* msg */
	memset(&msg,0,sizeof(msg));
	msg.msg_name = (void *) &d_nladdr ;
	msg.msg_namelen=sizeof(d_nladdr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	sendmsg(fd, &msg, 0);

	memset(nlh, 0, NLMSG_SPACE(sizeof(1024)));
	if(recvmsg(fd, &msg, 0) < 0)
	{
		perror("recvmsg");
		return 1;
	}


	printf("recieved message: %s\n", (char*)NLMSG_DATA(nlh));
	close(fd);
	return 0;
}
