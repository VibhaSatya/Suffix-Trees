#include <iostream>
#include <iomanip>
#include <fstream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<string>
using namespace std;


char *fileArr;
int matches=0; //number of characters matched so far
int doccount=0;//document number
int nodeCount=0;//number of nodes in the tree
int docList[312];//list of documents containing the query string
int docIndex=0;//index of docList
class suffixtree
{   
    suffixtree *child[256];
    suffixtree *queue[100];
    int front,rear;
    int start,end;
    int docNum;
    int edgeMatch;//character matches in an edge
    struct egde  //edge leading to current node
   {
     int start; //start index of edge
     int end;   //end index of edge
   }e;
public:
suffixtree()
{
    int i;
    front=-1;
    rear=-1;
    for(i=0;i<256;i++)
    {
    child[i]=NULL;
    }
    edgeMatch=0;
    docNum=-1;
    e.start=-1; //root values
    e.end=-1;
    nodeCount++;
}


void enqueue(suffixtree* node)
{
if(front==-1) front=0;
rear++;
queue[rear]=node;

}

suffixtree* dequeue()
{
front++;
printEdge(queue[front-1]->e.start,queue[front-1]->e.end);
cout<<queue[front-1]->docNum<<"\n\n";
return queue[front-1];
}

suffixtree* levelTraverse(suffixtree* node)
{
if(front>rear) return NULL;
int k;
for(k=0;node->child[k]!=NULL;k++)
enqueue(node->child[k]);

return(levelTraverse(dequeue()));

}

int searchForAll(char *word)
{
 matches=0;
 suffixtree* prev;
 suffixtree* current=this;
 do{
    prev=current;
    
    current=findNode(current,word+matches);
   }while(current!=NULL);//till no matches are found

 if(prev->child[0]==NULL) {cout<<"\nPERFECT MATCH. No other document has the exact same string.\n"; return -1;}
 dfs(prev);
 return 1;
  
}



void dfs(suffixtree* current)
{

suffixtree* stack[nodeCount];
int top=-1;
int k;
do
{
for(k=0;current->child[k]!=NULL;k++)
{
  stack[++top]=current->child[k];
  //printEdge(current->child[k]->e.start,current->child[k]->e.end);
}
current=stack[top];
docList[docIndex]=current->docNum;
int j;
for(j=0;j<docIndex;j++)
if(docList[j]==current->docNum) {docIndex--;break;}
docIndex++;
top--;
}while(top>-1);
for(k=0;k<docIndex;k++)
//cout<<docList[k]<<" ";
findTitle(docList[k]);


}
 void findAllOccurrences(char* word)
{
int k;
//prints the title and the matched text 
for(k=0;k<docIndex;k++)
{ 
cout<<"\n\n";
findMatchedText(word,docList[k]);
}



}



int search(char *word)
{
matches=0;
int i=0;
suffixtree* node=this;
suffixtree* prev;
do
{
prev=node;
node=findNode(node,word+matches);
if(node==NULL)break;
//printEdge(node->e.start,node->e.end);
//cout<<node->docNum;
}while(node->child[0]!=NULL);//till a leaf is hit
if(node==NULL) return prev->docNum;
else return node->docNum;
}

void findTitle(int num)
{
    string str;
    ifstream inFile;
    inFile.open("titles.txt");
    
    if (!inFile) {
        cout << "Unable to open file";
        return;
    }
    int i;
    //cout<<"\n"<<num<<"\n";
    while(!inFile.eof())
        {
        inFile>>i;
        std::getline(inFile,str,'.');
        if(i==num) break;
	}
	
	cout<<str<<"\n";
}

void findMatchedText(char* word, int num)
{
    ifstream inFile;
    char buf[3];
    sprintf(buf,"%d",num);
    string str="doc"+string(buf)+".txt";
    char* filename=const_cast<char*>(str.c_str());
    inFile.open(filename);
    
    if (!inFile) {
        cout << "Unable to open file";
        return ;
    }
    
   char ch; int pos=-1,windex=0,count=0;

    while(inFile>>noskipws>>ch)
        {
        
         if(ch==word[windex] || ch==tolower(word[windex]) || ch==toupper(word[windex])) {windex++;count++; continue;}
         //cout<<count<<" ";
         if(count==matches) 
		{
                //cout<<" "<<matches<<" ";
		//to find all surrounding characters of the matched query string
		inFile.seekg(pos+1); 
		 do {inFile>>noskipws>>ch; cout<<ch;}while(ch!='.' && ch!='$');
                break;
                }
	else {count=0;windex=0;}
         if(ch=='.') 
           {pos=inFile.tellg();}
	}
     inFile.close();

}

void printEdge(int start,int end)
{
    int i;
    cout<<"\n";
    for(i=start;i<=end;i++)
    cout<<fileArr[i];
    cout<<"\n";
    

}


void printChildren(suffixtree* current)
{
int k;
for(k=0;current->child[k]!=NULL;k++)
printEdge(current->child[k]->e.start,current->child[k]->e.end);


}
suffixtree* findNode(suffixtree* current,char* word)
{


    int i,j,check=0;
    int wordLength=strlen(word);
   // cout<<wordLength<<"\n";
    //checking every child node of the current node to find a match
    //cout<<"\n"<<word;
    //printEdge(current->e.start,current->e.end);
    
    for(i=0;current->child[i]!=NULL;i++)
    { 
      check=0;
      //current->child[i]->edgeMatch=0;
      //cout<<"\nChecking...\n";
      //printEdge(current->child[i]->e.start,current->child[i]->e.end);
      for(j=(current->child[i]->e.start);j<=(current->child[i]->e.end);j++)
      {
      if((fileArr[j]==word[check] || fileArr[j]==tolower(word[check]) || fileArr[j]==toupper(word[check])) && check<wordLength) {check++;}
      else break;
      if(check==wordLength) break;
      }
      if(check>0)
        {
        //cout<<"\nMATCH!!...\n";
        //cout<<word;
        //printEdge(current->child[i]->e.start,current->child[i]->e.end);
        current->child[i]->edgeMatch=check;
        matches+=check;
        return current->child[i];
        }
     
     }
     //cout<<"\nNo match:(\n";
     
     return NULL;//in case no match was found or if tree is empty
}

void createBranch(suffixtree* current,char* word,int wstart,int wend)
{
    suffixtree* temp;
    suffixtree* parent=current;
   
   // cout<<word<<" "<<doccount<<"\n";
    
  
    do
    {
      temp=findNode(current,word+matches);
      if(temp==NULL) 
      {
       //cout<<word;
       //add a new node to current
       int k;
       for(k=0;current->child[k]!=NULL;k++);//to find the first NULL child

       suffixtree *newnode = new suffixtree();
 
       newnode->e.end=wend;
       newnode->e.start=wstart+matches;
       current->child[k]=newnode;
       current->child[k]->docNum=doccount;
      // printEdge(newnode->e.start,newnode->e.end);
       //cout<<newnode->docNum<<"\n";
       return;
      } 
      parent=current;
      current=temp;
      //cout<<word;
      //printEdge(current->e.start,current->e.end);
     }while(current->child[0]!=NULL);

        //in case of partial match, split the existing node

        //create a new node for the word
        //cout<<word;
        //printEdge(current->e.start,current->e.end);
        int k;
        //to find which child current is
        for(k=0;parent->child[k]!=NULL;k++)
            if(parent->child[k]==current)
                break;
        suffixtree *newnode = new suffixtree();
        newnode->e.end=current->e.start+current->edgeMatch-1;
        newnode->e.start=current->e.start;
        suffixtree *temp2=current;
        current=newnode;
        parent->child[k]=current;
        current->docNum=temp2->docNum;
        //cout<<"\n\n"<<word<<"\n";
        //printEdge(temp2->e.start,temp2->e.end);
        //cout<<temp->docNum<<"\n";
        //cout<<wstart<<" "<<wend<<"\n";
        //cout<<"\n"<<temp2->e.start<<"\n";
        //creating 2 children
        //1.Remaining part of the old node

        current->child[0] = new suffixtree();
        current->child[0]->e.start = temp2->e.start+temp2->edgeMatch;
        current->child[0]->e.end = temp2->e.end;
        current->child[0]->docNum=temp2->docNum;
        //cout<<"edge1";
        //printEdge(current->child[0]->e.start,current->child[0]->e.end);
        //cout<<current->child[0]->docNum<<"\n";

        //2.Remaining part of the new word

        current->child[1] = new suffixtree();
        current->child[1]->e.start = wstart+matches;
        current->child[1]->e.end = wend;
        current->child[1]->docNum=doccount;
        //cout<<"edge2";
        //printEdge(current->child[1]->e.start,current->child[1]->e.end);
        //cout<<current->child[1]->docNum<<"\n";
        delete(temp2);
     

}


int construct(char* filename)
{
    //reading file doc by doc to construct the tree
    ifstream inFile;
    inFile.open(filename);
    
    if (!inFile) {
        cout << "Unable to open file";
        return -1;
    }
    
    
    string str;  
    int docPos=0; //index of the first letter in the doc in fileArr
    int docEndPos=0;//index of the last letter in the doc in fileArr
    int buflength=0;
    suffixtree *current=this;
    char ch;

        while(!inFile.eof())
        {
        std::getline(inFile,str,'$');
        str.append("$");
        str.append("\0");
        //cout<<str;
        char *buf=const_cast<char*>(str.c_str());
        doccount++;
        //cout<<doccount<<"\n";
        docPos=docPos+buflength;
        buflength=strlen(buf);
        docEndPos=docPos+buflength-1;
       //cout<<buf<<"\n";
        int pos=docPos;
        while(*buf!='$')
	{
        //calls createBranch with all suffixes of the document
        matches=0;
        //cout<<buf<<"\n\n";
        createBranch(current,buf,pos,docEndPos);
        buf++;
        pos++;
        }
       createBranch(current,buf,pos,docEndPos);

  }
      
   // cout<<doccount;
    inFile.close();
    return 0;


}
void f(suffixtree* current)
{
int k;
for(k=0;current->child[k]!=NULL;k++)
{
cout<<"\n"<<"child"<<k;
printEdge(current->child[k]->e.start,current->child[k]->e.end);
printChildren(current->child[k]);
}
}
};

