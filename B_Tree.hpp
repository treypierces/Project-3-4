#ifndef B_TREE
#define B_TREE

/**
 * THIS IS CURRENTLY A VECTOR IMPLEMENTATION, NOT A B-TREE
 * 
 * I'm using it for testing
 * 
 * PLEASE IMPLEMENT A B-TREE
 * 
 * Thx 
 */
# include <vector>
template <typename T>
class B_Tree
{
private:
    std::vector<T*> root;
public:
    
B_Tree(){
    root = std::vector<T*>();
};
~B_Tree(){
    for(T* node : root){
        delete node;
    }
};


void insert(T* new_node){
    root.push_back(new_node);
}

T* find(int id){
    for(T* node : root){
        if(node->id == id){
            return node;
        }
    }
    return nullptr;
}

bool empty(){
    return root.empty();
}

};

#endif

