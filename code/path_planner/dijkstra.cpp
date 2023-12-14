#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "ysglfontdata.h"
#include "fssimplewindow.h"
#include <iostream>
#include <limits>
#include <cmath>

struct Node{
    int x,y;
    std::string name = "";
    std::vector<Node*> neighbors;

    Node(int ipX,int ipY, std::string ipName){
        x = ipX; y = ipY; name = ipName;
    }
};

float getEdgeCost(const Node& fNode,const Node& sNode){
    float curDist = sqrt((sNode.y - fNode.y)*(sNode.y - fNode.y) + (sNode.x - fNode.x)*(sNode.x - fNode.x));
    return curDist;
}

struct PathInfo {
    Node* prev;
    float distance;
    bool visited;

    PathInfo(){
        prev = NULL;
        distance = std::numeric_limits<float>::infinity();
        visited = false;
    }
};


struct NodeFloatPair {
    Node* node;
    float dist;

    NodeFloatPair(Node* nodePtr, float newDist){
        node = nodePtr;
        dist = newDist;
    }
    NodeFloatPair(){}
};

struct NodeFloatPairComparator{
bool operator()(const NodeFloatPair& lhs, const NodeFloatPair& rhs) const {
    return lhs.dist > rhs.dist; 
}
};


std::vector<Node*> dijkstra(Node& startNode,Node& endNode){
    std::vector<Node*> path;
    std::unordered_map<Node*,PathInfo> traveledNodes;
    std::priority_queue<NodeFloatPair, std::vector<NodeFloatPair>,NodeFloatPairComparator> pq;
    NodeFloatPair curNode;
    float curDist;
    Node* curPrev = &endNode;

    pq.push(NodeFloatPair(&startNode,0));
    while(!pq.empty()){
        curNode = pq.top();
        pq.pop();

        // std::cout << "Current Keys:";
        // for (const auto& entry : traveledNodes) {
        // std::cout<< entry.first->name<<" ";
        // }
        // std::cout<<""<<std::endl;

        if (traveledNodes.find(curNode.node) == traveledNodes.end()){
            // std::cout<<"Adding node to dictionary: "<<curNode.node->name<<std::endl;
            traveledNodes[curNode.node] = PathInfo();
        }

        if ((curNode.dist > traveledNodes[curNode.node].distance) | (traveledNodes[curNode.node].visited == true)){
            // std::cout<<"Skipping "<<curNode.node->name<<std::endl;
            // pq.pop();
            continue;
        }
        else{
            traveledNodes[curNode.node].distance = curNode.dist;
        }
        // std::cout<<"CurNode: "<<curNode.node->name<<std::endl;
        traveledNodes[curNode.node].visited = true;

        for (Node* neighbor : curNode.node->neighbors){
        if (traveledNodes.find(neighbor) == traveledNodes.end()){
            traveledNodes[neighbor] = PathInfo();
            // std::cout<<"Adding node to dictionary: "<<neighbor->name<<std::endl;

            // std::cout<<"Current Neighbour:"<<neighbor->name<<", Current Distance:"<<traveledNodes[neighbor].distance<<std::endl;
        }   
        
        curDist = getEdgeCost(*curNode.node,*neighbor);
        if (curDist < traveledNodes[neighbor].distance){
            traveledNodes[neighbor].distance = curDist;
            if (traveledNodes[curNode.node].prev != neighbor){
                traveledNodes[neighbor].prev = curNode.node;
            }
            // std::cout<<"Adding: Name: "<< neighbor->name<< " Distance:"<<curDist<<std::endl;
            pq.push(NodeFloatPair(neighbor,curDist));
        }
        }

        // for (const auto& entry : traveledNodes) {
        //     std::cout << "Key: " << entry.first->name << ", Value: "<< traveledNodes[entry.first].distance << std::endl;
        // }
        

        if (curNode.node == &endNode){
            // std::cout<< curNode.node->name<<" Done Breaking!!!"<<std::endl;
            break;
        }


        // std::priority_queue<NodeFloatPair, std::vector<NodeFloatPair>,NodeFloatPairComparator> copypq = pq;
        // std::cout<<"Priority Queue Elements After Pop: "<<std::endl;
        //  while(copypq.size()!=0){
        //     std::cout<<"PQ Node Name:"<<copypq.top().node->name<<" PQ Node Distance: "<<copypq.top().dist<<std::endl;
        //     copypq.pop();
        // }
        // std::cout<<""<<std::endl;
    }

    // std::cout << "Final Current Keys:";
    // for (const auto& entry : traveledNodes) {
    // std::cout<< entry.first->name<<" ";
    // }
    // std::cout<<""<<std::endl;

    // std::cout<<"Broke out of while"<<std::endl;
    do{
        // std::cout<<curPrev->name<<std::endl;
        path.insert(path.begin(),curPrev);
        curPrev = traveledNodes[curPrev].prev;
    }while(curPrev != NULL);
    // std::cout<<""<<std::endl;
    // path.push_back(&endNode);
    return path;
}



