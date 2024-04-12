#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
  int id;
  char name;
  int order;
};

// Node for separate chaining
struct Node
{
  struct RecordType data;
  struct Node* next;
};

// HashType
struct HashType
{
  struct Node* head;
};

// Compute the hash function using multiplication method
int hash(int x, int hashSz)
{
  const double A = 0.6180339887; // Knuth's golden ratio
  double val = x * A;
  val -= (int)val;
  return (int)(hashSz * val);
}

// Parses input file to an array of records
int parseData(char* inputFileName, struct RecordType** ppData)
{
  FILE* inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
    if (*ppData == NULL)
    {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i)
    {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &pRecord->id);
      fscanf(inFile, "%c ", &pRecord->name);
      fscanf(inFile, "%d ", &pRecord->order);
    }

    fclose(inFile);
  }

  return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i)
  {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// Inserts a record into the hash table
void insertRecord(struct HashType hashTable[], struct RecordType record, int hashSz)
{
  int index = hash(record.id, hashSz);
  struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
  if (newNode == NULL)
  {
    printf("Cannot allocate memory\n");
    exit(-1);
  }
  newNode->data = record;
  newNode->next = hashTable[index].head;
  hashTable[index].head = newNode;
}

// Displays records in the hash structure
void displayRecordsInHash(struct HashType hashTable[], int hashSz)
{
  int i;
  for (i = 0; i < hashSz; ++i)
  {
    if (hashTable[i].head != NULL)
    {
      printf("Index %d -> ", i);
      struct Node* curr = hashTable[i].head;
      while (curr != NULL)
      {
        printf("%d, %c, %d", curr->data.id, curr->data.name, curr->data.order);
        curr = curr->next;
        if (curr != NULL)
        {
          printf(" -> ");
        }
      }
      printf("\n");
    }
  }
}

int main(void)
{
  struct RecordType *pRecords;
  int recordSz = 0;
  int hashSz = 31; 

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);

  // Initialize hash table
  struct HashType hashTable[hashSz];
  for (int i = 0; i < hashSz; ++i)
  {
    hashTable[i].head = NULL;
  }

  // Insert records into hash table
  for (int i = 0; i < recordSz; ++i)
  {
    insertRecord(hashTable, pRecords[i], hashSz);
  }

  // Display records in hash table
  displayRecordsInHash(hashTable, hashSz);

  // Free allocated memory
  for (int i = 0; i < hashSz; ++i)
  {
    struct Node* curr = hashTable[i].head;
    while (curr != NULL)
    {
      struct Node* temp = curr;
      curr = curr->next;
      free(temp);
    }
  }
  free(pRecords);

  return 0;
}
