/******************************************************************************
 * File: RedBlackTree.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * A partial implementation of a balanced BST backed by a red/black tree. This
 * implementation only handles insertions, not deletions, since we didn't cover
 * deletions in class.
 *
 * Feel free to copy the code in here and use it however you see fit!
 */
#pragma once

#include <cstddef> // For std::size_t

class RedBlackTree {
public:
  /**
   * Constructs a new, empty red/black tree.
   *
   * Because no explicit initialization is required here, this constructor is
   * defined using the = default syntax, which uses the compiler-generated
   * default.
   */
  RedBlackTree() = default;
  
  /**
   * Frees all memory allocated by the red/black tree.
   */
  ~RedBlackTree();
  
  /**
   * Returns whether the given key is present in the tree.
   */
  bool contains(int key) const; 
  
  /**
   * Inserts the given key into the red/black tree. If the element was added,
   * this function returns true. If the element already existed, then this
   * function returns false and does not modify the tree.
   */ 
  bool insert(int key);
  
  /**
   * Returns the rank of the specified key, which is the number of elements
   * in the data set less than the key. That is, the rank of the smallest
   * element is 0, the rank of the largest element is n - 1, etc. For
   *
   * The given key may not be present in the red/black tree. In that case,
   * you should still return the number of smaller elements in the red/black
   * tree. For example, the rank of 137 in a red/black tree containing
   * 103, 161, 166, and 261 is 1, and the rank of 161 in that same tree is 1.
   *
   * TODO: Implement this function.
   */
  std::size_t rankOf(int key) const;
  
  /**
   * Returns the nth-smallest key in the red/black tree. The smallest element
   * is considered the 0th element, the smallest after that the 1st, etc.
   * If the red/black tree does not contain at least n elements, this function
   * should throw a std::runtime_error to report an error.
   *
   * For example, if the BST contains 103, 161, 166, and 261, then selecting
   * the 0th-smallest element would return 103, selecting the 3rd-smallest
   * element would return 261, and selecting the 599th-smallest element would
   * throw a std::runtime_error.
   *
   * TODO: Implement this function.
   */
  int select(std::size_t rank) const;
  
  /**
   * For testing and debugging purposes, prints out a representation of the
   * red/black tree
   */
  void printDebugInfo() const;

private:
  /* Type representing a color. */
  enum class Color {
    BLACK, RED
  };
  
  /* Map a color to a string, for debugging purposes. */
  static const char* colorToString(Color c) {
    if (c == Color::BLACK) return "black";
    if (c == Color::RED)   return "red";
    return "(?)";
  }

  struct Node {
    int    key;         // The key itself
    int    size;        //Self-explanatory
    Color  color;       // What color we are
    
    Node*  left;        // Left and right children
    Node*  right;
    
    Node*  parent;      // Parent pointer. This is used to simplify the insertion
                        // procedure, but isn't strictly necessary.
  };
  
  Node* root = nullptr;
  
  /* Rotates a node with its parent. */
  void rotateWithParent(Node* curr);
  
  /* Inserts a key into the tree without doing any fixups. Returns a pointer
   * to the newly-inserted node.
   */
  Node* insertKey(int key);
  
  /* Performs the fixup logic given the position of the node in need of fixing. */
  void fixupFrom(Node* node);
  
  /* Returns the sibling of a node. Since this is essentially a function that
   * works on nodes and doesn't require a receiver object, we mark it static.
   */
  static Node* siblingOf(Node* node);
  
  /* Prints debug information about the given node, indented appropriately. */
  void printDebugInfoRec(Node* node, unsigned indent) const;
  
  /* For simplicity, disallow copying. This is here simply to ensure that you
   * don't accidentally copy the tree without meaning to.
   */
  RedBlackTree(const RedBlackTree &) = delete;
  void operator= (RedBlackTree) = delete;
};
