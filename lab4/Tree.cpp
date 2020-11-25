#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>

#include "Tree.h"

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/
int layer = 0;

bool compare(TreeNode* n1, TreeNode* n2) {
    if (layer % 2 == 0) {
        return n1->getX() < n2->getX();
    }
    else {
        return n1->getY() < n2->getY();
    }
}

TreeNode::TreeNode(int x, int y, int i) {
    data[0] = x;
    data[1] = y;
    index = i;
    left = NULL;
    right = NULL;
}

int TreeNode::getIndex() {
    return index;
}

int TreeNode::getX() {
    return data[0];
}

int TreeNode::getY() {
    return data[1];
}

TreeNode::~TreeNode() {

}

BinaryDimonTree::BinaryDimonTree() {
    root = NULL;
}

void TreeNode::setIndex(int i) {
    index = i;

}
void BinaryDimonTree::insert(TreeNode *t) {
    if (root == NULL) {
        root = t;
    }
    else {
        TreeNode* pos = root, * father = root;
        bool flag = false;
        while (true) {
            father = pos;
            if (father->getIndex() == 0) {
                if (t->getX() < pos->getX()) {
                    pos = pos->left;
                    flag = false;
                }
                else {
                    pos = pos->right;
                    flag = true;
                }
            }
            else {
                if (t->getY() < pos->getY()) {
                    pos = pos->left;
                    flag = false;
                }
                else {
                    pos = pos->right;
                    flag = true;
                }
            }
            if (pos == NULL) {
                t->setIndex(1 - father->getIndex());
                pos = t;
                if (flag) {
                    father->right = pos;
                }
                else {
                    father->left = pos;
                }
                break;
            }
        }
    }
}

TreeNode *BinaryDimonTree::find_nearest_node(int x, int y) {
    long long int init_dis = 9223372036854775807;
    TreeNode* guess = NULL;
    BinaryDimonTree::recur_search(root, x, y, init_dis, guess);
    return guess;
}

void BinaryDimonTree::recur_search(TreeNode* cur, int x, int y, long long int& min_distance, TreeNode*& guess) {
    if (cur == NULL) {
        return;
    }
    long long int current_distance = sqr_distance(x, y, cur->getX(), cur->getY());
    if ( current_distance< min_distance) {
        guess = cur;
        min_distance = current_distance;
    }
    else if (current_distance == min_distance) {
        if (cur->getX() < guess->getX()) {
            guess = cur;
        }
        else if (cur->getX() == guess->getX()) {
            if (cur->getY() < guess->getY()) {
                guess = cur;
            }
        }
    }

    int index = cur->getIndex();
    if (index == 0) {
        long long int tmp = (x - cur->getX()) * (x - cur->getX());
        if (x < cur->getX()) {
            recur_search(cur->left, x, y, min_distance, guess);
            if (tmp < min_distance) {
                recur_search(cur->right, x, y, min_distance, guess);
            }
        }
        else {
            recur_search(cur->right, x, y, min_distance, guess);
            if (tmp < min_distance) {
                recur_search(cur->left, x, y, min_distance, guess);
            }
        }
    }
    else {
        long long int tmp = (y - cur->getY()) * (y - cur->getY());
        if (y < cur->getY()) {
            recur_search(cur->left, x, y, min_distance, guess);
            if (tmp < min_distance) {
                recur_search(cur->right, x, y, min_distance, guess);
            }
        }
        else {
            recur_search(cur->right, x, y, min_distance, guess);
            if (tmp < min_distance) {
                recur_search(cur->left, x, y, min_distance, guess);
            }
        }
    }
}

long long int BinaryDimonTree::sqr_distance(int x, int y, int X, int Y) {
    return (X - x) * (X - x) + (Y - y) * (Y - y);
}

void build(vector<TreeNode*> *input,int begin, int end, BinaryDimonTree& tree,int i) {
    if (begin > end) {
        return;
    }
    layer = i;
    i++;
    int mid = (end+begin)/2 ;
    nth_element(input->begin() + begin, input->begin() + mid, input->begin() + end+1 );
    tree.insert(*(input->begin() + mid));
    build(input,begin, mid-1 ,tree,i);
    build(input,mid+1, end,tree,i);
}
istream& operator>>(istream& in, BinaryDimonTree& tree) {
    int node_num,x,y;
    int num = 0;
    TreeNode* pos;
    in >> node_num;
    vector<TreeNode*> *input=new vector<TreeNode*>();
    for (int i = 0; i < node_num; i++) {
        in >> x >> y;
        pos=new TreeNode(x, y, 0);
        input->push_back(pos);
    }
    build(input,0, input->size()-1, tree,0);
    delete input;
    return in;
}

BinaryDimonTree::~BinaryDimonTree() { 
    recur_delete(root); 
}

void BinaryDimonTree::recur_delete(TreeNode* cur) {
    if (cur == nullptr) return;
    recur_delete(cur->left);
    recur_delete(cur->right);
    delete cur;
    return;
}

