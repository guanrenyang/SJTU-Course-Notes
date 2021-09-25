#include <iostream>
#include <cstdlib>
using namespace std;
void printTable(int n) ;
// upper bound on hashtable size
#define MAXN 101
// actual used hashtable size
int m;
//10 20 50 53 75 100 67 105 3 36 39
// number of tables
#define ver 2 

int hashtable[ver][MAXN]; 

// possible positions for a key 
int pos[ver]; 

/* function to fill hash table with dummy value 
* dummy value: -1
* number of hashtables: ver */
void initTable() 
{ 
	for (int j=0; j<MAXN; j++) 
		for (int i=0; i<ver; i++) 
			hashtable[i][j] = -1; 
} 

/* return hashed value for a key 
* function: ID of hash function
* key: item to be hashed */
int myhash(int function, int key, int n) 
{ 
	switch (function) 
	{ 
		case 1: return key%m; 
		case 2: return (key/m)%m; 
	} 
} 

void place(int key, int tableID, int cnt, int n) 
{
    int h1=myhash(1,key,n);
    int h2=myhash(2,key,n);
    if(cnt>10*n) 
    {
        cout<<"Failed!";
        exit(1);
    }
    switch(tableID)
    {
        case 0:
        {   
            if(hashtable[tableID][h1]==-1)//如果表1对应位置空，那么直接插入
                hashtable[tableID][h1]=key;
            else if(hashtable[tableID][h1]==key||hashtable[1][h2]==key)
                break;
            else //如果表1对应位置不空
            {
                int seatRobber=hashtable[tableID][h1];//保存占据位置的元素
                hashtable[tableID][h1]=-1;
                place(key,0,++cnt,n);//将此元素key放入表1
                place(seatRobber,1,++cnt,n);//将seatRobber放入表2
                
            }
            break;
        }
        case 1:
        {
            
            if(hashtable[tableID][h2]==-1)
                hashtable[tableID][h2]=key;
            else if(hashtable[tableID][h2]==key||hashtable[0][h1]==key)
                break;
            else
            {
                int seatRobber=hashtable[tableID][h2];//保存占据位置的元素
                hashtable[tableID][h2]=-1;
                place(key,1,++cnt,n);//将此元素key放入表2
                place(seatRobber,0,++cnt,n);//将seatRobber放入表2
                
            }
        }
    }

} 

/* function to print hash table contents */
void printTable(int n) 
{ 
	for (int i=0; i<ver; i++, std::cout<<endl) 
		for (int j=0; j<m; j++) 
			(hashtable[i][j]==-1)? std::cout<<"- ": 
					std::cout<<hashtable[i][j]<<" "; 

} 

/* function for Cuckoo-hashing keys 
* keys[]: input array of keys 
* n: size of input array */
void cuckoo(int keys[], int n) 
{ 
	//init
	initTable(); 

	// start with placing every key at its position in 
	// the first hash table according to first hash 
	// function 
	for (int i=0, cnt=0; i<n; i++, cnt=0) 
		place(keys[i], 0, cnt, n); 

	//print the final hash tables 
	printTable(n); 
} 

int main() 
{ 
	int keys_1[MAXN];
    //input array size
    int n;
	std::cin >> n;
    //m in hash function
	m = n + 1;
	int i=0;
    while(i < n)
    {
		std::cin >> keys_1[i++]; 
    }
	cuckoo(keys_1, n); 
	return 0; 
} 
