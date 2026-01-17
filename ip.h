#pragma once

#include "icmp.h"
#include "ping.h"
#include "util.h"
#include <netinet/in.h>
#include <stdlib.h>

#undef RAND_MAX
#define RAND_MAX 65536
#define ERROR -1

#define ID rand () % RAND_MAX
#define ICMP 1
#define TTL 250


// https://en.wikipedia.org/wiki/IPv4#Packet_structure
typedef struct
{
    uint8 vers : 4;
    uint8 ihl : 4;
    uint8 dscp : 6;
    uint8 ecn : 2;
    uint16 totlen;
    uint16 id;
    uint16 flags : 3;
    uint16 offset : 13;
    uint8 ttl;
    uint8 protocol;
    uint16 checksum;
    uint32 src;
    uint32 dst;
    uint8 payload[];
} packed ipv4_pt;


int
setsocket (uint32 domain, uint32 type, uint32 protocol)
{
    int sock;

    if ((sock = socket (domain, type, protocol)) == -1)
    {
        fprintf (stderr, "[x] setsocket error: %d\n", errno);
        return ERROR;
    }

    int opt = 1;
    if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, &opt, sizeof (opt)) == -1)
    {
        fprintf (stderr, "[x] setsocket error: %d\n", errno);
        return ERROR;
    }

    return sock;
}


int
setip (struct sockaddr_in *dest, const char *dst)
{
    dest->sin_family = AF_INET;

    if (inet_pton (AF_INET, dst, &dest->sin_addr) <= 0)
    {
        fprintf (stderr, "[x] setip error: %d.", errno);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int
sendip (uint32 sock, ipv4_pt *pt, const struct sockaddr_in *dst)
{
    int bytes;

    if (!pt || !sock)
    {
        fprintf (stderr, "[x] sendip error: emtpy packet or socket.\n");
        return 0;
    }

    if ((bytes = sendto (sock, pt, pt->totlen, MSG_DONTWAIT,
                         (struct sockaddr *)dst, sizeof (*dst)))
        == -1)
    {
        fprintf (stderr, "[x] sendip error: %d.\n", errno);
        return 0;
    }

    return bytes;
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
    pt = (ipv4_pt *)malloc (size);
    assert (pt);

    pt->vers = 4;
    pt->ihl = sizeof (ipv4_pt) / 4;
    pt->dscp = 0;
    pt->ecn = 0;
    pt->totlen = size;
    pt->id = id;
    pt->flags = 0;
    pt->offset = 0;
    pt->ttl = ttl;
    pt->protocol = protocol;
    pt->checksum = 0; // later
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

