#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>

#include "hlt.hpp"
#include "networking.hpp"

using namespace hlt;

Site findClosestSite(Site origin)
{
    std::queue<Site> Q;
    std::vector<Site> children;
    int distance;
    
    Q.push(origin);
    
    while(!Q.empty())
    {
        Site t = Q.front();
        distance += t.getDistance();
        
        Q.pop();
        
        if(t == goal){
            return ;
        }

    }
    return path;
}

std::vector<Site> getTileWithinDistance(int distance, Location origin) {
    
    std::vector<Location> tiles;
    
    for(unsigned short a = 0; a < presentMap.height; a++) {
        for(unsigned short b = 0; b < presentMap.width; b++) {
            if (getDistance({b, a}, {origin.x, origin.y}) == distance) {
                tiles.push_back({b, a});
            }
        }
    }
    std::vector<Site> sites;
    for (int i = 0; i < tiles.size(); i++) {
        sites.push_back(presentMap.getSite({tiles.at(i).x, tiles.at(i).y}));
    }
    return sites;
}


int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);

    unsigned char myID;
    hlt::GameMap presentMap;
    getInit(myID, presentMap);
    sendInit("MyC++Bot");

    std::set<hlt::Move> moves;
    while(true) {
        moves.clear();

        getFrame(presentMap);

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                hlt::Location t;
                t.x = b;
                t.y = a;
    
                if (presentMap.getSite({ b, a }).owner == myID) {

                    
                }
                
            }
//moves.insert({ { b, a }, (unsigned char)(rand() % 5) });
                    
            
                    //Site Grading Function
                    //Board State Evaluation Function
                    //Find Closest Border Function
                }
            }

        sendFrame(moves);
    }

    return 0;
}
