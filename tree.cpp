#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include <cstring>

typedef char* Data;
extern FILE* LOGFILEPTR;
typedef char* Elem_t;
#include "AltList.h"
#include "tree.h"


void inputCleaning()
{
    while (getchar() != '\n') {;}
}

Node* nodeCtor (const Data data)
{
    Node* newDataPtr = (Node*) calloc (1, sizeof(*newDataPtr));
    assert (newDataPtr != nullptr);

    Data newData     = (Data) calloc (MAXDATASIZE, sizeof(*newData));
    assert (newData != nullptr);

    strcpy (newData, data);

    newDataPtr->data  = newData;
    newDataPtr->left  = nullptr;
    newDataPtr->right = nullptr;

    return newDataPtr;
}

int treeCtor (Tree* tree, const Data data)
{
    assert (tree != nullptr);

    int errors = noErrors;
    Tree* newDataPtr = (Tree*) calloc (1, sizeof(*newDataPtr));

    if (newDataPtr == nullptr)
        return errors |= listptrError;

    tree->root   = nodeCtor (data);
    tree->size   = 1;
    tree->status = noErrors;

//    if (tree->status |= treeVerify (tree))
//    {
//        listDump (list, "Error ctor\n");
//    }

    return noErrors;
}

void addChild (Node* parent, Node* child, Side side)
{
    assert (parent != nullptr);
    assert (child  != nullptr);

    if (side) // right
        parent->right = child;
    else
        parent->left  = child;

    return;
}

void treePrint (const Node* node, FILE* DBFileptr)
{
    assert (DBFileptr != nullptr);

    if (!node) return;

    fprintf (DBFileptr, "{");
    fprintf (DBFileptr, "\"%s\"", node->data);

    if (node->left)  treePrint (node->left, DBFileptr);
            
    if (node->right) treePrint (node->right, DBFileptr);
    fprintf (DBFileptr, "}");

    return;
}

#define dumpprint(...) fprintf(GraphFilePtr, __VA_ARGS__);
void makeGraph (Tree* tree)
{
    FILE* GraphFilePtr = fopen(GraphFile, "w");
    assert (tree != nullptr);
//    if (tree->status |= listVerify (list))
//    {
//        listDump (list, "Error in myGraph function, element: \n");
//    }

    dumpprint ("digraph MyGraph {\n")
    dumpprint ("    node [color=black, shape=record, style=\"rounded, filled\"];\n")
    dumpprint ("    rankdir=TB;\n")
    dumpprint ("    edge[constraint=true];\n")

    dumpprint ("    nd%p [fillcolor=\"#a1a1a1\", label=\"%s\"];\n",
                tree->root, tree->root->data);

   treeGraph (tree->root, GraphFilePtr);

    dumpprint ("}\n")

    fclose(GraphFilePtr);
    static int picVersion = 0;

    char buf[100] = "";
    sprintf(buf, "dot -Tsvg -Gcharset=latin1 GraphFile.txt > src/pic%d.svg", picVersion);
    picVersion++;

    system (buf);
}

void treeDump (Tree* tree, const char* str, ...)
{
    assert(tree != nullptr);
    fprintf(LOGFILEPTR, "<hr>\n");

    va_list argPtr = nullptr;
    va_start (argPtr, str);

    fprintf (LOGFILEPTR, "<h2>");
    vfprintf (LOGFILEPTR, str, argPtr);
    fprintf (LOGFILEPTR, "</h2>\n");
//    fprintf (LOGFILEPTR, "<h2>Status:</h2>\n");
//    
//if (list->status)
//    {
//        fprintf (LOGFILEPTR, "<h2>(ERROR: %d)</h2>\n", list->status);
//        fprintf (LOGFILEPTR, "-----------------Errors-------------------\n");
//    #define ErrorPrint(error, text)                                       \
//                if (error & list->status)                                       \
//                {                                                         \
//                    fprintf(LOGFILEPTR, "ERROR ["#error"] " #text "\n"); \
//                }
//
//        ErrorPrint (noErrors,             No errors);
//        ErrorPrint (listptrError,          Wrong ptr on structure with list);
//        ErrorPrint (capacityError,        Wrong capacity);
//        ErrorPrint (dataError,            No pointer on data (list with elements));
//        ErrorPrint (sizeError,            Bad Size of list);
//        ErrorPrint (sizeAndCapacityError, Size bigger than capacity => problem with list size) ErrorPrint (listResizeUpError,    Cannot resizeUp the list);
//        ErrorPrint (memAllocError,        Cannot allocate memory);
//        ErrorPrint (listResizeDownError,  Cannot resize down);
//        fprintf (LOGFILEPTR, "-------------End-of-errors----------------\n");
//    }
//    else
//        fprintf(LOGFILEPTR, "<h2> (no errors) </h2>\n");
    
    makeGraph (tree);
    static int picVersion = 0;
    fprintf (LOGFILEPTR, "<img src = \"src/pic%d.svg\"/>\n", picVersion++);

    return;
}

