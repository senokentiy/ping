
#include "ping.h"
#include <arpa/inet.h>
#include <stdlib.h>


// https://en.wikipedia.org/wiki/Internet_checksum
uint16
checksum (const void *pt, uint16 size)
{
    uint16 *pc = NULL;
    uint16 n, sum, carry = 0;
    uint32 acc = 0;

    if (!pt || !size)
    {
        fprintf (stderr, "[x] checksum error: empty packet.\n");
        return 0;
    }

    n = size;
    if (size % 2)
    {
        n++;
    }

    for (pc = (uint16 *)pt; n; n -= 2, pc++)
    {
        acc += *pc;
    }

    carry = (acc & 0xffff0000) >> 16;
    sum = (acc & 0x0000ffff);

    return ~(sum + carry);
}


void
copy (void *dst, const void *src, uint16 size)
{
    uint8 *sc = NULL;
    uint8 *dc = NULL;
    uint16 n;

    if (!src || !dst || !size)
    {
        fprintf (stderr, "[x] copy error: empty data.\n");
        return;
    }

    for (n = size, sc = (uint8 *)src, dc = dst; n; n--)
    {
        *dc++ = *sc++;
    }
}


void
show (const void *pt, uint16 size)
{
    uint8 *pc = NULL;
    uint16 n;

    if (!pt || !size)
    {
        fprintf (stderr, "[x] show error: empty packet.\n");
        return;
    }

    for (n = size, pc = (uint8 *)pt; n; n--)
    {
        printf ("%02x", *pc++);
    }

    printf ("\n");
}


icmp_pt *
mkicmp (uint8 type, uint8 code, const void *data, uint16 dsize)
{
    icmp_pt *pt = NULL;
    uint16 psize;

    if (!data || !dsize)
    {
        fprintf (stderr, "[x] icmp error: empty packet data.\n");
        return NULL;
    }

    psize = sizeof (icmp_pt) + dsize;
    pt = malloc (psize);
    assert (pt);

    pt->type = type;
    pt->code = code;
    copy (pt->data, data, dsize);
    pt->checksum = checksum (pt, psize);

    if (!pt->checksum)
    {
        fprintf (stderr, "[x] icmp checksum error: bad checksum.\n");
        return NULL;
    }

    return pt;
}


ipv4_pt *
mkip (uint16 id, uint8 ttl, uint8 protocol, uint32 src, uint32 dst,
      void *payload, uint16 pdsize)
{
    ipv4_pt *pt = NULL;
    uint16 size;

    if (!src || !dst)
    {
        fprintf (stderr, "[x] ip error: empty addr.\n");
        return NULL;
    }

    if (!payload)
    {
        fprintf (stderr, "[x] ip error: empty payload.\n");
        return NULL;
    }

    size = sizeof (ipv4_pt) + pdsize;
    pt = malloc (size);
    assert (pt);

    pt->vers = 4;
    pt->ihl = sizeof (ipv4_pt);
    pt->dscp = 0;
    pt->ecn = 0;
    pt->totlen = size;
    pt->id = id;
    pt->flags = 0;
    pt->offset = 0;
    pt->ttl = ttl;
    pt->protocol = protocol;
    pt->checksum = 0;             // later
    pt->src = src;
    pt->dst = dst;
    copy (pt->payload, payload, pdsize);
    pt->checksum = checksum (pt, size);

    if (!pt->checksum)
    {
        fprintf (stderr, "[x] ip checksum error: bad checksum.\n");
        return NULL;
    }

    return pt;
}


int
main (void)
{
    const uint8 *msg = (uint8 *)"ohme god damn";
    uint16 msize = strlen ((char *)msg);

    icmp_pt *icmp = mkicmp (ECHO, CODE, msg, msize);
    uint16 icmpsize = sizeof (icmp_pt) + msize;
    assert (icmp);

    uint32 dst = inet_addr ("192.168.0.1");
    uint32 src = inet_addr ("192.168.0.1");

    ipv4_pt *ip = mkip (0, 250, ICMP, src, dst, icmp, icmpsize);
    uint16 ipsize = sizeof (ipv4_pt) + icmpsize;
    assert (ip);

    show (icmp, icmpsize);
    show (ip, ipsize);

    free (icmp);
    free (ip);
    return EXIT_SUCCESS;
}



