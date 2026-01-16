#pragma once

#define _GNU_SOURCE 1

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define packed __attribute__ ((packed))
#define big_endian __attribute__ ((scalar_storage_order ("big-endian")))
#define lil_endian __attribute__ ((scalar_storage_order ("little-endian")))

#define ECHOREPLY 0
#define ECHO 8
#define CODE 0
#define ID 7
#define ICMP 1


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


// utils
void copy (void *, const void *, uint16);
uint16 checksum (const void *, uint16);
void show (const void *, uint16);

// icmp
icmp_pt *mkicmp (uint8, uint8, const void *, uint16);

// ip
ipv4_pt *mkip (uint16, uint8, uint8, uint32, uint32, void *, uint16);

