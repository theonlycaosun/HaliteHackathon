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
hlt::GameMap nextMap;
int totalStrength;

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
    int direction = NORTH;
    // don't get stuck in an infinite loop
    float maxDistance = 900;
    
    for (char d = 0; d < 5; d++) {
        float distance = 0;
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

int findNearestCombatDirection(Location loc) {
    int direction = NORTH;
    // don't get stuck in an infinite loop
    float maxDistance = std::max(presentMap.width, presentMap.height)/2;
    
    for (char d = 0; d < 5; d++) {
        float distance = 0;
        Location current = loc;
        Site site = presentMap.getSite(current, d);
        while (site.strength != 0 && distance < maxDistance) {
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

int bestAdjacentAttack(Location loc) {
    int answer = 0;
    float value = -1;
    
    for (int i = 1; i < 5; i++) {
        if (nextMap.getSite(nextMap.getLocation(loc, i)).owner != myID) {
            if (totalStrength > 200) {
                if ((float)((float)(nextMap.getSite(nextMap.getLocation(loc, i)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, i)).strength+0.01)) > value) {
                    answer = i;
                    value = (float)((float)(nextMap.getSite(nextMap.getLocation(loc, i)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, i)).strength+0.01));
                    Site node = nextMap.getSite(nextMap.getLocation(loc, i));
                    for (int j = 1; j < 5; j++) {
                        if (nextMap.getSite(presentMap.getLocation(loc, j)).owner != myID) {
                            if (nextMap.getSite(presentMap.getLocation(loc, j)).owner == 0) {
                                value += 0.1*(float)((float)(nextMap.getSite(nextMap.getLocation(loc, j)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, j)).strength+0.01));
                            }
                            else if (nextMap.getSite(nextMap.getLocation(loc, j)).owner != myID) {value += 0.4*(float)((float)(nextMap.getSite(nextMap.getLocation(loc, j)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, j)).strength+0.01));}
                            else value -= 0.02*(float)(nextMap.getSite(nextMap.getLocation(loc, j)).strength);
                        }
                    }
                }
            
            } else {
                if ((float)((float)(nextMap.getSite(nextMap.getLocation(loc, i)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, i)).strength+0.01)) > value) {
                    answer = i;
                    value = (float)((float)(nextMap.getSite(nextMap.getLocation(loc, i)).production)/(float)(nextMap.getSite(nextMap.getLocation(loc, i)).strength+0.01));
                }
            }
        }
    }
    return answer;
}

void updateNextMap(Location loc, char d) {
    Site current = presentMap.getSite(loc);
    Site next = nextMap.getSite(nextMap.getLocation(loc, d));
    
    if (next.owner == myID) {
        nextMap.getSite(nextMap.getLocation(loc, d)).strength += current.strength;
        presentMap.getSite(loc).strength = 0;
        //presentMap.getSite(t).strength > nextMap.getSite(presentMap.getLocation(t, findNearestEnemyDirection(t))).strength
    } else {
        if (current.strength >= next.strength) {
            nextMap.getSite(nextMap.getLocation(loc, d)).owner = myID;
            nextMap.getSite(nextMap.getLocation(loc, d)).strength = presentMap.getSite(loc).strength - nextMap.getSite(nextMap.getLocation(loc, d)).strength;
            presentMap.getSite(loc).strength = 0;
        }
    }
}


Move assignMove(Location t) {
    Site site = presentMap.getSite(t);
    if (site.strength == 0) {
         return Move{t, STILL};
    }
        bool isBorder = false;
        for (int i = 1; i < 5; i++) {
            if (nextMap.getSite(presentMap.getLocation(t, i)).owner != myID) {
                isBorder = true;
            }
        }
    
    if (site.strength != 255) {
        if (isBorder && site.strength > presentMap.getSite(presentMap.getLocation(t, bestAdjacentAttack(t))).strength) {
            int highestProductionNeighborDirection = bestAdjacentAttack(t);
            updateNextMap(t, static_cast<unsigned char>(highestProductionNeighborDirection));
            return Move{t, static_cast<unsigned char>(highestProductionNeighborDirection)};
        }
    }
    if (site.strength == 255){
        if (isBorder && site.strength >= presentMap.getSite(presentMap.getLocation(t, bestAdjacentAttack(t))).strength) {
            int highestProductionNeighborDirection = bestAdjacentAttack(t);
            updateNextMap(t, static_cast<unsigned char>(highestProductionNeighborDirection));
            return Move{t, static_cast<unsigned char>(highestProductionNeighborDirection)};
        }
    }
    
        
        if (site.strength <= site.production*5) {
            return Move{t, STILL};
        }
    
       if (nextMap.getSite(presentMap.getLocation(t, findNearestEnemyDirection(t))).owner == myID && nextMap.getSite(presentMap.getLocation(t, findNearestEnemyDirection(t))).strength + presentMap.getSite(t).strength < 300) {
           unsigned char fNED = static_cast<unsigned char>(findNearestEnemyDirection(t));
           updateNextMap(t,fNED);
           return Move{t, fNED};
       }
       else return Move{t, STILL};
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
        
        nextMap = presentMap;
        
        totalStrength = 0;
        
        for(unsigned short a = 0; a < presentMap.height; a++) {
            for(unsigned short b = 0; b < presentMap.width; b++) {
                hlt::Location t = { b, a };
                if (presentMap.getSite(t).owner == myID) {
                    moves.insert(assignMove(t));
                    totalStrength += presentMap.getSite(t).strength;
                }
            }
        }
        sendFrame(moves);
    }
    return 0;
}
