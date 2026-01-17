#pragma once

#include "ping.h"


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

    for (n = size, sc = (uint8 *)src, dc = (uint8 *)dst; n; n--)
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

