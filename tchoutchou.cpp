#include "tchoutchou.h"
#include <iostream>
#include <fstream>

namespace tch
{
    void Network::LoadNetwork(const std::string path)
    {
        std::ifstream file(path);

        while(1)
        {
            std::string line;

            if(!getline(file, line))
            {
                break;
            }

            if(line.find("v ") == 0)
            {
                Point p;

                sscanf(line.c_str(), "v %f %f %f\n", &p.pos.x, &p.pos.y, &p.pos.z);

                points.push_back(p);
            }

            else
            if(line.find("l ") == 0)
            {
                size_t index1, index2;

                sscanf(line.c_str(), "l %d %d\n", &index1, &index2);

                index1--;
                index2--;

                if(index1 < index2)
                {
                    points[index1].indexAfter.push_back(index2);
                    points[index2].indexBefore.push_back(index1);
                }

                else
                {
                    points[index2].indexAfter.push_back(index1);
                    points[index1].indexBefore.push_back(index2);
                }
            }
        }

        file.close();
    }

    void Network::addVehicle(Vehicle *vehicle, size_t indexPoint)
    {
        const glm::vec3 posNode = points[indexPoint].pos;

        vehicle->bogies[0].pos = posNode;
        vehicle->bogies[0].indexBefore = indexPoint;
        const size_t switchAfter = points[indexPoint].switchAfter;
        const size_t indexAfter = points[indexPoint].indexAfter[switchAfter];
        vehicle->bogies[0].indexAfter = indexAfter;

        //calculAngles(points[indexPoint], points[indexAfter], vehicle->bogies[0].pos, &vehicle->bogies[0].angleZ, &vehicle->bogies[0].angleY);
        calculAngles(vehicle->isTramway, points[indexPoint], points[indexAfter], vehicle->bogies, 0);

        posNextBogies(vehicle, indexPoint, indexAfter);

        /*for(const Bogy b : vehicle->bogies)
        {
            std::cout << "(" << b.pos.x << " " << b.pos.y << ") " << b.indexBefore << " " << b.indexAfter << std::endl;
        }*/
    }

    void Network::posNextBogies(Vehicle *vehicle, size_t indexPoint, size_t indexAfter)
    {
        for(size_t i=1; i<vehicle->bogies.size(); i++)
        {
            SemiSphere ssphere;

            ssphere.radius = fabs(vehicle->bogies[i].posInit - vehicle->bogies[i-1].posInit);
            ssphere.center = vehicle->bogies[i-1].pos;
            const size_t indexBefore = points[indexPoint].indexBefore[points[indexPoint].switchBefore];
            ssphere.dir = points[indexBefore].pos - points[indexPoint].pos;

            size_t indexAf = indexPoint;
            size_t switchBe = points[indexAf].switchBefore;

            //std::cout << "Bogie " << i << " :" << std::endl << std::endl;

            if(points[indexAf].indexBefore.size() != 0)
            {
                size_t indexBe = points[indexAf].indexBefore[switchBe];

                while(1)
                {
                    Segment seg;

                    seg.p1 = points[indexBe].pos;
                    seg.p2 = points[indexAf].pos;

                    //std::cout << indexBe << " (" << points[indexBe].pos.x << " " << points[indexBe].pos.y << ") - " << indexAf << " (" << points[indexAf].pos.x << " " << points[indexAf].pos.y << ")" << std::endl;

                    if(interSemiSphereSeg(ssphere, seg, vehicle->bogies[i].pos))
                    {
                        vehicle->bogies[i].indexBefore = indexBe;
                        vehicle->bogies[i].indexAfter = indexAf;

                        //calculAngles(points[indexBe], points[indexAf], vehicle->bogies[i].pos, &vehicle->bogies[i].angleZ, &vehicle->bogies[i].angleY);
                        calculAngles(vehicle->isTramway, points[indexBe], points[indexAf], vehicle->bogies, i);

                        indexPoint = indexAfter;
                        const size_t switchAfter = points[indexPoint].switchAfter;
                        indexAfter = points[indexPoint].indexAfter[switchAfter];

                        break;
                    }

                    indexAf = indexBe;
                    switchBe = points[indexAf].switchBefore;

                    if(points[indexAf].indexBefore.size() != 0)
                    {
                        indexBe = points[indexAf].indexBefore[switchBe];
                    }

                    else
                    {
                        break;
                    }
                }
            }

            else
            {
                std::cout << "Tchoutchou : Error : Out of way !" << std::endl;

                exit(-1);
            }
        }
    }
}
