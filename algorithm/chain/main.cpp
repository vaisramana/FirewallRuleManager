
#include<iostream>
#include<vector>
#include<stdio.h>
#include<queue>


using namespace std;

#define FALSE (0)
#define TRUE (1)

/*STACK First-In Last-Out*/

struct stackNode
{
    int data;
    stackNode *next;
};


class stack
{
    public:
        stack();
        void push(int data);
        bool pop(int & data);
        void print();
    private:
        stackNode *top;
};

stack::stack():top(NULL) {}

void stack::print()
{
    stackNode * tmpPtr;
    cout<<"stack top->";
    
    for(tmpPtr=top; tmpPtr!=NULL; tmpPtr=tmpPtr->next)
    {
        cout<<tmpPtr->data<<" ";
    }
    cout<<endl;
}

void stack::push(int data)
{
    stackNode * newNodePtr;
    newNodePtr = new stackNode;
    newNodePtr->data = data;

    if(NULL == top)
    {
        top = newNodePtr;
        top->next = NULL;
    }
    else
    {
        newNodePtr->next = top;
        top = newNodePtr;
    }
}

bool stack::pop(int & data)
{
    stackNode * tmpPtr;
    
    if(NULL == top)
    {
        return false;
    }
    else
    {
        tmpPtr = top;
        data = top->data;
        top = top->next;
        delete tmpPtr;
        return true;
    }
}



struct binaryTreeNode
{
    char data;
    binaryTreeNode * leftChild;
    binaryTreeNode * rightChild;
};

class binaryTree
{
    public:
        binaryTree();
        binaryTreeNode * createNode();
        void printNode(binaryTreeNode *nodePtr);
        void printTree();
        int getNodeDepth(binaryTreeNode *nodePtr);
        int getTreeDepth();
        void printTreeGraph();
        
    private:
        binaryTreeNode *top;
};


binaryTreeNode * binaryTree::createNode()
{
    char input;
    binaryTreeNode * ptr;
    
    scanf("%c",&input);
    getchar();
    if(' ' == input)
    {
        ptr = NULL;
    }
    else
    {
        ptr = new binaryTreeNode;
        ptr->data = input;
        cout<<"create left child of "<<input<<":"<<endl;
        ptr->leftChild = createNode();
        cout<<"create right child of "<<input<<":"<<endl;
        ptr->rightChild = createNode();
    }
    return ptr;
}

binaryTree::binaryTree()
{
    top = createNode();
}

void binaryTree::printNode(binaryTreeNode *nodePtr)
{
    if(NULL != nodePtr)
    {
        cout<<nodePtr->data<<" ";
        printNode(nodePtr->leftChild);
        printNode(nodePtr->rightChild);
    }
}

void binaryTree::printTree()
{
    printNode(top);
    cout<<endl;
}

int binaryTree::getNodeDepth(binaryTreeNode *nodePtr)
{
    int ldep=0, rdep=0;
    
    if(NULL == nodePtr)
    {
        return 0;
    }
    else
    {
        ldep = getNodeDepth(nodePtr->leftChild);
        rdep = getNodeDepth(nodePtr->rightChild);
        return ldep>rdep?(ldep+1):(rdep+1);
    }
}

int binaryTree::getTreeDepth()
{
    return getNodeDepth(top);
}




void binaryTree::printTreeGraph()
{
    int cnt = 0,base=1;
    queue<binaryTreeNode *> q;
    queue<char> list;
    
    if(NULL == top)
    {
        cout<<"empty tree"<<endl;
        return;
    }
    
    
    q.push(top);

    while(TRUE != q.empty())
    {
        binaryTreeNode * nodePtr = q.front();
        q.pop();
        cout<<nodePtr->data<<" ";
        cnt++;
        if(cnt == base)
        {
            cout<<endl;
            cnt = 0;
            base = base*2;
        }
        if(NULL != nodePtr->leftChild)
        {
            q.push(nodePtr->leftChild);
        }
        if(NULL != nodePtr->rightChild)
        {
            q.push(nodePtr->rightChild);
        }
    }
    return;
}




int main()
{
    stack myStack;
    int popData;
    myStack.push(2);
    myStack.push(5);
    myStack.push(9);
    myStack.print();
    myStack.pop(popData);
    myStack.print();
    myStack.pop(popData);
    myStack.print();
    myStack.pop(popData);
    myStack.print();
    myStack.pop(popData);
    myStack.print();

    binaryTree tree;
    tree.printTree();
    tree.printTreeGraph();
}

