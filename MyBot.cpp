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

int findNearestEnemyDirection(Location loc) {
    char direction = NORTH;
    // don't get stuck in an infinite loop
    int maxDistance = 900;
    
    for (int d = 0; d < 5; d++) {
        int distance = 0;
        Location current = loc;
        Site site = presentMap.getSite(current, d);
        while (site.owner == myID && distance < maxDistance) {
            distance++;
            current = presentMap.getLocation(current, d);
            site = presentMap.getSite(current);
        }
        
        if (distance < maxDistance) {
            direction = d;
            maxDistance = distance;
        }
    }
    
    return direction;
}

Move assignMove(Location t) {
    Site site = presentMap.getSite(t);
   if (site.strength <= site.production*6) {
        return Move{t, STILL};
    }
    else {
        return Move{t, static_cast<unsigned char>(findNearestEnemyDirection(t))};
    }
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
