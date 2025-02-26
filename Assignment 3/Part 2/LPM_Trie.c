/* file:    LPM_Trie.c*/
/* author:  */
/* date:    */
/* version: 1.0 */

/* Description:
    Program parses routing table and stores it as Trie
    by bits. It outputs interface of incoming pockets
  */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   uint32_t network;
   int prefix_len;
   int interface;
} Subnet;

typedef struct TrieNode {
   struct TrieNode *children[2];
   int interface;
} TrieNode;

/* Parsing functions */
uint32_t ip_to_int(const char *ip);
Subnet parse_subnet(char *line);

/* Trie manipulation */
int searchForInterface(TrieNode *root, uint32_t network);
void insertSubnet(TrieNode *root, Subnet sub);
TrieNode *create_trie_node();
void freeTrie(TrieNode *node);

int main(int argc, char *argv[]) {
   int n, m;
   char line[30];
   TrieNode *root = create_trie_node();

   scanf("%d", &n);
   getchar();
   for (int i = 0; i < n; i++) {
      fgets(line, sizeof(line), stdin);
      Subnet curent_subnet = parse_subnet(line);
      insertSubnet(root, curent_subnet);
   }

   scanf("%d", &m);
   getchar();
   for (int i = 0; i < m; i++) {
      fgets(line, sizeof(line), stdin);
      line[strcspn(line, "\n")] = 0;
      printf("%d\n", searchForInterface(root, ip_to_int(line)));
   }

   freeTrie(root);
   return 0;
}

int searchForInterface(TrieNode *root, uint32_t network) {
   TrieNode *cur_node = root;
   int last_interface;

   for (int i = 31; i >= 0; i--) {
      int bit = (network >> i) & 1;
      if (cur_node->interface != -1) {
         last_interface = cur_node->interface;
      }
      if (cur_node->children[bit] == NULL) {
         break;
      }
      cur_node = cur_node->children[bit];
   }
   return last_interface;
}

void insertSubnet(TrieNode *root, Subnet sub) {
   TrieNode *cur_node = root;

   for (int i = 31; i >= 32 - sub.prefix_len; i--) {
      int bit = (sub.network >> i) & 1;

      if (cur_node->children[bit] == NULL) {
         cur_node->children[bit] = create_trie_node();
      }
      cur_node = cur_node->children[bit];
   }
   cur_node->interface = sub.interface;
}

TrieNode *create_trie_node() {
   TrieNode *new_node = (TrieNode *)malloc(sizeof(TrieNode));
   new_node->children[0] = NULL;
   new_node->children[1] = NULL;
   new_node->interface = -1;
   return new_node;
}

void freeTrie(TrieNode *node) {
   if (node == NULL) {
      return;
   }
   freeTrie(node->children[0]);
   freeTrie(node->children[1]);
   free(node);
}


uint32_t ip_to_int(const char *ip) {
   uint32_t a, b, c, d;
   sscanf(ip, "%u.%u.%u.%u", &a, &b, &c, &d);
   return (a << 24) | (b << 16) | (c << 8) | d;
}

Subnet parse_subnet(char *line) {
   char ip_str[20];
   int prefix, interface;
   Subnet new;
   sscanf(line, "%[^/]/%d %d", ip_str, &prefix, &interface);

   new.network = ip_to_int(ip_str) & (~((1 << (32 - prefix)) - 1));
   new.prefix_len = prefix;
   new.interface = interface;
   return new;
}