std::vector<Node*> pathPlanner(Node& lastNode, Node& orderStartNode,Node& orderEndNode){
    std::vector<Node*> finalPath;
    // std::cout<<"First Call!!!"<<std::endl;
    std::vector<Node*> path1 = dijkstra(lastNode, orderStartNode);
    // std::cout<<"Path 1:";
    // for(const Node* curNode : path1){
    //     std::cout<<curNode->name<<"->";
    // }
    // std::cout<<""<<std::endl;


    // std::cout<<"Second Call!!!"<<std::endl;
    std::vector<Node*> path2 = dijkstra(orderStartNode, orderEndNode);
    // std::cout<<"Path 2:";
    // for(const Node* curNode : path2){
    //     std::cout<<curNode->name<<"->";
    // }
    // std::cout<<""<<std::endl;

    finalPath.insert(finalPath.end(),path1.begin(),path1.end());
    finalPath.insert(finalPath.end(),path2.begin(),path2.end());

    std::cout<<"Final Path:";
    for(const Node* curNode : finalPath){
        std::cout<<curNode->name<<"->";
    }
    std::cout<<""<<std::endl;
    return finalPath;
}

void drawNetwork(std::vector<Node>& nodes){
    glPointSize(8);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
    for(const Node& node: nodes){
        glVertex2i(node.x,node.y);
    }
    glEnd();

    for(const Node& node: nodes){
        glRasterPos2i(node.x,node.y - 10);
        YsGlDrawFontBitmap6x7(node.name.c_str());
    }


    glBegin(GL_LINES);
    glColor3f(0,0,0);
    for(const Node& node: nodes){
        for(const Node* neighbor : node.neighbors){
            glVertex2i(node.x,node.y);
            glVertex2i(neighbor->x,neighbor->y);
        }
    }
    glEnd();


    for(const Node& node: nodes){
        for(const Node* neighbor : node.neighbors){
            glRasterPos2i((node.x + neighbor->x)/2,(node.y + neighbor->y)/2 + 10);
            YsGlDrawFontBitmap6x7(std::to_string(getEdgeCost(node,*neighbor)).c_str());
        }
    }

}

void drawDijkstra(std::vector<Node*>& path){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    for(int i = 0 ; i < path.size() - 1; i++){
        glVertex2i(path[i]->x,path[i]->y);
        glVertex2i(path[i+1]->x,path[i+1]->y);
    }
    glEnd();
}


void takeInputForPathPlanning(int &lastNode, int &startNode, int &endNode) {
    std::cout << "Enter end node of the previous order node: ";
    std::cin >> lastNode;

    std::cout << "Enter start node of the next order node: ";
    std::cin >> startNode;

    std::cout << "Enter end node of the next order node: ";
    std::cin >> endNode;
}


void sleepTime(int timeSleep){
    auto t0=time(NULL);
    while(time(NULL)-t0<timeSleep)
    {
    }
}

int main(void){
    std::vector<Node*> path;
    int lastNode,startNode,endNode;
    bool IP = false;

    FsOpenWindow(100,100,600,600,1);

    std::vector<Node> nodes = {
        Node(100, 200, "Node1"),
        Node(300, 400, "Node2"),
        Node(150, 550, "Node3"),
        Node(450, 300, "Node4"),
        Node(200, 100, "Node5"),
        Node(550, 500, "Node6"),
        Node(400, 150, "Node7"),
        Node(100, 550, "Node8"),
        Node(300, 100, "Node9"),
        Node(500, 400, "Node10")
    };

    nodes[0].neighbors = {&nodes[1], &nodes[2], &nodes[4], &nodes[8]};
    nodes[1].neighbors = {&nodes[0], &nodes[3], &nodes[5], &nodes[8]};
    nodes[2].neighbors = {&nodes[0], &nodes[5], &nodes[7]};
    nodes[3].neighbors = {&nodes[1], &nodes[4], &nodes[6], &nodes[9]};
    nodes[4].neighbors = {&nodes[0], &nodes[3], &nodes[6], &nodes[8]};
    nodes[5].neighbors = {&nodes[1], &nodes[2], &nodes[9]};
    nodes[6].neighbors = {&nodes[3], &nodes[4], &nodes[9]};
    nodes[7].neighbors = {&nodes[2], &nodes[8]};
    nodes[8].neighbors = {&nodes[0], &nodes[1], &nodes[4], &nodes[7]};
    nodes[9].neighbors = {&nodes[3], &nodes[5], &nodes[6]};


    // # Test For All Nodes

    for(int lastNode = 0; lastNode < 10; lastNode++){
        for(int startNode = 0; startNode < 10; startNode++){
            for(int endNode = 0; endNode < 10; endNode++){
                std::cout<<"Testing Input Path: "<<nodes[lastNode].name<<"->"<<nodes[startNode].name<<"->"<<nodes[endNode].name<<std::endl;
                path = pathPlanner(nodes[lastNode],nodes[startNode],nodes[endNode]);
                path.empty();
            }
        }
    }
    std::cout<<"Successfully passed all tests."<<std::endl;
    

    // takeInputForPathPlanning(lastNode,startNode,endNode);
    // path = pathPlanner(nodes[lastNode],nodes[startNode],nodes[endNode]);
    std::cout<<"Press 0 while on the GUI Window to enter input nodes"<<std::endl;


    for(;;){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        FsPollDevice();
        auto key = FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

        else if (FSKEY_0 == key){
            takeInputForPathPlanning(lastNode,startNode,endNode);
            std::cout<<"Input Path: "<<nodes[lastNode].name<<"->"<<nodes[startNode].name<<"->"<<nodes[endNode].name<<std::endl;
            path.empty();
            path = pathPlanner(nodes[lastNode],nodes[startNode],nodes[endNode]);
            IP = true;
        }
        // std::cout<<"IP:"<<IP<<std::endl;
        drawNetwork(nodes);
        if (IP == true)  {
        drawDijkstra(path);
        }      
        FsSwapBuffers();
    }

    return 0;
}




// Stuck
// 
// 
