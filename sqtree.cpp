/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"
#include <algorithm>
#include <iostream>
#include <string>

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
 /**
   * Constructor that builds a SQtree out of the given PNG.
   * Every Node in the tree corresponds to a rectangular region
   * in the original PNG, represented by an (x,y) pair for the 
   * upper left corner of the rectangle and an integer width and height.
   * In addition, the Node stores a pixel representing the average
   * color over the rectangle. It also stores a measure of color 
   * variability over the rectangle. The formula for this variability
   * is in the assignment description.
   *
   * Every node's children correspond to a partition
   * of the node's rectangle into at most four smaller rectangles.
   * The partition is made by splitting the node's rectangle
   * horizontally and vertically so that the resulting (at least
   * two non-empty) rectangles have the smallest maximum variability.
   * In other words, consider every possible split and chose the
   * one that minimizes the maximum variability among the (at most) four
   * rectangles.
   * A Node of size 1x1 has no children.  A Node of size 2x1 has only
   * one possible partition: into two 1x1 children.  A Node of size 3x1
   * has two possible partitions: A|BC and AB|C where ABC are the three
   * pixels in order. A Node of size 2x2 has three possible partitions:
   *    AB      A|B      A|B
   *    --  or  C|D  or  -+- 
   *    CD               C|D
   * In general, a Node of size w x h has w*h-1 partitions.
   * The constructor first builds the stats object used to compute
   * the variability and average of image subrectangles so that it can
   * pick the best partition of a Node of size w x h in O(wh) time.
   * Then it recursively partitions Nodes, starting at the root and
   * always using the best partition, until the Node's variability is
   * at most tol or the Node is a single pixel.
   * Default tolerance is 0.0.
   * 
   */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  if(!tol){
    tol = 0.0;
  }
  stats s(imIn);
  pair<int,int> t = make_pair(0,0);
  pair<int,int>& loc = t;
  root = buildTree(s, loc, imIn.width(), imIn.height(),tol);
  return;
}

 /**
   * Helper for the SQtree constructor.
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to calc avg and var
   * @param ul upper left point of current node's rectangle.
   * @param w,h are width and height of rectangle
   * @param tol is the allowed tolerance
   */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
           //do we need to insert tol = 0.0
  Node * node = new Node(s, ul, w, h);
  if(node == NULL) return NULL;
  if(w  == 0 || h == 0){
    return NULL;    
  }
  if(node->var <= tol || (w == 1 && h == 1)){
    return node;
  }
  double minVar = node->var;
  int bestX = 0;
  int bestY = 0;
  pair<int,int> ulp, ulp2, ulp3, ulp4;
  if(w == 1){
    for(int j = 1; j<h; j++){
        double var3 = 0, var4 = 0; 
        ulp3 = make_pair(ul.first, ul.second);
        ulp4 = make_pair(ul.first, ul.second+j);
        var3 = s.getVar(ulp3, (int)w, (int)j);
        var4 = s.getVar(ulp4, (int)w, (int)h-(int)j);
        double makemax = var3; 
        if(var4 >= makemax){
          makemax = var4; 
        }
        if(makemax <= minVar){
          minVar = makemax;
          bestX = 1;
          bestY = j;
        } 
    }   
    ulp3 = make_pair(ul.first, ul.second);
    ulp4 = make_pair(ul.first, ul.second+bestY);
    pair<int,int>& ulp3r = ulp3;
    pair<int,int>& ulp4r = ulp4;
    Node* nw = buildTree(s, ulp3r, bestX,bestY,tol);
    Node* sw = buildTree(s, ulp4r, bestX,(int)h-bestY,tol);
    node->NW = nw;
    node->SW = sw;
    return node;
  }
  else if(h == 1){
    for(int i = 1; i<w; i++){
        double var1 = 0, var2 = 0; 
        ulp = make_pair(ul.first, ul.second);
        ulp2 = make_pair(ul.first+i, ul.second);
        var1 = s.getVar(ulp, (int)i,(int)h);
        var2 = s.getVar(ulp2, (int)w-(int)i,(int)h);
        double makemax = var1; 
        if(var2 >= makemax){
          makemax = var2; 
        }
        if(makemax <= minVar){
          minVar = makemax;
          bestX = i;
          bestY = 1;
        } 
      }
    ulp = make_pair(ul.first,ul.second);
    ulp2 = make_pair(ul.first+bestX,ul.second);
    pair<int,int>& ulpr = ulp;
    pair<int,int>& ulp2r = ulp2;
    Node* nw = buildTree(s, ulpr, bestX,bestY,tol);
    Node* ne = buildTree(s, ulp2r, (int)w-bestX, bestY,tol);
    node->NW = nw;
    node->NE = ne;
    return node;
  }else{
    for(int j = 1; j<h; j++){
      for(int i = 1; i<w; i++){
        double var1 = 0, var2 = 0, var3 = 0, var4 = 0; 
        ulp = make_pair(ul.first, ul.second);
        ulp2 = make_pair(ul.first+i, ul.second);
        ulp3 = make_pair(ul.first, ul.second+j);
        ulp4 = make_pair(ul.first+i, ul.second+j);
        var1 = s.getVar(ulp, (int)i, (int)j);
        var2 = s.getVar(ulp2, (int)w-(int)i, (int)j);
        var3 = s.getVar(ulp3, (int)i, (int)h-(int)j);
        var4 = s.getVar(ulp4, (int)w-(int)i, (int)h-(int)j);
        double makemax = var1; 
        if(var2 >= makemax){
          makemax = var2; 
        }
        if(var3 >= makemax){
          makemax = var3; 
        }
        if(var4 >= makemax){
          makemax = var4; 
        }
        if(makemax <= minVar){
          minVar = makemax;
          bestX = i;
          bestY = j;
        } 
      }
    }       
    ulp = make_pair(ul.first, ul.second);
    ulp2 = make_pair(ul.first+bestX, ul.second);
    ulp3 = make_pair(ul.first, ul.second+bestY);
    ulp4 = make_pair(ul.first+bestX, ul.second+bestY);
    pair<int,int>& ulpr = ulp;
    pair<int,int>& ulp2r = ulp2;
    pair<int,int>& ulp3r = ulp3;
    pair<int,int>& ulp4r = ulp4;
    Node* nw = buildTree(s, ulpr, bestX,bestY,tol);//works
    Node* ne = buildTree(s, ulp2r, (int)w-bestX, bestY,tol);
    Node* sw = buildTree(s, ulp3r, bestX,(int)h-bestY,tol);
    Node* se = buildTree(s, ulp4r, (int)w-bestX, (int)h-bestY,tol);
    node->NW = nw;
    node->NE = ne;
    node->SW = sw;
    node->SE = se;
    return node;
  }  
  return NULL;
}

