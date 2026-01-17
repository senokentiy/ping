
#include "icmp.h"
#include "ip.h"
#include "util.h"
#include <stdlib.h>


int
main (void)
{
    srand (getpid ());

    const uint8 *msg = (uint8 *)"ohme god damn";
    uint16 msize = strlen ((char *)msg);

    icmp_pt *icmp = mkicmp (ECHO, CODE, msg, msize);
    uint16 icmpsize = sizeof (icmp_pt) + msize;
    assert (icmp);

    uint32 src = inet_addr ("192.168.31.110");
    uint32 dst = inet_addr ("192.168.31.110");

    ipv4_pt *ip = mkip (ID, TTL, ICMP, src, dst, icmp, icmpsize);
    uint16 ipsize = sizeof (ipv4_pt) + icmpsize;
    assert (ip);

    show (icmp, icmpsize);
    show (ip, ipsize);

    int sock = setsocket (AF_INET, SOCK_RAW, ICMP);
    if (sock == -1)
    {
        return EXIT_FAILURE;
    }

    if (!sendip (sock, ip))
    {
        return EXIT_FAILURE;
    }

    free (icmp);
    free (ip);
    return EXIT_SUCCESS;
}



