/* file:    LPM_naive.c*/
/* author:  */
/* date:    */
/* version: 1.0 */

/* Description:
    Program takes routing table on input and evaluates
     correct destination of every package.
  */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int getPacketInterface(int packet_ip[4], int **network, int **mask, int *interface, int *mask_int, int size);
void makeMask(int mask_number, int mask[4]);

int main(int argc, char *argv[]) {
   int table_size, input_size;
   int **network, **mask, *interface, *mask_int; /* Represents routing table */
   int packet_ip[4];

   scanf("%d", &table_size);
   mask_int = malloc(sizeof(int) * table_size);  /* Holds integer form of mask */
   interface = malloc(sizeof(int) * table_size); /* holds interface of subnets in table */
   mask = malloc(sizeof(int *) * table_size);    /* Holds actual mask */
   network = malloc(sizeof(int *) * table_size); /* Holds addresses of subnets*/
   assert(mask != NULL && interface != NULL && network != NULL);

   /* Scans table from terminal */
   for (int i = 0; i < table_size; i++) {
      network[i] = malloc(sizeof(int) * 4);
      mask[i] = calloc(4, sizeof(int));
      assert(network[i] != NULL && mask[i] != NULL);

      scanf("%d.%d.%d.%d/", &network[i][0], &network[i][1], &network[i][2], &network[i][3]);
      scanf("%d %d", &mask_int[i], &interface[i]);
      makeMask(mask_int[i], mask[i]);
   }

   /* Scans the ip address of packet */
   scanf("%d", &input_size);
   for (int i = 0; i < input_size; i++) {
      scanf("%d.%d.%d.%d", &packet_ip[0], &packet_ip[1], &packet_ip[2], &packet_ip[3]);
      printf("%d\n", getPacketInterface(packet_ip, network, mask, interface, mask_int, table_size));
   }

   for (int i = 0; i < table_size; i++) {
      free(network[i]);
      free(mask[i]);
   }
   free(mask);
   free(network);
   free(interface);
   free(mask_int);
   return 0;
}

/* Function returns the correct Interface for packet */
int getPacketInterface(int packet_ip[4], int **network, int **mask, int *interface, int *mask_int, int size) {
   int longest_match = -1, destination; /* Keeps track of greatest match and final destination */
   for (int i = 0; i < size; i++) {
      int match = 1;
      for (int j = 0; j < 4; j++) {
         /* Match is found through AND operation of package ip, mask and rounting table */
         if ((packet_ip[j] & mask[i][j]) != (network[i][j] & mask[i][j])) {
            match = 0;
         }
      }

      if (match && (longest_match < mask_int[i])) {
         destination = interface[i];
         longest_match = mask_int[i];
      }
   }
   return destination;
}

/* makeMask turns the integer [0, 32] to an actual
    mask in form oct.oct.oct.oct where oct [0, 255]*/
void makeMask(int mask_int, int mask[4]) {
   int mask_idx = 0, temp = 128;
   for (; mask_idx < (mask_int / 8); mask_idx++) {
      mask[mask_idx] = 255;
   }

   /* Fills out left over mask */
   for (int i = 0; i < (mask_int % 8); i++) {
      mask[mask_idx] += temp;
      temp /= 2;
   }
}
