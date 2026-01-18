
#include "icmp.h"
#include "ip.h"
#include "ping.h"
#include "util.h"



int
main (void)
{
    srand (time (0));

    const uint8 *msg = (uint8 *)"ohme god damn";
    uint16 msize = strlen ((char *)msg);

    icmp_pt *icmp = mkicmp (ECHO, CODE, msg, msize);
    uint16 icmpsize = sizeof (icmp_pt) + msize;
    assert (icmp);

    char *src = "192.168.31.110";
    char *dst = "192.168.31.129";

    ipv4_pt *ip = mkip (ID, TTL, ICMP, inet_addr (src), inet_addr (dst), icmp,
                        icmpsize);
    uint16 ipsize = sizeof (ipv4_pt) + icmpsize;
    assert (ip);

    show (icmp, icmpsize);
    show ((void *)ip, ipsize);

    int sock = setsocket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == ERROR)
    {
        return EXIT_FAILURE;
    }

    struct sockaddr_in dest;
    if (setip (&dest, dst))
    {
        return EXIT_FAILURE;
    }

    if (!sendip (sock, ip, &dest))
    {
        return EXIT_FAILURE;
    }

    printf ("packet sent to %s.\n", dst);

    free (icmp);
    free (ip);
    close (sock);

    return EXIT_SUCCESS;
}



