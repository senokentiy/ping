
#include "ping.h"


// https://en.wikipedia.org/wiki/Internet_checksum
uint16
checksum (const icmp_pt *pt, uint16 size)
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
copy (uint8 *dst, const uint8 *src, uint16 size)
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
mkicmp (uint8 type, uint8 code, const uint8 *data, uint16 dsize)
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
        fprintf (stderr, "[x] checksum error: bad checksum.\n");
        return NULL;
    }

    return pt;
}


int
main (void)
{
    const uint8 *msg = (uint8 *)"ohme god damn";
    uint16 msize = strlen ((char *)msg);

    icmp_pt *packet = mkicmp (ECHO, CODE, msg, msize);
    assert (packet);
    uint16 psize = sizeof (icmp_pt) + msize;

    show (packet, psize);

    free (packet);
    return EXIT_SUCCESS;
}



