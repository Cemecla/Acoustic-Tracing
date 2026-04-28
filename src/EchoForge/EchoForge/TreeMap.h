#pragma once

#include <math.h>


namespace ef {

	template<typename K, typename V>
	class TreeMap
	{
	public:
		TreeMap() {

		}

		void insert(K key, V value) {
			insertNode(K key, V value, m_root);
		}

		V search(K key) const {
			return searchNode(key, m_root);
		}

		int size() const {
			return m_size;
		}

		V remove(K key) {
			// removeNode(key, m_root);
		}

		void clear() {
			clearNode(m_root);
			m_root = nullptr;
		}

	private:
		struct Node {
			K key;
			V value;
			Node* left = nullptr;
			Node* right = nullptr;
			int height = 0;

			Node(K key, V value) : key(key), value(value) {

			}
		}

		int getBalanceFactor(Node* node) const {
			if (node == nullptr)
				return 0;

			return getNodeHeight(node->left) - getNodeHeight(node->right);
		}

		int getNodeHeight(Node* node) const {
			if (node == nullptr)
				return -1;

			return node->height;
		}

		void updateNodeHeight(Node* node) {
			node->height = 1 + max(getNodeHeight(node->left), getNodeHeight(node->right));
		}

		void rotateLeft(Node* node) {
			Node* nodex = node->right;
			Node* orphan = nodex->left;

			nodex->left = node;
			node->right = orphan;

			updateNodeHeight(node);
			updateNodeHeight(nodex);
		}

		void rotateRight(Node* node) {
			Node* nodex = node->left;
			Node* orphan = nodex->right;

			nodex->right = node;
			node->left = orphan;

			updateNodeHeight(node);
			updateNodeHeight(nodex);
		}


		V searchNode(K key, Node* node) const {
			if (node == nullptr)
				return nullptr;

			if (key == node->key)
				return node->value;

			if (key < node->key)
				return searchNode(key, node->left);
			else
				return searchNode(key, node->right);
		}

		void insertNode(K key, V value, Node* node) {
			if (key < node->key) {
				insertNode(key, value, node->left);
				m_size++;
			}

			else if (key > node->key)
				insertNode(key, value, node->right);

			else if (key == node->key) {
				node->value = value;
				updateNodeHeight(node);
			}

			else if (node == nullptr)
				&node = Node(key, value);

			int bf = getBalanceFactor(node);

			if (bf <= -2)
			{
				if (getBalanceFactor(node->left) <= 0) {
					rotateRight(node);
				}
				else
				{
					rotateLeft(node);
					rotateRight(node);
				}
			}
			else if(bf >= 2)
			{
				if (getBalanceFactor(node->right) >= 0) {
					rotateLeft(node);
				}
				else
				{
					rotateRight(node);
					rotateLeft(node);
				}
			}
		}

		V removeNode(K key, Node* node) {
			if (key < node->key) {
				removeNode(key, value, node->left);
			}

			else if (key > node->key)
				removeNode(key, value, node->right);

			else if (key == node->key) {
				// NOT COMPLETED BREAK THE TREE
				V value = node->value;
				delete node;
				return value;
			}

			else if (node == nullptr)
				return nullptr;
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
