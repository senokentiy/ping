#pragma once

#include "ping.h"
#include "util.h"

#define ECHOREPLY 0
#define ECHO 8
#define CODE 0


// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
typedef struct
{
    uint8 type;
    uint8 code;
    uint16 checksum;
    uint8 data[];
} packed icmp_pt;


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
    pt = (icmp_pt *)malloc (psize);
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
