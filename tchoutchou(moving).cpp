#include "tchoutchou.h"
#include <iostream>
#include <conio.h>

namespace tchoutchou
{
    void FollowRails::moveHeadBogy(Vehicle *vehicle,bool *forth)
    {
        vehicleCinetic(vehicle,forth);

        if(*forth)
        {
            const unsigned int before = vehicle->bogies[0].before;
            const unsigned int after = vehicle->bogies[0].after;

            const glm::vec3 pointBefore = ways[vehicle->indexWay].points[before];
            const glm::vec3 pointAfter = ways[vehicle->indexWay].points[after];

            const float distUntilNextPoint = glm::length(pointAfter - vehicle->bogies[0].pos);

            if(vehicle->speed < distUntilNextPoint)
            {
                const glm::vec3 vecMove = vehicle->speed * glm::normalize(pointAfter - pointBefore);

                vehicle->bogies[0].pos += vecMove;

                //std::cout << "Petite vitesse sens aller" << std::endl;

                float overflow;

                if(detectOverflowSeg(pointBefore, pointAfter, vehicle->bogies[0].pos, &overflow))
                {
                    vehicle->bogies[0].before++;
                    vehicle->bogies[0].after++;

                    const unsigned int newBefore = vehicle->bogies[0].before;
                    const unsigned int newAfter = vehicle->bogies[0].after;

                    const glm::vec3 newPointBefore = ways[vehicle->indexWay].points[newBefore];
                    const glm::vec3 newPointAfter = ways[vehicle->indexWay].points[newAfter];

                    const glm::vec3 newVecMove = overflow * glm::normalize(newPointAfter - newPointBefore);

                    vehicle->bogies[0].pos = newPointBefore + newVecMove;
                }
            }

            else
            {
                float distTest = distUntilNextPoint;

                //std::cout << "Grande vitesse sens aller" << std::endl;

                for(unsigned int i = after; i< ways[vehicle->indexWay].points.size()-1; i++)
                {
                    const unsigned int newBefore = i;
                    const unsigned int newAfter = i+1;

                    const glm::vec3 newPointBefore = ways[vehicle->indexWay].points[newBefore];
                    const glm::vec3 newPointAfter = ways[vehicle->indexWay].points[newAfter];

                    const float segLength = glm::length(newPointAfter - newPointBefore);

                    distTest += segLength;

                    if(distTest > vehicle->speed)
                    {
                        const float distFromBefore = segLength - (distTest - vehicle->speed);

                        const glm::vec3 newVecMove = distFromBefore * glm::normalize(newPointAfter - newPointBefore);

                        vehicle->bogies[0].pos = newPointBefore + newVecMove;

                        vehicle->bogies[0].before = newBefore;
                        vehicle->bogies[0].after = newAfter;

                        break;
                    }
                }
            }
        }

        else
        {
            const unsigned int lastBogy = vehicle->bogies.size() - 1;
            const unsigned int before = vehicle->bogies[lastBogy].before;
            const unsigned int after = vehicle->bogies[lastBogy].after;

            const glm::vec3 pointBefore = ways[vehicle->indexWay].points[before];
            const glm::vec3 pointAfter = ways[vehicle->indexWay].points[after];

            const float distUntilNextPoint = glm::length(pointBefore - vehicle->bogies[lastBogy].pos);

            if(vehicle->speed < distUntilNextPoint)
            {
                const glm::vec3 vecMove = vehicle->speed * glm::normalize(pointBefore - pointAfter);

                //std::cout << vehicle->bogies[lastBogy].pos.x << " " << vehicle->bogies[lastBogy].pos.y << std::endl;
                vehicle->bogies[lastBogy].pos += vecMove;
                /*std::cout << vehicle->bogies[lastBogy].pos.x << " " << vehicle->bogies[lastBogy].pos.y << std::endl;

                std::cout << "Petite vitesse sens retour" << std::endl;*/

                float overflow;

                if(detectOverflowSeg(pointBefore, pointAfter, vehicle->bogies[lastBogy].pos, &overflow))
                {
                    vehicle->bogies[lastBogy].before--;
                    vehicle->bogies[lastBogy].after--;

                    const unsigned int newBefore = vehicle->bogies[lastBogy].before;
                    const unsigned int newAfter = vehicle->bogies[lastBogy].after;

                    const glm::vec3 newPointBefore = ways[vehicle->indexWay].points[newBefore];
                    const glm::vec3 newPointAfter = ways[vehicle->indexWay].points[newAfter];

                    const glm::vec3 newVecMove = overflow * glm::normalize(newPointBefore - newPointAfter);

                    vehicle->bogies[lastBogy].pos = newPointAfter + newVecMove;
                }
            }

            else
            {
                float distTest = distUntilNextPoint;

                //std::cout << "Grande vitesse sens retour" << std::endl;

                for(unsigned int i = before; i>=1; i--)
                {
                    const unsigned int newBefore = i - 1;
                    const unsigned int newAfter = i;

                    const glm::vec3 newPointBefore = ways[vehicle->indexWay].points[newBefore];
                    const glm::vec3 newPointAfter = ways[vehicle->indexWay].points[newAfter];

                    const float segLength = glm::length(newPointAfter - newPointBefore);

                    distTest += segLength;

                    if(distTest > vehicle->speed)
                    {
                        const float distFromBefore = segLength - (distTest-vehicle->speed);

                        const glm::vec3 newVecMove = distFromBefore * glm::normalize(newPointBefore - newPointAfter);

                        vehicle->bogies[lastBogy].pos = newPointAfter + newVecMove;

                        vehicle->bogies[lastBogy].before = newBefore;
                        vehicle->bogies[lastBogy].after = newAfter;

                        break;
                    }
                }
            }
        }
    }

