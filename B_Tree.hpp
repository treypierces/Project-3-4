#ifndef B_TREE
#define B_TREE

/**
 * THIS IS CURRENTLY A BINARY TREE IMPLEMENTATION, NOT A B-TREE
 * 
 * I'm using it for testing
 * 
 * PLEASE IMPLEMENT A B-TREE
 * 
 * Thx 
 */

template <typename T>
class B_Tree
{
private:
    T* root;
public:
    
B_Tree(){
    root = nullptr;
};
~B_Tree(){
    free(root);
};


void insert(T* new_node){
    if(root == nullptr){
        root = new_node;
    } else {
        insert_helper(root, new_node);
    }
}

T* find(int id){
    T* current = root;
    while(current != nullptr){
        if(current->id == id){
            return current;
        } else if(id < current->id){
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

private:
    void insert_helper(T* current, T* new_node){
        if(new_node->id < current->id){
            if(current->left == nullptr){
                current->left = new_node;
            } else {
                insert_helper(current->left, new_node);
            }
        } else {
            if(current->right == nullptr){
                current->right = new_node;
            } else {
                insert_helper(current->right, new_node);
            }
        }
    }
};
#endif

