#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <set>
#include <fstream>
#include <queue>

#include "hlt.hpp"
#include "networking.hpp"

using namespace hlt;

//strength/production


/*std::vector<Site> getTilesWithinDistance(int distance, Location origin) {
    
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
}*/

unsigned char myID;
hlt::GameMap presentMap;

//lower is better
/*float evaluate (origin, destination) {
    if (destination.owner == 0) {
        return destination.production;
    }
    else if (destination.owner == myID){
        return destination.production;
    }
    else {
        return destination.strength/destination.production;
    }
}*/

Move assignMove(Location t) {
    if (presentMap.getSite(t).strength == 0) {
        return Move{t, STILL};
    }
    else {
    hlt::Location l;
        if (presentMap.getSite(t).strength < presentMap.getSite(t).production*5) {
            return Move{t, STILL};
        }
        else {
            //Breadth First Search
            if(presentMap.getSite(t).owner == myID) {
                std::queue<hlt::Location> bfs;
                std::set<hlt::Location> visited;
                bfs.push(t);
                do {
                    l = bfs.front();
                    visited.insert(l);
                    bfs.pop();
                    
                    for(int d : CARDINALS) {
                        hlt::Location n = presentMap.getLocation(l, d);
                        if(visited.count(n) == 0) bfs.push(n);
                    }
                } while(presentMap.getSite(l).owner == myID);
                if (t.y < l.y) {
                    return Move{t, SOUTH};
                }
                else if (t.y > l.y) {
                    return Move{t, NORTH};
                }
                else if (t.x < l.x) {
                    return Move{t, EAST};
                }
                else if (t.x > l.x) {
                    return Move{t, WEST};
                }
                else return Move{t, WEST};
            }
            else return Move{t, WEST};
        }
    }
    return Move{t, WEST};
}

int main() {
    srand(time(NULL));

    std::cout.sync_with_stdio(0);
    getInit(myID, presentMap);
    sendInit("MyC++Bot");

    std::set<hlt::Move> moves;
    while(true) {
        moves.clear();

        getFrame(presentMap);

        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                hlt::Location t = { b, a };
                if (presentMap.getSite(t).owner == myID) {
                    moves.insert(assignMove(t));
                }
            }
        }
        sendFrame(moves);
    }
    return 0;
}
