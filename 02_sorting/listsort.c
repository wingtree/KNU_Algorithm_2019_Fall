/* Directions: complete the code by filling in "FILL" parts
 *   adding functions and variables is allowed
 *   do not change print and file load/save
 *   do not use your compiler-specific libraries
 * FILL parts: insertion and merge sort on linked lists
 */

/* Insertion sort and merge sort on Linked Lists
 * COMP319 Algorithms, Fall 2019
 * School of Electronics Engineering, Kyungpook National University
 * Instructor: Gil-Jin Jang
 */

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////
// common functions for handling arrays
/////////////////////////////////////////////////////////////
long *readkeys_textfile( const char infile[], int *pN )
  // returns an array of long integers, with its size stored in
  // the memory indicated by integer pointer variable pN
  // the retured memory should freed by the caller
{
  long *A, tmp;
  int i;
  FILE *fp;

  // check for input file name
  if ( infile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return NULL;
  }

  // check for file existence
  fp = fopen(infile,"r");
  if ( !fp ) {
    fprintf(stderr, "cannot open file %s\n",infile);
    return NULL;
  }
  else {
    // check for number of elements
    if ( fscanf(fp, "%d", pN) != 1 || *pN <= 0 ) {
      fprintf(stderr, "cannot read number of elements %s\n",infile);
      return NULL;
    }
    else {
      A = (long*)malloc(sizeof(long)*(*pN));
      for (i=0; i<(*pN); i++) {
	if ( fscanf(fp, "%ld", &tmp) != 1 ) {
	  fprintf(stderr, "cannot read value at %d/%d\n",i+1,(*pN));
	  *pN = i;	// read data items
	  return A;
	}
	else A[i] = tmp;
      }
    }
    fclose(fp);
    return A;
  }
}

/////////////////////////////////////////////////////////////
// functions for handling linked lists
/////////////////////////////////////////////////////////////
typedef struct LNode {	// update: item type chagned to long, variable name to key
  long key;
  struct LNode *next;
} NODE;

struct LNode *generateLNode(long key)
{
  struct LNode *tmp;
  tmp = (struct LNode*) malloc(sizeof(struct LNode));
  tmp->key = key;
  tmp->next = NULL;	// bugfix on 190924
  return tmp;
}

struct LNode *insert_head(struct LNode *head, struct LNode *newPtr)
{
  if ( newPtr == NULL ) return head;	// Nothing to add
  else {
    newPtr->next = head;
    return newPtr;
  }
}

struct LNode *insert_next(struct LNode *prev, struct LNode *newPtr)
{
  //struct LNode *cur;	// unnecessary 190924 
  if ( newPtr == NULL ) return prev;	// Nothing to add
  else if ( prev == NULL ) return insert_head(NULL, newPtr);
  else {
    //cur = prev->next;	// unnecessary, so removed, on 190924 
    //newPtr->next = cur;	// unnecessary, so removed, on 190924 
    newPtr->next = prev->next;
    prev->next=newPtr;
    return newPtr;
  }
}

void traverse(struct LNode *L)
{
  struct LNode *cur;
  printf("[ ");
  for (cur=L ; cur != NULL ; cur = cur->next )
    printf("%ld ",cur->key);
  printf("]\n");
}

// the below list handling codes are not used.
// so commented out, but may use them
/*
struct LNode *emptylist()
{
  return NULL;
}

struct LNode *delete_head(struct LNode *head)
{
  struct LNode *cur;
  if ( head == NULL ) return NULL;	// empty, nothing to delete
  else {
    cur = head;
    head = head->next;
    cur->next = NULL;	 // not necessary, but just left 
    free(cur);
    return head;
  }
}

struct LNode *delete_next(struct LNode *prev)
{
  struct LNode *cur;
  if ( prev == NULL ) return NULL;	// empty from prev, nothing
  else {
    cur = prev->next;
    prev->next = cur->next;
    cur->next = NULL;	// not necessary
    free(cur);
    //cur = prev->next;	// not necessary here
    return prev;
  }
}

struct LNode *lookup_recursive (long x, struct LNode *L)
{
  if (L == NULL) return NULL;
  else if (x == L->key) return L;
  else return lookup_recursive(x, L->next);
}

struct LNode *lookup_iterative (long x, struct LNode *L)
{
  struct LNode *cur;
  for (cur=L ; cur != NULL ; cur = cur->next ) {
    if ( x == cur->key ) return cur;
  }
  return NULL;
}
*/

void free_list_iterative (struct LNode *L)
{
  struct LNode *tmp;
  while ( L != NULL ) {
    tmp = L; 
    L = L->next;	// every node access should be done before free()
    free(tmp);
  }
}

/////////////////////////////////////////////////////////////
// read a linked list from textfile
/////////////////////////////////////////////////////////////
struct LNode *array2linkedlist(long A[], int N)
{
  struct LNode *head, *cur, *prev;
  int i;

  head = NULL; prev = NULL;
  for (i=0; i<N; i++) {
    cur = generateLNode(A[i]);
    prev = insert_next(prev,cur);
    if ( head == NULL ) head = prev;
  }
  
  return head;
}

struct LNode *readkeys_textfile_linkedlist( const char infile[], int *pN )
  // read text file of integers:
  // number_of_intergers integer1 integer2 ...
  // then convert it to a linked list 
  // returns a list list of struct LNode, with its size stored in
  // the memory indicated by integer pointer variable pN
{
  struct LNode *head;
  long *A;

  A = readkeys_textfile(infile, pN);
  if ( A != NULL && *pN > 0 ) {
    head = array2linkedlist(A,*pN);
    free(A);
    return head;
  }
  else return NULL;	// failed
}

