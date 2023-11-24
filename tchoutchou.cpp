#include "tchoutchou.h"
#include <iostream>

namespace tch
{
    void Network::addVehicle(Vehicle *vehicle, const size_t indexPoint)
    {
        vehicle->bogies[0].pos = points[indexPoint].pos;
        vehicle->bogies[0].indexBefore = indexPoint;
        vehicle->bogies[0].indexAfter = points[indexPoint].indexAfter;

        size_t indexPointBogyPrev = indexPoint;

        for(size_t i=1; i<vehicle->bogies.size(); i++)
        {
            SemiSphere s;

            s.center = vehicle->bogies[i-1].pos;
            s.radius = fabs(vehicle->bogies[i].posInit - vehicle->bogies[i-1].posInit);

            const size_t before = points[indexPointBogyPrev].indexBefore;
            s.dir = glm::normalize(points[before].pos - points[indexPointBogyPrev].pos);

            glm::vec3 inter;

            for(size_t j=0; j<points.size()-1; j++)
            {
                Segment seg;

                seg.p1 = points[j].pos;
                seg.p2 = points[points[j].indexAfter].pos;

                if(interSemiSphereSeg(s, seg, inter))
                {
                    indexPointBogyPrev = j;
                    vehicle->bogies[i].pos = inter;
                    vehicle->bogies[i].indexBefore = j;
                    vehicle->bogies[i].indexAfter = points[j].indexAfter;
                    break;
                }
            }
        }
    }
}
