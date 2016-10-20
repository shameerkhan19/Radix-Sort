//Name: Shameer Khan
//CS-323
//Project 3.1 : RadixSort

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int tableSize=256;

class listNode{
friend class linkedStack;
friend class linkedQueue;
friend class radixSort;

private:
    string originalData;//this is just to keep the printing neat and nice. just for printing purpose.
    string paddedData;// this is actually used in assigning strings to their respective bucket.
    listNode *next;
public:
    listNode(){};
    listNode(string o, string p){
        originalData=o;
        paddedData=p;
        next=NULL;
    }

};

class linkedStack{
    friend class radixSort;
private:
    listNode *top;
    int size;

public:
    linkedStack(){
        top=NULL;
        size=0;
    }
    linkedStack(listNode *n){
        top=n;
        size++;
    }

    void push(listNode *n){
        if(top==NULL){
            top=n;
            size++;
            return;
        }
        n->next=top;
        top=n;
        size++;
    }

    listNode* pop(){
    listNode* temp = top;
    top=top->next;
    size--;
    return temp;
    }

    void printStack(ofstream &out){
    listNode *current=top;
    while(current!=NULL){
        out<<current->originalData<<endl;
        current=current->next;
        }
    }

    bool isEmpty(){
        if(size==0)return true;
        return false;
    }

};

class linkedQueue{
friend class radixSort;

private:
    listNode* head;
    listNode* tail;
    int size;
public:
    linkedQueue(){
            head=NULL;
            tail=NULL;
            size=0;
    }
    linkedQueue(listNode *n){
        head=n;
        tail=head;
        size++;
    }

    void addTail(listNode *n){
        if(head==NULL){
            head=n;
            tail=head;
            size++;
            return;
        }
        tail->next=n;
        tail=n;
        size++;
    }
    listNode* deleteHead(){
        listNode *temp=head;
        if(head==tail){
            head=NULL;
            tail=NULL;
            size--;
            return temp;
        }
        head=head->next;
        size--;
        return temp;
    }

    bool isEmpty(){
    if(size==0)return true;
        return false;
    }
    void printQueue(){
    listNode *current=head;
    while(current!=NULL){
        cout<<current->originalData<<endl;
        current=current->next;
        }
    }

};

class radixSort{

private:
    linkedQueue hashTable[2][tableSize];
    int currentTable;
    int previousTable;
    int currentDigit;
    int currentQueue;
    int longestDigit;
    fstream infile;
    ofstream outfile;

public:
    radixSort(char* filename, char* arg){
        currentDigit=0;
        currentTable=0;
        previousTable=0;
        longestDigit=findLongest(filename);
        currentQueue=0;
        outfile.open(arg);
    }
    int findLongest(char* file){
    string s;
    int min=0;
    infile.open(file);
    while(infile>>s){
        if(s.length()>min){
            min=s.length();
            }
        }
    return min;
    }
    char getVal(string S){
        int location= (longestDigit)-currentDigit;
        if(location<0 || S[location]==' ')
            return (int)'a';
            return S[location];
    }

    int hashIndex(char C){
        return (int)(C);
    }

    string padding (string s){
    if(s.length()==longestDigit)return s;
    else{
    int widthNeeded=(longestDigit-s.length());
    string empt=" ";
    while(empt.length()!=widthNeeded){
        empt+=s[0];
    }
    empt+=s;
    return empt;
        }
    }

    void createHash_Table(linkedStack S){
    infile.clear();
    infile.seekg(0,ios::beg);
    string s, keeper="";
    while(infile>>s){
        keeper= padding(s);
        listNode *n= new listNode(s,keeper);
        S.push(n);
        }
    outfile<<"final stack :"<<endl;
    S.printStack(outfile);
    phase_One(S);
    }

    void phase_One(linkedStack S){
    while(!S.isEmpty()){
        listNode *n =S.pop();
        char currentChar=getVal(n->paddedData);
        int index=hashIndex(currentChar);
        hashTable[currentTable][index].addTail(n);
        }
    outfile<<"status after popping all nodes from stack and entering them to hashtable, initialization of hastable phase 1: "<<endl;
        printTable(false);
    outfile<<endl;
        bucketSort();
    }

    void bucketSort(){
    currentTable=1;
    previousTable=0;
    currentDigit=1;
    currentQueue=0;
    while(currentDigit<=longestDigit){
        while(currentQueue<=tableSize-1){
            while(!hashTable[previousTable][currentQueue].isEmpty()){
                listNode *n= hashTable[previousTable][currentQueue].deleteHead();
                char currentChar= getVal(n->paddedData);
                int index=hashIndex(currentChar);
                hashTable[currentTable][index].addTail(n);
                }
                currentQueue++;
            }
        int temp=currentTable;
        currentTable=previousTable;
        previousTable=temp;
        currentQueue=0;
        currentDigit++;
        printTable(true);
        outfile<<endl;
        }
   outfile<<"Final sorted HashTable : \n";
   printTable(false);
   //hashTable[1][122].printQueue();
    }

    void printTable(bool flag){
    if (flag==true){
        outfile<<"Table so far : \n";
        outfile<<"(Printing these for Debugging purpose so i can see the changes. Check the bottom for final sorted table) \n";
    }
    for(int i=0; i<2; i++){
            for(int j=0; j<tableSize; j++){
                if(hashTable[i][j].size!=0){
                    outfile<<"["<<i<<"] ["<<j<<"]";
                    listNode *c=hashTable[i][j].head;
                    int count=0;
                    while(count!=hashTable[i][j].size-1){
                        outfile<<c->originalData<<",";
                        c=c->next;
                        count++;
                    }
                    outfile<<c->originalData<<endl;
                }
            }
        }
    }

    ~radixSort(){
        outfile.close();
        infile.close();
    }

};

int main(int argc, char* argv[]){

    radixSort rS(argv[1],argv[2]);
    linkedStack Stack1;
    rS.createHash_Table(Stack1);
    return 0;
}