/** 
 * Render SQtree and return the resulting image.
   * Render returns a PNG image consisting of rectangles defined
   * by the leaves in the sqtree.  Draws every leaf node's rectangle
   * onto a PNG canvas using the average color stored in the node.
   */
PNG SQtree::render() {
  // Your code here.
  int w = root->width;
  int h = root->height;
  PNG* finalpic = new PNG((unsigned int)w,(unsigned int)h);
  helper_render(root,finalpic);
  PNG returnpic = *finalpic;
  delete finalpic;
  finalpic=NULL;
  // returnpic.writeToFile("images/out/testout.png");
  return returnpic;
}

void SQtree::helper_render(Node* node, PNG *&finalpic){
  if(node == NULL){
    return;
  }
  if(node->NW == NULL && node->NE == NULL && node->SE == NULL & node->SW == NULL){
    int ulfy = node->upLeft.second;
    int ulfx = node->upLeft.first;
    int hy = node->height;
    int hx = node->width;
    for(int j = ulfy; j < ulfy+hy; j++){
      for(int i = ulfx; i < ulfx+hx; i++){
        RGBAPixel* finalpix = finalpic->getPixel(i,j);
        *finalpix = node->avg;     
    }
  }
  return;
  }
  helper_render(node->NW, finalpic);
  helper_render(node->NE, finalpic);
  helper_render(node->SW, finalpic);
  helper_render(node->SE, finalpic);
  return;
}
/**
   * Destroys all dynamically allocatedmake memory associated with the
   * current SQtree class.
   * You may want a recursive helper function for this one.
   */
void SQtree::clear() {
  // Your code here.
  
  clear_helper(root);
  
  return;
}
/**
   * Copies the parameter other SQtree into the current SQtree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The SQtree to be copied.
   */
void SQtree::copy(const SQtree & other) {
  // Your code here.
  root = copy_helper(other.root);
  return;
}



/**
   * Return the number of nodes in the tree.
   * You may want a private helper for this.
   */
int SQtree::size() {
  // Your code here.
  int sz = get_node_size(root);
  return sz;
}

void SQtree::clear_helper(Node *& node){
  if(node == NULL){
    return;
  }
  if(node->NW == NULL && node->NE == NULL && node->SE == NULL && node->SW == NULL){
    delete node;
    node = NULL;
    return;
  }
  clear_helper(node->NW);
  clear_helper(node->NE);
  clear_helper(node->SW);
  clear_helper(node->SE);
  return;
}

SQtree:: Node* SQtree::copy_helper(Node* other){
  if(other == NULL){
    return NULL;
  }
  Node* newnode = other;
  newnode->NW = copy_helper(other->NW);
  newnode->NE = copy_helper(other->NE);
  newnode->SW = copy_helper(other->SW);
  newnode->SE = copy_helper(other->SE);
  return newnode;
}


int SQtree::get_node_size(Node* node){
  if (node == NULL){
    return 0;
  }
  return 1 + get_node_size(node->NW) + get_node_size(node->NE) + get_node_size(node->SE) + get_node_size(node->SW);
}


