#include "tchoutchou.h"
#include <iostream>
#include <conio.h>

namespace tch
{
    void Network::movingVehicle(Vehicle *vehicle)
    {
        const float speed = fabs(vehicle->speed);

        if(vehicle->speed > 0)
        {
            const Point beforePoint = points[vehicle->bogies[0].indexBefore];
            size_t switchAfter = beforePoint.switchAfter;
            const size_t indexAfter = beforePoint.indexAfter[switchAfter];
            const Point afterPoint = points[indexAfter];

            const float distUntilNextPoint = glm::length(afterPoint.pos - vehicle->bogies[0].pos);

            if(speed < distUntilNextPoint)
            {
                vehicle->bogies[0].pos += speed * glm::normalize(afterPoint.pos - beforePoint.pos);

                calculAngles(beforePoint.pos, afterPoint.pos, &vehicle->bogies[0].angleZ, &vehicle->bogies[0].angleY);
            }

            else
            {
                float distUntilPointAfter = distUntilNextPoint;

                size_t indexBe = vehicle->bogies[0].indexAfter;
                size_t switchAfter = points[indexBe].switchAfter;
                size_t indexAf = points[indexBe].indexAfter[switchAfter];

                while(1)
                {
                    const glm::vec3 pointBefore = points[indexBe].pos;
                    const glm::vec3 pointAfter = points[indexAf].pos;

                    distUntilPointAfter += glm::length(pointAfter - pointBefore);

                    if(distUntilPointAfter >= speed)
                    {
                        break;
                    }

                    indexBe = indexAf;
                    const size_t switchAfter = points[indexBe].switchAfter;
                    indexAf = points[indexBe].indexAfter[switchAfter];
                }

                const float tooMuch = distUntilPointAfter - speed;
                const glm::vec3 vecSeg = glm::normalize(points[indexAf].pos - points[indexBe].pos);

                vehicle->bogies[0].pos = points[indexAf].pos - tooMuch * vecSeg;

                vehicle->bogies[0].indexBefore = indexBe;
                vehicle->bogies[0].indexAfter = indexAf;

                const Point beforePoint = points[vehicle->bogies[0].indexBefore];
                switchAfter = beforePoint.switchAfter;
                const size_t indexAfter = beforePoint.indexAfter[switchAfter];
                const Point afterPoint = points[indexAfter];

                calculAngles(beforePoint.pos, afterPoint.pos, &vehicle->bogies[0].angleZ, &vehicle->bogies[0].angleY);
            }

            const size_t beforeForNext = vehicle->bogies[0].indexAfter;
            switchAfter = points[beforeForNext].switchAfter;
            const size_t afterForNext = points[beforeForNext].indexAfter[switchAfter];

            posNextBogies(vehicle, beforeForNext, afterForNext, false);
        }
    }
}
