// Add any standard library you need.
#include <bits/stdc++.h>
using namespace std;

//
// Below is the class template provided by us.
// Feel free to reconstruct the code as long as its functions are realized.
//

// ==================== k-d tree ======================
struct kdNode
{
    int key[2];  // To store 2 dimensional value
    kdNode *left = nullptr;
    kdNode *right = nullptr;

    kdNode(int arr[])
    {
        key[0]=arr[0];
        key[1]=arr[1];
    }
};
class kdTree{
public:
    kdNode* root = nullptr;
    vector<struct kdNode*> rangeNode;
    kdTree();
    kdNode *minNode(kdNode * p1,kdNode *p2,int dim)
    {
        if(p1==NULL&&p2==NULL)
            return NULL;
        else if(p1==NULL&&p2!=NULL)
        {
            return p2;
        }
        else if(p1!=NULL&&p2==NULL)
        {
            return p1;
        }
        else 
        {
            return p1->key[dim]>p2->key[dim] ?p2:p1;
        }
    }
    // Insert a new node with 2 dimensional value `arr`.
    // `dim` refers to the dimension of the root.
    void insert(kdNode * & root, int arr[], int dim);
    void insert(kdNode * & root, kdNode* & item, int dim);
    // Search node with value `arr`.
    // Return NULL if not exist.
    kdNode* search(kdNode* root, int arr[], int dim);
    
    // Delete the node with value `arr`.
    void remove(kdNode* & root, int arr[], int dim);
    
    // Find the node with the minimum value in dimension `dimCmp`.
    // `dimCmp` equals to 0 or 1.
    kdNode* findMin(kdNode* root, int dimCmp, int dim);
    
    // Find a vector of nodes whose values are >= `lower_bound` and <= `upper_bound`.
    // `lower_bound` contains lower bound for each dimension of the target nodes.
    // `upper_bound`, on the contrary, holds upper bound per dimension.
    // e.g. given lower_bound=[1, 2], upper_bound=[3, 4], node=[2, 3] will be chosen, because 1<=2<=3, 2<=3<=4.
    void rangeSearch(kdNode* root, int dim, int lower_bound[], int upper_bound[],int t);
    vector<struct kdNode*> rangeSearch(kdNode* root, int dim, int lower_bound[], int upper_bound[]);
};


// ==================== k-d tree ======================
kdTree::kdTree(){
    this->root=NULL;
}
vector<struct kdNode*> kdTree::rangeSearch(kdNode* root, int dim, int lower_bound[], int upper_bound[])
{
    rangeSearch(root,dim,lower_bound,upper_bound,1);
    return rangeNode;
}
void kdTree::insert(kdNode * & root, int arr[], int dim){
    if(root==NULL)
    {
        root=new kdNode(arr);
    }
    else if(root->key[0]==arr[0]&&root->key[1]==arr[1])
    {
        return;
    }
    else if(arr[dim]<root->key[dim])
    {
        insert(root->left,arr,!dim);
    }
    else if(arr[dim]>root->key[dim])
    {
        insert(root->right,arr,!dim);
    }
}
void kdTree::insert(kdNode * & root, kdNode* & item, int dim)
{
    if(root==NULL)
    {
        root=item;
    }
    else if(item!=NULL&&root->key[0]==item->key[0]&&root->key[1]==item->key[1])
    {
        insert(root,item->left,dim);
        insert(root,item->right,dim);
    }
    else if(item!=NULL&&item->key[dim]<root->key[dim])
    {
        insert(root->left,item,!dim);
    }
    else if(item!=NULL&&item->key[dim]>root->key[dim])
    {
        insert(root->right,item,!dim);
    }
}

kdNode* kdTree::search(kdNode * root, int arr[], int dim){
    if(root==NULL)
    {
        return NULL;
    }
    else if(root->key[0]==arr[0]&&root->key[1]==arr[1])
    {
        return root;
    }
    else if(arr[dim]<root->key[dim])
    {
        return search(root->left,arr,!dim);
    }
    else if(arr[dim]>root->key[dim])
    {
        return search(root->right,arr,!dim);
    }
}

void kdTree::remove(kdNode* & root, int arr[], int dim){
    if((root!=NULL)&&root->left->key[0]==arr[0]&&root->left->key[1]==arr[1])
    {
        kdNode* r=root->left->right;
        kdNode* l=root->left->left;
        delete root->left;
        root->left=NULL;
        insert(this->root,r,0);
        insert(this->root,l,0);
    }
    else if((root!=NULL)&&((root->right)->key[0]==arr[0])&&((root->right)->key[1]==arr[1]))
    {
        delete root->right;
        root->right=NULL;
    }
    else if(arr[dim]<root->key[dim])
    {
        remove(root->left,arr,!dim);
    }
    else if(arr[dim]>root->key[dim])
    {
        remove(root->right,arr,!dim);
    }

}

kdNode* kdTree::findMin(kdNode* root, int dimCmp, int dim){
    if(!root) return NULL;
    kdNode *min = findMin(root->left, dimCmp, !dim); 
    if(dimCmp != dim) 
    {
        kdNode* rightMin =findMin(root->right, dimCmp, !dim); 
        min=minNode(min, rightMin, dimCmp);
    }
    return minNode(min, root, dimCmp);;
}

void kdTree::rangeSearch(kdNode* root, int dim, int lower_bound[], int upper_bound[],int t){
    if((root!=NULL)&&(lower_bound[dim]<=root->key[dim]&&root->key[dim]<=upper_bound[dim])&&(lower_bound[!dim]<=root->key[!dim]&&root->key[!dim]<=upper_bound[!dim]))
        rangeNode.push_back(root);
    if((root->left!=NULL)&&lower_bound[dim]<=root->left->key[dim]&&root->left->key[dim]<=upper_bound[dim])
        rangeSearch(root->left,!dim,lower_bound,upper_bound,1);
    if((root->right!=NULL)&&lower_bound[dim]<=root->right->key[dim]&&root->right->key[dim]<=upper_bound[dim])
        rangeSearch(root->right,!dim,lower_bound,upper_bound,1);
}

int main()
{
    const int k = 2;
    kdTree* obj = new kdTree();
    int points[][k] = {{30, 40}, {5, 25}, {70, 70}, {10, 12}, {50, 30}, {35, 45}};

    /********************initial insert************************/
    obj->insert(obj->root, points[0], 0);
    obj->insert(obj->root, points[1], 0);
    obj->insert(obj->root, points[2], 0);
    obj->insert(obj->root, points[3], 0);
    kdNode* res1 = obj->search(obj->root, points[2], 0);
    /********************verify res1: 70 70****************/
    if(res1 == NULL)
        cout << "error!" << endl;
    else
        cout << res1->key[0] << " " << res1->key[1] << endl;

    obj->remove(obj->root, points[2], 0);
    kdNode* res2 = obj->search(obj->root, points[2], 0);
    /********************verify res2: NULL*****************/
    if(res2 != NULL)
        cout << "error!" << endl;
    else
        cout << "ok!" << endl;

    kdNode* res3 = obj->findMin(obj->root, 1, 0);
    /********************verify res3: 10 12****************/
    if(res3 == NULL)
        cout << "error!" << endl;
    else
        cout << res3->key[0] << " " << res3->key[1] << endl;

    int bounds[][k] = {{6, 15}, {50, 50}};
    vector<kdNode*> res4 = obj->rangeSearch(obj->root, 0, bounds[0], bounds[1]);
    /********************verify res4: 30 40****************/
    for(int i=0; i<res4.size(); ++i)
        cout << res4[i]->key[0] << " " << res4[i]->key[1] << endl;
    return 0;
}