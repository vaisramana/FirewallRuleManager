
#include<iostream>
#include<vector>

using namespace std;



class arrayOperation
{
public:
    /*print*/
    void printArray(vector<int> input);
    /*search*/
    bool traversalSearch(vector<int> input, int key, int & index);
    bool binarySearch(vector<int> input, int key, int & index);

    /*sort*/
    void bubbleSort(vector<int> & input);
    void selectiveSort(vector<int> & input);
    
};

void arrayOperation::printArray(vector<int> input)
{
    int i;
    cout<<"Array size:"<<input.size()<<" elements:"<<endl;
    for(i=0; i<input.size(); i++)
    {
        cout<<input[i]<<" ";
    }
    cout<<endl;
}




bool arrayOperation::traversalSearch(vector<int> input, int key, int & index)
{
    int i;
    for(i=0; i<input.size(); i++)
    {
        if(input[i] == key)
        {
            index = i;
            return true;
        }
    }
    return false;
}

/* find one element in a pre-ordered ascending array, return false if not found */

bool arrayOperation::binarySearch(vector<int> input, int key, int & index)
{
    int low=0;
    int high = input.size()-1;
    int mid;
    while(low<high)
    {
        mid = (low+high)/2;
        if(input[mid] == key)
        {
            index = mid;
            return true;
        }
        if(input[mid] > key)
        {
            high = mid-1;
        }
        else
        {
            low = mid+1;
        }
    }
    return false;
}

/*
example:
13 6 18 1 1 16
each rround to compare with neighbors and move the bigger one to right
round 1: the biggest between index 0-5 is put it in index 5, result:
6 13 1 1 16 18
round 2: the biggest between index 0-4 is put it in index 4, result:
6 1 1 13 16 18
round 3: the biggest between index 0-3 is put it in index 3, result:
1 1 6 13 16 18
round 4: the biggest between index 0-2 is put it in index 2, result:
1 1 6 13 16 18
round 5: the biggest between index 0-1 is put it in index 1, result:
1 1 6 13 16 18
no need to check index 0, it's already the smallest value
*/

void arrayOperation::bubbleSort(vector<int> & input)
{
    int i,j,tmp;
    
    
    for(i=0; i<input.size()-1; i++)
    {
        for(j=0; j<input.size()-i-1; j++)
        {
            if(input[j] > input[j+1])
            {
                tmp = input[j];
                input[j] = input[j+1];
                input[j+1] = tmp;
            }
        }
        printArray(input);
    }
}
/*
example:
13 6 18 1 1 16
round 1: found smallest value between index 0-5, put it in index 0, result:
1 6 18 13 1 16
round 2: found smallest value between index 1-5, put it in index 1, result:
1 1 18 13 6 16
round 3: found smallest value between index 2-5, put it in index 2, result:
1 1 6 13 18 16
round 4: found smallest value between index 3-5, put it in index 3, result:
1 1 6 13 18 16
round 5: found smallest value between index 4-5, put it in index 4, result:
1 1 6 13 16 18
no need to check index 5, it's already the largest value
*/
void arrayOperation::selectiveSort(vector<int> & input)
{
    int i,j,k,tmp;

    for(i=0; i<(input.size()-1);i++)
    {
        k=i;
        for(j=i+1;j<input.size();j++)
        {
            if(input[k]>input[j])
            {
                k=j;
            }
        }
        if(i<k)
        {
            tmp = input[i];
            input[i] = input[k];
            input[k] = tmp;
        }
        //printArray(input);
    }
}



int main()
{
    int i,indexFound;
    bool result;
    vector<int> input;
    arrayOperation array;
    
    int a[6] = {13,6,18,1,1,16};
    for(i=0; i<(sizeof(a)/4);i++)
    {
        input.push_back(a[i]);
    }
    /*
    result = array.traversalSearch(input,a[3],indexFound);
    cout<<"traversalSearch() result:"<<result<<" indexFound:"<<indexFound<<endl;
    result = array.binarySearch(input,18,indexFound);
    cout<<"binarySearch() result:"<<result<<" indexFound:"<<indexFound<<endl;
*/

    array.printArray(input);
    array.bubbleSort(input);
    array.printArray(input);
}

