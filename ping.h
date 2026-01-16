#pragma once

#define _GNU_SOURCE 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define packed __attribute__ ((packed))
// #define big_endian __attribute__ ((scalar_storage_order ("big-endian")))

#define ECHOREPLY 0
#define ECHO 8
#define CODE 0

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
typedef struct
{
    uint8 type;
    uint8 code;
    uint16 checksum;
    uint8 data[];
} packed icmp_pt;


icmp_pt *mkicmp (uint8, uint8, const uint8 *, uint16);
uint16 checksum (const icmp_pt *, uint16);
void copy (uint8 *, const uint8 *, uint16);
void show (const icmp_pt *, uint16);

