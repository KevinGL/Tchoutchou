#include "tchoutchou.h"
#include <iostream>

namespace tchoutchou
{
    void FollowRails::moveHeadBogy(Vehicle *vehicle,const bool forth)
    {
        if(forth)
        {
            const unsigned int before=vehicle->bogies[0].before;
            const unsigned int after=vehicle->bogies[0].after;

            const glm::vec3 pointBefore=lines[vehicle->indexLine].points[before];
            const glm::vec3 pointAfter=lines[vehicle->indexLine].points[after];

            const float distUntilNexPoint=glm::length(pointAfter-vehicle->bogies[0].pos);

            //std::cout << vehicle->speed << " " << distUntilNexPoint << std::endl;

            if(vehicle->speed<distUntilNexPoint)
            {
                const glm::vec3 vecMove=vehicle->speed*glm::normalize(pointAfter-pointBefore);

                vehicle->bogies[0].pos+=vecMove;

                float overflow;

                if(detectOverflowSeg(pointBefore,pointAfter,vehicle->bogies[0].pos,&overflow))
                {
                    vehicle->bogies[0].before++;
                    vehicle->bogies[0].after++;

                    const unsigned int newBefore=vehicle->bogies[0].before;
                    const unsigned int newAfter=vehicle->bogies[0].after;

                    const glm::vec3 newPointBefore=lines[vehicle->indexLine].points[newBefore];
                    const glm::vec3 newPointAfter=lines[vehicle->indexLine].points[newAfter];

                    const glm::vec3 newVecMove=overflow*glm::normalize(newPointAfter-newPointBefore);

                    vehicle->bogies[0].pos=newPointBefore+newVecMove;
                }
            }

            else
            {
                float distTest=distUntilNexPoint;

                for(unsigned int i=after;i<lines[vehicle->indexLine].points.size()-1;i++)
                {
                    const unsigned int newBefore=i;
                    const unsigned int newAfter=i+1;

                    const glm::vec3 newPointBefore=lines[vehicle->indexLine].points[newBefore];
                    const glm::vec3 newPointAfter=lines[vehicle->indexLine].points[newAfter];

                    const float segLength=glm::length(newPointAfter-newPointBefore);

                    distTest+=segLength;

                    if(distTest>vehicle->speed)
                    {
                        const float distFromBefore=segLength-(distTest-vehicle->speed);

                        const glm::vec3 newVecMove=distFromBefore*glm::normalize(newPointAfter-newPointBefore);

                        vehicle->bogies[0].pos=newPointBefore+newVecMove;

                        vehicle->bogies[0].before=newBefore;
                        vehicle->bogies[0].after=newAfter;

                        break;
                    }
                }
            }
        }

        else
        {
            //
        }
    }

    void FollowRails::moveOthersBogies(Vehicle *vehicle,const bool forth)
    {
        if(forth)
        {
            for(unsigned int i=1;i<vehicle->bogies.size();i++)
            {
                const glm::vec3 posBogyPrev=vehicle->bogies[i-1].pos;
                const float distFromBogyPrev=fabs(vehicle->posInitBogies[i]-vehicle->posInitBogies[i-1]);

                const unsigned int beginLoop=vehicle->bogies[i-1].after;

                for(unsigned int j=beginLoop;j>=1;j--)
                {
                    Demisphere demiSphere;

                    demiSphere.center=posBogyPrev;
                    demiSphere.radius=distFromBogyPrev;

                    glm::vec3 pointBefore=lines[vehicle->indexLine].points[j-1];
                    glm::vec3 pointAfter=lines[vehicle->indexLine].points[j];

                    demiSphere.direction=pointBefore-pointAfter;

                    Segment seg;

                    seg.v1=pointBefore;
                    seg.v2=pointAfter;

                    std::vector<glm::vec3> inters;

                    betweenDemisphereSegment(&demiSphere,&seg,inters);

                    if(inters.size()!=0)
                    {
                        vehicle->bogies[i].pos=inters[0];

                        vehicle->bogies[i].before=j-1;
                        vehicle->bogies[i].after=j;

                        break;
                    }
                }
            }
        }

        else
        {
            //
        }
    }

    void FollowRails::moveVehicle(Vehicle *vehicle)
    {
        if(vehicle->forth)
        {
            moveHeadBogy(vehicle,true);
            moveOthersBogies(vehicle,true);
        }
        else
        {
            moveHeadBogy(vehicle,false);
            moveOthersBogies(vehicle,false);
        }
    }

    bool FollowRails::detectOverflowSeg(const glm::vec3 pointBefore,const glm::vec3 pointAfter,const glm::vec3 posBogy,float *overflow)
    {
        glm::vec3 miSeg;

        miSeg.x=(pointBefore.x+pointAfter.x)/2;
        miSeg.y=(pointBefore.y+pointAfter.y)/2;
        miSeg.z=(pointBefore.z+pointAfter.z)/2;

        const float segLength=glm::length(pointAfter-pointBefore);
        const float distFromMi=glm::length(posBogy-miSeg);

        if(distFromMi>segLength/2)
        {
            *overflow=distFromMi-(segLength/2);
            return true;
        }

        return false;
    }
}
