//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

class TreeNode
{
  //friend ostream &operator<<(ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;

private:
  /* data */
  int data[2];
  int index;
  TreeNode* left;
  TreeNode* right;

public:
  /* methods */
  TreeNode(int x, int y,int i);
  int getIndex();
  void setIndex(int i);
  int getX();  /* DO NOT CHANGE */
  int getY();  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */
};

class BinaryDimonTree
{
friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
    TreeNode* root;

public:
  /* methods */
  BinaryDimonTree();          /* DO NOT CHANGE */
  ~BinaryDimonTree();
  void recur_delete(TreeNode* cur);
  void insert(TreeNode *t);
  TreeNode *find_nearest_node(int x, int y);  /* DO NOT CHANGE */

  void recur_search(TreeNode *cur, int x, int y, long long int &min_distance, TreeNode *&guess);

  long long int sqr_distance(int x, int y, int X, int Y);

};

#endif //C_BINARYDIMEN_TREE_H
