#pragma once

#include <algorithm>


namespace ef {

	template<typename K, typename V>
	class TreeMap
	{
	public:
		TreeMap() : m_root(nullptr), m_size(0) {

		}

		~TreeMap() {
			clear();
		}

		V& operator[](const K& key) {
			V* found = searchNode(key, m_root);
			if (found) return *found;

			m_root = insertNode(key, V{}, m_root);
			return *searchNode(key, m_root);
		}

		void insert(const K& key, const V& value) {
			m_root = insertNode(key, value, m_root);
		}

		V* search(const K& key) const {
			return searchNode(key, m_root);
		}

		int size() const {
			return m_size;
		}

		bool empty() const {
			return m_size == 0;
		}

		bool contains(const K& key) const {
			return searchNode(key, m_root) != nullptr;
		}

		bool remove(const K& key) {
			bool removed = false;
			m_root = removeNode(key, m_root, removed);
			return removed;

		}

		void clear() {
			clearNode(m_root);
			m_root = nullptr;
			m_size = 0;
		}

	private:
		struct Node {
			K key;
			V value;
			Node* left = nullptr;
			Node* right = nullptr;
			int height = 0;

			Node(const K& key, const V& value) : key(key), value(value) {};
		};

		int getNodeHeight(Node* node) const {
			if (node == nullptr)
				return -1;

			return node->height;
		}

		void updateNodeHeight(Node* node) {
			if (node == nullptr)
				return;

			node->height = 1 + std::max(getNodeHeight(node->left), getNodeHeight(node->right));
		}

		int getBalanceFactor(Node* node) const {
			if (node == nullptr)
				return 0;

			return getNodeHeight(node->left) - getNodeHeight(node->right);
		}

		Node* rotateLeft(Node* node) {
			Node* nodex = node->right;
			Node* orphan = nodex->left;

			nodex->left = node;
			node->right = orphan;

			updateNodeHeight(node);
			updateNodeHeight(nodex);

			return nodex;
		}

		Node* rotateRight(Node* node) {
			Node* nodex = node->left;
			Node* orphan = nodex->right;

			nodex->right = node;
			node->left = orphan;

			updateNodeHeight(node);
			updateNodeHeight(nodex);

			return nodex;
		}

		Node* rebalance(Node* node) {
			updateNodeHeight(node);
			int bf = getBalanceFactor(node);

			// Left-heavy
			if (bf >= 2) {
				if (getBalanceFactor(node->left) < 0) {
					node->left = rotateLeft(node->left);
				}
				return rotateRight(node);
			}

			// Right-heavy
			if (bf <= -2) {
				if (getBalanceFactor(node->right) > 0) {
					node->right = rotateRight(node->right);
				}
				return rotateLeft(node);
			}
			return node;
		}


		V* searchNode(const K& key, Node* node) const {
			if (node == nullptr)
				return nullptr;

			if (key == node->key)
				return &node->value;

			if (key < node->key)
				return searchNode(key, node->left);
			else
				return searchNode(key, node->right);
		}

		Node* insertNode(const K& key, const V& value, Node* node) {
			if (node == nullptr) {
				m_size++;
				return new Node(key, value);
			}

			if (key < node->key) {
				node->left = insertNode(key, value, node->left);
			}
			else if (key > node->key) {
				node->right = insertNode(key, value, node->right);
			}
			else {
				node->value = value;
				return node;
			}

			return rebalance(node);
		}

		Node* findSuccessor(Node* node) const {
			while (node->left != nullptr) 
				node = node->left;
			return node;
		}

		Node* removeNode(const K& key, Node* node, bool& removed) {
			if (node == nullptr) return nullptr;

			if (key < node->key) {
				node->left = removeNode(key, node->left, removed);
			}
			else if (key > node->key) {
				node->right = removeNode(key, node->right, removed);
			}
			else {
				removed = true;
				m_size--;

				if (node->left == nullptr || node->right == nullptr) {
					Node* child = (node->left != nullptr) ? node->left : node->right;
					delete node;
					return child;
				}
				else {
					Node* successor = findSuccessor(node->right);
					node->key = successor->key;
					node->value = successor->value;

					bool placeholder = false;
					node->right = removeNode(successor->key, node->right, placeholder);

					m_size++;
				}
			}

			return rebalance(node);
		}


		void clearNode(Node* node) {
			if (node == nullptr)
				return;

			clearNode(node->left);
			clearNode(node->right);

			delete node;
		}

		Node* m_root;
		int m_size = 0;
	};
}
