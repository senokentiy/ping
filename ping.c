
#include "ping.h"
#include <stdio.h>


// https://en.wikipedia.org/wiki/Internet_checksum
uint16
checksum (const icmp_pt *pt)
{
    return 0;
}


void
copy (uint8 *src, uint8 *dst, uint16 size)
{
    uint8 *sc;
    uint8 *dc;
    uint16 n;

    if (!src || !dst)
    {
        fprintf (stderr, "[x] copy error: empty data.");
        return;
    }

    for (n = size, sc = src, dc = dst; n; n--)
    {
        *dc++ = *sc++;
    }
}


void
show (const icmp_pt *pt, uint16 size)
{
    uint8 *cursor = (uint8 *)pt;

    if (!pt)
    {
        fprintf (stderr, "[x] show error: empty packet.");
        return;
    }

    for (uint16 n = size; n; n--)
    {
        printf ("%x", *cursor++);
    }
    printf ("\n");
}


icmp_pt *
mkicmp (uint8 type, uint8 code, const uint8 *data, uint16 dsize)
{
    icmp_pt *pt = NULL;

    if (!data || !dsize)
    {
        fprintf (stderr, "[x] icmp error: empty packet data.");
        return NULL;
    }

    pt = malloc (sizeof (icmp_pt) + dsize);
    assert (pt);

    pt->type = type;
    pt->code = code;

    pt->checksum = checksum (pt);

    return pt;
}


int
main (void)
{
    uint8 *msg = (uint8 *)"hello";
    icmp_pt *packet = mkicmp (echo, 0, msg, 5);
    uint16 size = sizeof (icmp_pt) + 5;

    show (packet, size);

    return EXIT_SUCCESS;
}



