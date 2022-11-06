#include <stdlib.h>

const char   GraphFile[20] = "GraphFile.txt";
const size_t MAXDATASIZE   = 50;

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

Node* nodeCtor (const Data data);

int treeCtor (Tree* tree, const Data data);

void addChild (Node* parent, Node* child, Side side);
void treePrint (const Node* node);
void makeGraph (Tree* tree);
void treeGraph (Node* node, FILE* GraphFilePtr);

void difference (Node* node, char* obj1, char* obj2);
int definition (Node* node, char* dataToFind, List_t* list);
void akinator (Node* node) ;
void differenceMode (Node* treeRoot);
void definitionMode (Node* treeRoot);

void inputCleaning();

void treeDump (Tree* tree, const char* str, ...);

int findInTree (Node* node, char* dataToFind);
void listPrint (List_t* list);
void printMenu ();
void treePrint (const Node* node, FILE* DBFileptr);
Node* treeParse (Node* node, FILE* DBFileptr);