void savekeys_textfile_linkedlist( const char outfile[],
    struct LNode *head, int N )
  // save the given linked list to file whose name given by outfile[]
{
  FILE *fp;

  // check for output filename
  if ( outfile == NULL ) {
    fprintf(stderr, "NULL file name\n");
    return;
  }

  // check for file existence
  fp = fopen(outfile,"w");
  if ( !fp ) {
    fprintf(stderr, "cannot open file for write %s\n",outfile);
  }
  else {
    // save number of elements
    fprintf(fp, "%d\n",N);
    for (; head; head = head->next ) fprintf(fp, "%ld ", head->key);
    fprintf(fp,"\n");
    fclose(fp);
  }
}


/////////////////////////////////////////////////////////////
// linked list insertion sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */

NODE* list_insertion_sort ( NODE* head )
{
  struct LNode* sorted_head = NULL;
  struct LNode* sorted_tail = NULL;

  NODE* pos = head;
  NODE* temp;

  while (pos != NULL) {
    NODE* cur = head;
    NODE* least = head;
    NODE* buffer = head;

    // find minimum value node
    while (cur != NULL) {
      if (cur->key < least->key) {
        least = cur;
        buffer = temp;
      }
      if (cur->next && cur->next->key < least->key) {
        temp = cur;
      }
      // printf("cur: %ld least: %ld buffer: %ld\n", cur->key, least->key, buffer->key);
      cur = cur->next;
    }

    // remove minimum value node from list
    if (least == head) {
      NODE* temp = head->next;
      head->next = NULL;
      head = temp;
      // printf("\npos: %ld least: %ld buffer: %ld\n", pos->key, least->key, buffer->key);
    } else {
      buffer->next = least->next;
      least->next = NULL;
      // printf("\npos: %ld least: %ld buffer: %ld\n", pos->key, least->key, buffer->key);
    }

    // move minimum value node to sorted list
    if (sorted_head == NULL) {
      sorted_head = least;
      sorted_tail = least;
    } else {
      sorted_tail->next = least;
      sorted_tail = sorted_tail->next;
    }
    pos = head;
  }

  return sorted_head;
}


/////////////////////////////////////////////////////////////
// linked list merge sort
/////////////////////////////////////////////////////////////
/* GJ: may add any additional functions */
struct LNode *list_array_merge_sort( struct LNode **head_array, int N )
{
  int mid = N / 2;
  NODE* left_list;
  for (int i = 0; i < mid; i++) {
    left_list = head_array[i];
    NODE* right_list = head_array[N - 1 - i];
    NODE* temp = NULL;
    NODE* temp_cur = NULL;

    // sorting
    while (left_list && right_list) {
      if (left_list->key <= right_list->key) {
        if (!temp) {
          temp = left_list;
          temp_cur = temp;
        } else {
          temp_cur->next = left_list;
          temp_cur = temp_cur->next;
        }
        left_list = left_list->next;
        temp_cur->next = NULL;
      } else {
        if (!temp) {
          temp = right_list;
          temp_cur = temp;
        } else {
          temp_cur->next = right_list;
          temp_cur = temp_cur->next;
        }
        right_list = right_list->next;
        temp_cur->next = NULL;
      }
    }

    // put the rest data
    while (left_list) {
      temp_cur->next = left_list;
      temp_cur = temp_cur->next;
      left_list = left_list->next;
      temp_cur->next = NULL;
    }

    while (right_list) {
      temp_cur->next = right_list;
      temp_cur = temp_cur->next;
      right_list = right_list->next;
      temp_cur->next = NULL;
    }

    head_array[i] = temp;
  }

  int length = N / 2 + N % 2;

  if (length != 1) {
    left_list = list_array_merge_sort(head_array, length);
  }
  return head_array[0];
}

/////////////////////////////////////////////////////////////

/* File name by Argument input -> scanf read
 * Input file name can be typed in the terminal
 * Update on 9/24: element size is changed to long,
 * to distinguish array index easily
 * in struct Node, item -> key, Node -> LNode 
 * Update on 9/25: file reading is moved to a function readkeys_textfile()
 */
int main()
{
  int i, numElements;
  struct LNode *head;

  /* for file name, max length 1023 including path */
  char infile[1024], outfile[1024];
  int method;	// which sorting method

  // required for mergesort
  struct LNode **H;

  /* file name given by keyboard */
  printf("Input file name? ");
  scanf("%s",infile);
  printf("Output file name? ");
  scanf("%s",outfile);

  printf("Method (1: insertion, 2: merge)? ");
  scanf("%d",&method);

  /* read text file of integers:
   * number_of_intergers integer1 integer2 ...
   * then convert it to a linked list */
  head = readkeys_textfile_linkedlist(infile, &numElements);

  if ( head != NULL ) {
    // print all if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
    {
      traverse(head);
      printf("%d Loaded\n", numElements);
    }

    switch ( method ) {
      case 1:	// insertion sort
        head = list_insertion_sort(head);
        break;
      case 2:	// merge sort
        // change linked list to individual list items,
        // and save their pointers to an array of LNode* types
        H = (struct LNode**) malloc(sizeof(struct LNode*)*numElements);
        for (i=0; i<numElements; i++, head=head->next) H[i] = head;

        // have to isolate list items later
        for (i=0; i<numElements; i++) H[i]->next=NULL;

        head = list_array_merge_sort(H,numElements);
        break;
      default:
      	break;
    }

    // print out results, if not too many
    if ( numElements < 100 )	// GJ: comment out this line to always display
      traverse(head);

    // save results
    savekeys_textfile_linkedlist(outfile, head, numElements);

    // once allocated, should be freed after use
    free_list_iterative(head);
  }

  return 0;
}