    void FollowRails::moveOthersBogies(Vehicle *vehicle, const bool forth)
    {
        if(forth)
        {
            for(unsigned int i=1; i < vehicle->bogies.size(); i++)
            {
                const glm::vec3 posBogyPrev = vehicle->bogies[i-1].pos;
                const float distFromBogyPrev = fabs(vehicle->posInitBogies[i] - vehicle->posInitBogies[i-1]);

                const unsigned int beginLoop = vehicle->bogies[i-1].after;

                for(unsigned int j = beginLoop; j>=1; j--)
                {
                    Demisphere demiSphere;

                    demiSphere.center = posBogyPrev;
                    demiSphere.radius = distFromBogyPrev;

                    glm::vec3 pointBefore = ways[vehicle->indexWay].points[j-1];
                    glm::vec3 pointAfter = ways[vehicle->indexWay].points[j];

                    demiSphere.direction = pointBefore - pointAfter;

                    Segment seg;

                    seg.v1 = pointBefore;
                    seg.v2 = pointAfter;

                    std::vector<glm::vec3> inters;

                    betweenDemisphereSegment(&demiSphere ,&seg ,inters);

                    if(inters.size() != 0)
                    {
                        vehicle->bogies[i].pos = inters[0];

                        vehicle->bogies[i].before = j-1;
                        vehicle->bogies[i].after = j;

                        break;
                    }
                }
            }
        }

        else
        {
            const unsigned int lastBogy = vehicle->bogies.size()-1;

            for(int i = lastBogy-1; i>=0; i--)
            {
                const glm::vec3 posBogyNext = vehicle->bogies[i+1].pos;
                const float distFromBogyNext = fabs(vehicle->posInitBogies[i] - vehicle->posInitBogies[i+1]);

                for(unsigned int j = vehicle->bogies[i+1].before; j < ways[vehicle->indexWay].points.size()-1; j++)
                {
                    Demisphere demiSphere;

                    demiSphere.center = posBogyNext;
                    demiSphere.radius = distFromBogyNext;

                    glm::vec3 pointBefore = ways[vehicle->indexWay].points[j];
                    glm::vec3 pointAfter = ways[vehicle->indexWay].points[j+1];

                    demiSphere.direction = pointAfter - pointBefore;

                    Segment seg;

                    seg.v1 = pointBefore;
                    seg.v2 = pointAfter;

                    std::vector<glm::vec3> inters;

                    betweenDemisphereSegment(&demiSphere, &seg, inters);

                    //std::cout << i << " " << inters.size() << std::endl;

                    if(inters.size() != 0)
                    {
                        vehicle->bogies[i].pos = inters[0];

                        vehicle->bogies[i].before = j-1;
                        vehicle->bogies[i].after = j;

                        break;
                    }
                }
            }
        }
    }

    void FollowRails::moveVehicle(Vehicle *vehicle)
    {
        bool forth;

        moveHeadBogy(vehicle, &forth);
        moveOthersBogies(vehicle, forth);
    }

    bool FollowRails::detectOverflowSeg(const glm::vec3 pointBefore, const glm::vec3 pointAfter, const glm::vec3 posBogy, float *overflow)
    {
        glm::vec3 miSeg;

        miSeg.x = (pointBefore.x + pointAfter.x)/2;
        miSeg.y = (pointBefore.y + pointAfter.y)/2;
        miSeg.z = (pointBefore.z + pointAfter.z)/2;

        const float segLength = glm::length(pointAfter - pointBefore);
        const float distFromMi = glm::length(posBogy - miSeg);

        if(distFromMi > segLength/2)
        {
            *overflow = distFromMi - (segLength/2);
            return true;
        }

        return false;
    }
}