void treeGraph (Node* node, FILE* GraphFilePtr)
{
    assert (node != nullptr);

    if (node->left)
    {
        dumpprint ("    nd%p [fillcolor=\"#a1a1a1\", label=\"%s\"];\n",
                    node->left, node->left->data);

        dumpprint ("    nd%p -> nd%p\n", node, node->left);

        treeGraph (node->left, GraphFilePtr);
    }
    if (node->right)
    {
        dumpprint ("    nd%p [fillcolor=\"#a1a1a1\", label=\"%s\"];\n",
                    node->right, node->right->data);

        dumpprint ("    nd%p -> nd%p\n", node, node->right);

        treeGraph (node->right, GraphFilePtr);
    }

    return;
}
#undef dumpprint

void akinator (Node* node) 
{
    assert (node != nullptr);

    Node* curNode = node;

    while (true)
    {
        char answer[20] = "";

        printf("%s ?\n", curNode->data);
        scanf ("%s", answer);

        if (strcmp (answer, "yes") == 0)
        {
            if (curNode->left)
                curNode = curNode->left;
            else
            {
                printf ("I guessed\n");
                break;
            }
        }
        else if (strcmp (answer, "no") == 0)
        {
            if (curNode->right)
                curNode = curNode->right;
            else
            {
                char object[MAXDATASIZE]     = "";
                char difference[MAXDATASIZE] = "";

                printf ("Who was it?\n");

                inputCleaning ();
                scanf ("%[^\n]", object);
                assert (object != nullptr);

                printf ("What's the difference between %s and %s? \n", object, curNode->data);
                inputCleaning ();
                scanf ("%[^\n]", difference);

                Node* yesNode = nodeCtor (object);
                Node* noNode  = nodeCtor (curNode->data);
                strcpy (curNode->data, difference);

                addChild (curNode, yesNode, Left);
                addChild (curNode, noNode,  Right);

                printf ("I will be smarter next time\n");

                break;
            }
        }
    }
}

Node* treeParse (Node* node, FILE* DBFileptr)
{
    char data[MAXDATASIZE] = "";
    char bracket           = '\0';

    bracket = fgetc (DBFileptr);

    Node* curNode = nullptr; 

    if (bracket == '{')
    {
        fscanf (DBFileptr, "\"%[^\"]\"", data);
        curNode = nodeCtor (data);
    }

    bracket = fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }

    if (bracket == '{')
    {
        ungetc (bracket, DBFileptr);
        curNode->left = treeParse (curNode, DBFileptr);
    }

    bracket = fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }
    else if (bracket == '{')
    {
        ungetc (bracket, DBFileptr);
        curNode->right = treeParse (curNode, DBFileptr);
    }

    bracket = fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }

    assert (0);
    return nullptr;
}

int definition (Node* node, char* dataToFind, List_t* list)
{
    if (strcmp (node->data, dataToFind) == 0)
    {
        return 1;
    }

    else if (node->left == nullptr && node->right == nullptr)
    {
        return 0;
    }

    if (definition (node->left, dataToFind, list))
    {
        listTailAdd (list, node->data);

        return 1;
    }

    else if (definition (node->right, dataToFind, list))
    {
        Elem_t sentence = (Elem_t) calloc (MAXDATASIZE + 10, sizeof(char));
        sprintf (sentence, "Not %s", node->data);

        listTailAdd (list, sentence);

        return 1;
    }
    
    else
    {

        return 0;
    }

}

void difference (Node* node, char* object1, char* object2, List_t* list1, List_t* list2)
{
    definition (node, object1, list1);
    definition (node, object2, list2);

    ListElement* curElement1 = list1->nullElement->prevElementInd;
    ListElement* curElement2 = list2->nullElement->prevElementInd;

    size_t size = list2->size; 

    if (list1->size > list2->size)
    {
        size = list1->size;
    }

    for (size_t index = 0; index < size; ++index)
    {
    }
}

