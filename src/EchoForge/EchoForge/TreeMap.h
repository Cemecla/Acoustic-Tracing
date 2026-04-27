#pragma once


namespace ef {

	template<typename K, typename V>
	class TreeMap
	{
	public:
		TreeMap() {

		}

		void insert(K key, V value) {
			
		}

		V search(K key) const {
			return searchNode(key, peak);
		}

		int size() const {

		}

		V remove(K key) {

		}

		void clear() {
			clearNode(peak);
			peak = nullptr;
		}

	private:
		struct Node {
			K key;
			V value;
			Node* left = nullptr;
			Node* right = nullptr;
			int height;
		}

		int getNodeHeight(Node* node) const {

		}

		void rotateLeft(Node* node) {

		}

		void rotateRight(Node* node) {

		}

		V searchNode(K key, Node* node) const {
			if (node == nullptr)
				return nullptr;

			if (key == node->key)
				return node->value;

			V result = searchNode(key, node->left);

			if (result != nullptr)
				return result;

			V result = searchNode(key, node->right);
			
			return result;
		}

		void clearNode(Node* node) {
			if (node == nullptr)
				return;

			clearNode(node->left);
			clearNode(node->right);

			delete node;
		}

		Node* peak;
	
	};
}