int readFile(char *filename)
{
    //reading file character by character to store into an array
    FILE* inFile=fopen(filename,"r");
    char ch;
    char *fileptr =(char*)malloc(sizeof(char)*1000);
    if (!inFile) {
        cout << "Unable to open file";
        return -1;
    }
    int charCount=0; //keeps track of characters read to realloc 
    int allocCount=2;
    
    if(fileptr==NULL) {cout<<"\n\nMALLOC RETURNED NULL\n\n";return -1;}
    int i=0;
    while ((ch=getc(inFile))!=EOF) {
     fileptr[i]=ch;
     //cout<<fileptr[i];
     charCount++;    
     if(charCount==1000)
       {
       //cout<<"\n\n";
       fileptr=(char*)realloc(fileptr,sizeof(char)*1000*allocCount);
       if(fileptr==NULL) {cout<<"\n\nREALLOC RETURNED NULL\n\n";return -1;}
       allocCount++;
       charCount=0;
       }
     i++;
    }
    
    fileArr=fileptr;
    fclose(inFile);
    return 0;    

}


int main() {

    struct timespec start,end;
    readFile("cleanAesop.txt");
    //cout<<fileArr;
    suffixtree *root=new suffixtree();
    cout<<"\n********************************************************\n";
    cout<<"CONSTRUCTING TREE...\n\n";
   // clock_gettime(CLOCK_REALTIME,&start);
    root->construct("cleanAesop.txt");
   // clock_gettime(CLOCK_REALTIME,&end);
    //printf("%ld.%ld\n",start.tv_sec,start.tv_nsec);
    //printf("%ld.%ld\n",end.tv_sec,end.tv_nsec);
   // printf("%ld",(end.tv_nsec-start.tv_nsec)/1000);
    cout<<"\n";
    cout<<"Enter your query string: ";
    
    string str;
    std::getline(cin,str,'\n');
    char *query=const_cast<char*>(str.c_str());
    char ch;
    //root->f(root);
    //root->levelTraverse(root);
    // cout<<"\n"<<"\n"<<"CHILDREN\n\n";
    // root->printChildren(root);
    cout<<"\n********************************************************\n";
    cout<<"FIRST OCCURRENCE\n";
   // clock_gettime(CLOCK_REALTIME,&start);
    int num= root->search(query);
    root->findTitle(num);
    root->findMatchedText(query,num);
   // clock_gettime(CLOCK_REALTIME,&end);
   // printf("%lld",(end.tv_nsec-start.tv_nsec)/1000);
    cout<<"\n\n";
    cout<<"Press y to get the titles of all documents containing this string. ";
    cin>>ch; 
    if(ch!='y') return 0;
    
    cout<<"\n********************************************************\n";
    cout<<"OTHER DOCUMENTS CONTAINING THIS STRING\n\n";
    //clock_gettime(CLOCK_REALTIME,&start);
    int ret=root->searchForAll(query);
   // clock_gettime(CLOCK_REALTIME,&end);
  //  printf("%ld",(end.tv_nsec-start.tv_nsec)/1000);
    cout<<"\n\n";
 
    cout<<"Press y to get all occurrences of this string. ";
    cin>>ch; 
    if(ch!='y') return 0;
    
    cout<<"\n";
    cout<<"\n********************************************************\n";
    cout<<"ALL OCCURRENCES\n\n";
   // clock_gettime(CLOCK_REALTIME,&start);
    if(ret==-1)
        root->findMatchedText(query,num);
   
    else
        root->findAllOccurrences(query);
  
    cout<<"\n";
  //  clock_gettime(CLOCK_REALTIME,&end);
  //  printf("%ld",(end.tv_nsec-start.tv_nsec)/1000);
    cout<<"\n********************************************************\n";
   

    free(fileArr);
    return 0;
}
