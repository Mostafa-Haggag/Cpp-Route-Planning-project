#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {
/*it inherrits from the model class which is provided bth eio 2d open street map example*/
  public:/*it has a sub class called nodes which interrits from the model struct called Node*/
    class Node : public Model::Node {
      public:
        /*model node struct has x and y coordinate and we extent this struct by also adding a pointer to the parent of thenode*/
        Node * parent = nullptr;
        float h_value = std::numeric_limits<float>::max();/*h value of the node*/
        float g_value = 0.0;/*g value of the node*/
        bool visited = false;/*boolean of the node used in the a star search */
        std::vector<Node *> neighbors;/*each node has this vector of neightbors and that will be populated with all of the neighbors of the node*/

        void FindNeighbors();/*this will populate this vector caled neighbors when it is callled*/
        float distance(Node other) const {
            return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
        }

        Node(){}/*default constructor*/
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
        /*we use this to construct of RouteModel node from an existing model node. index is useful tol keep track of the nodes and it*/
        /*is also helpful to keep track of the model to which this node belongs*/
      private:
        int index;/**/
        Node * FindNeighbor(std::vector<int> node_indices);/*used inside of the findneighbors funcitons aboe*/
        RouteModel * parent_model = nullptr;/*point to the route model to which this node belongs*/
    };
    /*constructor of the class which takes the xml which is the openstreet map data*/
    RouteModel(const std::vector<std::byte> &xml);
    Node &FindClosestNode(float x, float y);/*find the closest node in the model give some x and y coordinates*/
    auto &SNodes() { return m_Nodes; }/*return a vecotr of nodes.*/
    std::vector<Node> path;/*stor the final path from the starting to ending point*/
    
  private:
    void CreateNodeToRoadHashmap();/*this method creat a map from nodes to the roads that the belong to and that is useful */
    /*when we want to find the neighboring nodes for a given node */
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;/*hesh tables fih el model of road w fih index*/
    std::vector<Node> m_Nodes;/*this the vector of nodes in the model*/

};

#endif
