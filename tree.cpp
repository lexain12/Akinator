#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include <cstring>

#define SPEAKMODE 0

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

    dumpprint ("digraph MyGraph {\n")
    dumpprint ("    node [color=black, shape=record, style=\"rounded, filled\"];\n")
    dumpprint ("    rankdir=TB;\n")
    dumpprint ("    edge[constraint=true];\n")

    dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"%s\"];\n",
                tree->root, tree->root->data);

   treeGraph (tree->root, GraphFilePtr);

    dumpprint ("}\n")

    fclose(GraphFilePtr);
    static int picVersion = 0;

    char buf[MAXDATASIZE] = "";
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
    
    makeGraph (tree);
    static int picVersion = 0;
    fprintf (LOGFILEPTR, "<img src = \"src/pic%d.svg\"/>\n", picVersion++);

    return; }

void treeGraph (const Node* node, FILE* GraphFilePtr)
{
    assert (node != nullptr);
    assert (GraphFilePtr != nullptr);

    if (node->left)
    {
        dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"%s\"];\n",
                    node->left, node->left->data);

        dumpprint ("    nd%p -> nd%p\n", node, node->left);

        treeGraph (node->left, GraphFilePtr);
    }
    if (node->right)
    {
        dumpprint ("    nd%p [fillcolor=\"#54e3c2\", label=\"%s\"];\n",
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
                assert (difference != nullptr);

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

    bracket = (char) fgetc (DBFileptr);

    Node* curNode = nullptr; 

    if (bracket == '{')
    {
        fscanf (DBFileptr, "\"%[^\"]\"", data);
        curNode = nodeCtor (data);
    }

    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }

    if (bracket == '{')
    {
        ungetc (bracket, DBFileptr);
        curNode->left = treeParse (curNode, DBFileptr);
    }

    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }
    else if (bracket == '{')
    {
        ungetc (bracket, DBFileptr);
        curNode->right = treeParse (curNode, DBFileptr);
    }

    bracket = (char) fgetc (DBFileptr);


    if (bracket == '}')
    {
        return curNode;
    }

    assert (0);
    return nullptr;
}

int definition (Node* node, const char* dataToFind, List_t* list)
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
        Elem_t sentence = (Elem_t) calloc (MAXDATASIZE, sizeof(char));
        sprintf (sentence, "Not %s", node->data);

        listTailAdd (list, sentence);

        return 1;
    }
    
    else
        return 0;

}

int findInTree (Node* node, const char* dataToFind)
{
    if (strcmp (node->data, dataToFind) == 0)
        return 1;

    else if (node->left == nullptr && node->right == nullptr)
        return 0;

    if (findInTree (node->left, dataToFind))
        return 1;

    else if (findInTree (node->right, dataToFind))
        return 1;
    
    else
        return 0;
}

void difference (Node* node, const char* obj1, const char* obj2)
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
        {
            printf ("%s is %s, but %s is %s\n", obj1, listTail (&list1)->element, obj2, listTail (&list2)->element);
        }

        listTailDelete (&list1);
        listTailDelete (&list2);
    }

    if (commonList.size > 1)
    {
        speak (obj1, ToPrint);
        speak (" and ", ToPrint);
        speak (obj2, ToPrint);
        speak (" are ", ToPrint);
        listPrint (&commonList);
        
    }
    else
    {
        speak ("Nothing in common\n", ToPrint);
    }

    listDump (&commonList, "1\n");
    listDump (&diffList  , "2\n");

    listDtor (&list1);
    listDtor (&list2);
    listDtor (&commonList);
    listDtor (&diffList);
}

void differenceMode (Node* treeRoot)
{
    printf ("\033c");
    speak ("Write first word\n", ToPrint);

    char first[MAXDATASIZE] = "";
    scanf ("%s", first);

    speak ("Write second word\n", ToPrint);

    char second[MAXDATASIZE] = "";
    scanf ("%s", second);

    difference (treeRoot, first, second);
}

void definitionMode (Node* treeRoot)
{
    List_t list = {};
    listCtor (&list);

    char dataToFind[MAXDATASIZE] = "";

    printf ("\033c");
    speak ("Okay, let's start\n", ToPrint);
    speak ("Please, enter the word\n", ToPrint);
    
    inputCleaning ();
    scanf ("%s", dataToFind);

    int isFound = definition (treeRoot, dataToFind, &list);

    if (!isFound) 
    {
        speak ("Object not found\n", ToPrint);
    }
    else 
        listPrint (&list);


    listDump (&list, "definition\n");
    listDtor (&list);
}

void listPrint (List_t* list)
{
    ListElement* curElement = list->nullElement->prevElementInd;

    for (size_t index = 0; index < list->size - 2; ++index)
    {
        speak (curElement->element, NotPrint);
        printf ("%s, ", curElement->element);
        curElement = curElement->prevElementInd;
    }

    speak (curElement->element, NotPrint);
    printf ("%s \n", curElement->element);
}

void printDifferences (List_t* commonList, List_t* diffList, const char* str1, const char* str2)
{
    ListElement* curCommonElement = commonList->nullElement->prevElementInd;
    ListElement* curDiffElement   = diffList->nullElement->prevElementInd;

    while (curCommonElement != commonList->nullElement && curDiffElement != diffList->nullElement)
    {
        printf ("%s is %s and %s is %s, ", curCommonElement->element, str1, curDiffElement->element, str2);
        curCommonElement = curCommonElement->prevElementInd;
        curDiffElement   = curDiffElement  ->prevElementInd;
    }

    while (curCommonElement->prevElementInd != commonList->nullElement)
    {
        printf ("%s, ", curCommonElement->element);
        curCommonElement = curCommonElement->prevElementInd;
    }

    while (curDiffElement->prevElementInd != diffList->nullElement)
    {
        printf ("%s, ", curDiffElement->element);
        curDiffElement   = curDiffElement  ->prevElementInd;
    }
    
    if (curDiffElement->prevElementInd == diffList->nullElement)
    {
        printf ("%s ", curDiffElement->element);
    }

    else if (curCommonElement->prevElementInd == commonList->nullElement)
    {
        printf ("%s ", curCommonElement->element);
    }

    printf("\n");
    return;

}

void speak (const char* str, int isPrint)
{
#if (SPEAKMODE)
    char cmd[MAXDATASIZE] = "";
    sprintf (cmd, "say -v Alex -r 400 \"%s\"", str);
    system (cmd);
#endif

    if (isPrint)
        printf ("%s", str);

}

void printMenu ()
{
    speak ("WELCOME TO AKINATOR GAME\n", ToPrint);
    speak ("Choose mode of the game\n", ToPrint);
    speak ("1 - casual game (guessing your character)\n", ToPrint);
    speak ("2 - definition (give definition of word from database)\n", ToPrint);
    speak ("3 - difference (give difference between two words from database)\n", ToPrint);
    speak ("4 - graphic dump of current database\n", ToPrint);
    speak ("5 - quit\n", ToPrint);

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
                definitionMode (tree.root);
                break;

            case Difference:
                differenceMode (tree.root);
                break;

            case Graphic:
                treeDump (&tree, "Graphic dump, called by graphic mode\n");
                system ("open logfile.html");
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

        inputCleaning ();
        getchar ();
    }

    return 0;
}
