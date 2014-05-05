/*
 * Jacob Harrelson (tharrel@g.clemson.edu)
 * Mark Smotherman - CPSC 3300
 * Project 3 - OPT replacement
 * April 24, 2014
 *
 */
#include <stdio.h>
#include <stdlib.h>

#define LINES_PER_BANK 32
#define BANK_COUNT 4

unsigned int valid[BANK_COUNT][LINES_PER_BANK];
unsigned int tag[BANK_COUNT][LINES_PER_BANK];

void cache_init() {
  unsigned int i;
  for (i = 0; i < BANK_COUNT; i++) {
    unsigned int j;
    for (j = 0; j < LINES_PER_BANK; j++) {
      valid[i][j] = tag[i][j] = 0;
    }
  }
}

int main() {
  unsigned int address[1000] = {};
  unsigned int addr_tag;
  unsigned int addr_index;
  unsigned int hits;
  unsigned int misses;
  unsigned int cnt;

  cache_init();

  cnt = hits = misses = 0;

  while ((scanf("%x", &address[cnt]) != EOF) && (cnt < 1000))
    cnt++;
  
  unsigned int distance_map[BANK_COUNT];
  unsigned int i;
  for (i = 0; i < cnt; i++) {
    addr_index = (address[i] >> 5) & 0x1f;
    addr_tag = address[i] >> 10;
    distance_map[0] = distance_map[1] = distance_map[2] = distance_map[3] = -1;

    int bank = -1;
    unsigned int j;
    for (j = 0; j < BANK_COUNT; j++) {
      if ((valid[j][addr_index]) && (addr_tag == tag[j][addr_index])) {
	hits++;
	bank = j;
	break;
      }
    }

    if (bank < 0) {
      misses++;

      for (j = 0; j < BANK_COUNT; j++) {
	if (valid[j][addr_index] == 0) {
	  bank = j;
	  break;
	}
      }

      if (bank < 0) {
	unsigned int k;
	unsigned int addr_index2;
	unsigned int addr_tag2;
	for (j = cnt; j > i; j--) {
	  addr_index2 = (address[j] >> 5) & 0x1f;
	  addr_tag2 = address[j] >> 10;
	  
	  if (addr_index2 == addr_index) {
	    for (k = 0; k < BANK_COUNT; k++) {
	      if (addr_tag2 == tag[k][addr_index]) {
		distance_map[k] = j;
		break;
	      }
	    }
     	  }
	}
	
	unsigned int max = 0;
	for (j = 0; j < BANK_COUNT; j++) {
	  if (distance_map[j] > max) {
	    max = distance_map[j];
	    bank = j;
	  }
	
	  if (distance_map[j] == -1) {
	    bank = j;
	    break;
	  }
	}

      }
      
      //printf("miss on trace index %d address 0x%.8x replace bank %d\n", 
      //     i, address[i], bank);
    }

    valid[bank][addr_index] = 1;
    tag[bank][addr_index] = addr_tag;
  }

  printf("%d misses out of %d (%5.2f%% hit rate)\n", 
	 misses, cnt, (hits / (cnt * 1.0)) * 100);

  return 0;
}