int findInTree (Node* node, char* dataToFind)
{
    if (strcmp (node->data, dataToFind) == 0)
    {
        return 1;
    }

    else if (node->left == nullptr && node->right == nullptr)
    {
        return 0;
    }

    if (findInTree (node->left, dataToFind))
    {
        return 1;
    }

    else if (findInTree (node->right, dataToFind))
    {
        return 1;
    }
    
    else
    {

        return 0;
    }
}

void difference (Node* node, char* obj1, char* obj2)
{
    List_t list1 = {};
    listCtor (&list1);

    List_t list2 = {};
    listCtor (&list2);

    List_t diffList = {};
    listCtor (&diffList);

    List_t commonList = {};
    listCtor (&commonList);

    definition (node, obj1, &list1);
    definition (node, obj2, &list2);

    listDump (&list1, "LIST1\n");
    listDump (&list2, "LIST2\n");

    while (list1.size != list2.size)
    {
        if (list1.size > list2.size)
        {
            listTailAdd (&diffList, listHead (&list1)->element);
            listHeadDelete (&list1);
        }

        else
        {
            listTailAdd (&diffList, listHead (&list2)->element);
            listHeadDelete (&list2);
        }
    }

    size_t size = list1.size;

    for (size_t index = 0; index < size - 1; ++index)
    {
        if (strcmp (listTail (&list1)->element, listTail (&list2)->element) == 0)
        {
            listHeadAdd (&commonList, listTail (&list1)->element);
        }
        else 
            listHeadAdd (&diffList, listTail (&list1)->element);

        listTailDelete (&list1);
        listTailDelete (&list2);
    }

    if (diffList.size > 1)
    {
        printf ("Differences:\n");
        listPrint (&diffList);
    }

    if (commonList.size > 1)
    {
        printf ("Common:\n");
        listPrint (&commonList);
    }
    else
        printf ("Nothing in common\n");

}

void listPrint (List_t* list)
{
    ListElement* curElement = list->nullElement->prevElementInd;

    for (size_t index = 0; index < list->size - 2; ++index)
    {
        printf ("%s, ", curElement->element);
        curElement = curElement->prevElementInd;
    }

    printf ("%s \n", curElement->element);
}

void printMenu ()
{
    printf ("\nWELCOME TO AKINATOR GAME\n");
    printf ("Choose mode of the game\n");
    printf ("1 - casual game (guesing your character)\n");
    printf ("2 - definition (give definition of word from database\n");
    printf ("3 - difference (give difference between two words from database\n");
    printf ("4 - graphic dump of current database\n");
    printf ("5 - quit\n");
}


int main ()
{
    Tree tree = {};
    treeCtor (&tree, "Givtnoe");

    FILE* DBFileptr = fopen ("DBFile.txt", "r");
    tree.root = treeParse (tree.root, DBFileptr);
    fclose (DBFileptr);


    while (true)
    {
        printMenu ();

        int mode = 0;
        scanf ("%d", &mode);

        switch (mode)
        {
            case WrongMode:
                printf ("Wrong input, please try again\n");
                break;

            case Casual:
                printf ("\033c");
                akinator (tree.root);
                break;

            case Definition:
                {
                List_t list = {};
                listCtor (&list);

                char dataToFind[MAXDATASIZE] = "";

                printf ("\033c");
                printf ("Okay, let's start\n");
                printf ("Please, enter the word\n");
                
                inputCleaning ();
                scanf ("%s", dataToFind);

                int isFound = definition (tree.root, dataToFind, &list);

                if (!isFound) 
                    printf ("Object not found\n");
                else 
                    listPrint (&list);

                break;
                }

            case Difference:
                {
                printf ("\033c");
                printf ("Write first word\n");

                char first[MAXDATASIZE] = "";
                scanf ("%s", first);

                printf ("Write second word\n");

                char second[MAXDATASIZE] = "";
                scanf ("%s", second);

                difference (tree.root, first, second);

                break;
                }

            case Graphic:
                treeDump (&tree, "Graphic dump, called by graphic mode\n");

                break;

            case Quit:

                DBFileptr = fopen ("DBFile.txt", "w");

                treePrint (tree.root, DBFileptr);
                fclose (DBFileptr);

                return 0;
                break;

            default:
                printf ("Wrong mode, try again\n");
        }
    }

    return 0;
}
