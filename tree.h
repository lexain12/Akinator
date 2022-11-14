#include <stdlib.h>

const char   GraphFile[20] = "GraphFile.txt";
const size_t MAXDATASIZE   = 150;
const char SPEAKFILE[40]   = "speakFile.txt";

enum Mode
{
    WrongMode  = 0,
    Casual     = 1,
    Definition = 2,
    Difference = 3,
    Graphic    = 4,
    Quit       = 5,

};

enum Side 
{
    Left  = 0,
    Right = 1,
};

struct Node 
{
    Data  data;
    Node* left;
    Node* right;
};

struct Tree
{
    Node*  root;
    size_t size;
    int    status;
};

enum PRINT
{
    NotPrint = 0,
    ToPrint  = 1,
};

Node* nodeCtor (const Data data);

int treeCtor (Tree* tree, const Data data);

void addChild (Node* parent, Node* child, Side side);
void treePrint (const Node* node);
void makeGraph (Tree* tree);
void treeGraph (const Node* node, FILE* GraphFilePtr);

void difference (Node* node, const char* obj1, const char* obj2);
int definition (Node* node, const char* dataToFind, List_t* list);
void akinator (Node* node) ;
void differenceMode (Node* treeRoot);
void definitionMode (Node* treeRoot);

void inputCleaning();

void treeDump (Tree* tree, const char* str, ...);

void speak (const char* str, int isPrint);
int findInTree (Node* node, const char* dataToFind);
void listPrint (List_t* list);
void printMenu ();
void printDifferences (List_t* commonList, List_t* diffList, const char* obj1, const char* obj2);
void treePrint (const Node* node, FILE* DBFileptr);
Node* treeParse (Node* node, FILE* DBFileptr);

