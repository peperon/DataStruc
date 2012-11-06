#include<random>
#include<utility>
using namespace std;

template<class T>
struct TreapNode
{
	T value;
	int heap_key;
	TreapNode<T> *left, *right, *parent;

	TreapNode(T v, int h_k, TreapNode<T>* p = 0, TreapNode<T>* l = 0 , TreapNode<T>* r = 0):
	value(v), heap_key(h_k), left(l), right(r), parent(p)
	{}

	~TreapNode()
	{
		delete left;
		delete right;
	}
};

template<class T>
class Treap
{
public:
	Treap():root(0) {}
	Treap(const Treap&);
	Treap<T>& operator = (const Treap&);
	~Treap(){ delete root; }

	void insert(T);
	TreapNode<T>* find(T) const;
	bool remove(T);
	bool isEmpty() const { return (root == 0); }
private:
	TreapNode<T>* root;
	TreapNode<T>* rightRotation(TreapNode<T>*);
	TreapNode<T>* leftRotation(TreapNode<T>*);	
	void copy(const TreapNode<T>*, TreapNode<T>*);

	TreapNode<T>* insert_p(TreapNode<T>*, T, int);
	TreapNode<T>* deleteRootOfSubtree(TreapNode<T>*);
	pair<TreapNode<T>*, bool > remove_p(TreapNode<T>*, T);
};

template<class T>
void Treap<T>::copy(const TreapNode<T>* from, TreapNode<T>* to)
{
	if(to == root)	
		root = new TreapNode<T>(from->value, from->heap_key);

	if(!from->left)
	{
		TreapNode<T>* leftNode = new TreapNode<T>(from->left->value, from->left->heap_key, to);
		to->left = leftNode;
		copy(from->left, leftNode);
	}

	if(!from->right)
	{
		TreapNode<T>* rightNode = new TreapNode<T>(from->right->value, from->right->heap_key, to);
		to->right = rightNode;
		copy(from->right, rightNode);
	}
}

template<class T>
Treap<T>& Treap<T>::operator=(const Treap& treap)
{
	if(this != &treap && !treap.isEmpty())
	{
		delete root;
		copy(treap.root, root);
	}
	return *this;
}

template<class T>
Treap<T>::Treap(const Treap<T>& treap)
{
	if(!treap.isEmpty())
		copy(treap.root, root);
}

template<class T>
TreapNode<T>* Treap<T>::rightRotation(TreapNode<T>* node)
{
	TreapNode<T>* left = node->left;
	node->left = left->right;
	left->right = node;
	left->parent = node->parent;
	node->parent = left;
        if (node->left) 
          node->left->parent = node;

	return left;
}

template<class T>
TreapNode<T>* Treap<T>::leftRotation(TreapNode<T>* node)
{
	TreapNode<T>* right = node->right;
	node->right = right->left;
	right->left = node;
	right->parent = node->parent;
	node->parent = right;
        if (node->right) {
          node->right->parent = node;
        }
	return right;
}

template<class T>
TreapNode<T>* Treap<T>::find(T value) const
{
	TreapNode<T>* curr = root;
	while(curr)
	{
		if(curr->value == value)
			return curr;
		if(curr->value > value)
			curr = curr->left;
		else
			curr= curr->right;
	}

	return 0;
}

template<class T>
TreapNode<T>* Treap<T>::insert_p(TreapNode<T>* node, T value, int key)
{
	if(node == NULL)	
		node = new TreapNode<T>(value, key);	
	else if(node->value < value)
	{
		node->right = insert_p(node->right, value, key);
		node->right->parent = node;
		if(node->heap_key < node->right->heap_key)
			node = leftRotation(node);
	}
	else if(node->value > value)
	{
		node->left = insert_p(node->left, value, key);
		node->left->parent = node;
		if(node->heap_key < node->left->heap_key)
			node = rightRotation(node);
	}

	return node;
}

template<class T>
void Treap<T>::insert(T value)
{
	int key = rand();
	root = insert_p(root, value, key);
}

template<class T>
TreapNode<T>* Treap<T>::deleteRootOfSubtree(TreapNode<T>* rootSubTree)
{
	if(rootSubTree->left == 0 && rootSubTree->right == 0)
	{
		delete rootSubTree;
		return 0;
	}
	if(rootSubTree->left == 0)
	{
		TreapNode<T>* right = rootSubTree->right;
		right->parent = rootSubTree->parent;
		rootSubTree->right = 0;
		delete rootSubTree;
		return right;
	}
	if(rootSubTree->right == 0)
	{
		TreapNode<T>* left = rootSubTree->left;
		left->parent = rootSubTree->parent;
		rootSubTree->left = 0;
		delete rootSubTree;
		return left;
	}

	if(rootSubTree->right->heap_key < rootSubTree->left->heap_key)
	{
		rootSubTree = rightRotation(rootSubTree);
		rootSubTree->right = deleteRootOfSubtree(rootSubTree->right);
	}
	else
	{
		rootSubTree = leftRotation(rootSubTree);
		TreapNode<T>* test = deleteRootOfSubtree(rootSubTree->left);
		rootSubTree->left = test;
	}

	return rootSubTree;
}

template<class T>
pair<TreapNode<T>*, bool > Treap<T>::remove_p(TreapNode<T>* root, T value)
{
	if(root == NULL)
		return pair<TreapNode<T>*, bool>(root, false);
	if(root->value == value)
	{
		TreapNode<T>* new_root = deleteRootOfSubtree(root);
		return pair<TreapNode<T>*, bool>(new_root, true);
	}

	pair<TreapNode<T>*, bool> result;
	if(root->value > value)
	{
		result = remove_p(root->left, value);
		root->left = result.first;
		return pair<TreapNode<T>*, bool>(root, result.second);
	}
	else
	{
		result = remove_p(root->right, value);
		root->right = result.first;
		return pair<TreapNode<T>*, bool>(root, result.second);
	}
}

template<class T>
bool Treap<T>::remove(T value)
{
	pair<TreapNode<T>*, bool> result = remove_p(root, value);
	root = result.first;
	return result.second;
}