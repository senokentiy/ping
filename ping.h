#pragma once

#define _GNU_SOURCE 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define packed __attribute__ ((packed))

#define echoreply 0
#define echo 8

typedef unsigned char uint8;
typedef unsigned short uint16;

// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
typedef struct
{
    uint8 type, code;
    uint16 checksum;
    void *data;
} packed icmp_pt;


icmp_pt *mkicmp (uint8, uint8, const uint8 *, uint16);
uint16 checksum (const icmp_pt *);
void copy (uint8 *, uint8 *, uint16);
void show (const icmp_pt *, uint16);

