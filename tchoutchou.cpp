#include "tchoutchou.h"
#include <iostream>

namespace tchoutchou
{
    void FollowRails::Init(std::vector<Way> &appWays, std::vector<Vehicle> &appVehicles, const unsigned int appTimeFrame)
    {
        ways=  appWays;
        //vehicles=appVehicles;
        timeFrame = appTimeFrame;

        std::cout << std::endl << "- Tchoutchou : Hello I am Tchoutchou, your best physical engine for trains, tramways, metros ..." << std::endl;

        if(appVehicles.size() != 0 && ways.size() != 0)
        {
            std::cout << "- Tchoutchou : I am positionning your vehicles on their ways, please wait ..." << std::endl;

            bool initVehicleOk = true;

            for(unsigned int i = 0; i < appVehicles.size(); i++)
            {
                if(!InitVehicleBogies(&appVehicles[i]))
                {
                    initVehicleOk = false;
                    break;
                }
            }

            if(initVehicleOk)
            {
                std::cout << "- Tchoutchou : Your vehicles have been positionned on their ways :)" << std::endl;

                /*for(unsigned int i=0;i<appVehicles[0].bogies.size();i++)
                {
                    //std::cout << appVehicles[0].bogies[i].pos.x << " " << appVehicles[0].bogies[i].pos.y << " " << appVehicles[0].bogies[i].pos.z << std::endl;
                    std::cout << appVehicles[0].bogies[i].before << " (" << ways[appVehicles[0].indexLine].points[appVehicles[0].bogies[i].before].x << " " << ways[appVehicles[0].indexLine].points[appVehicles[0].bogies[i].before].y << ") " << appVehicles[0].bogies[i].after << " (" << ways[appVehicles[0].indexLine].points[appVehicles[0].bogies[i].after].x << " " << ways[appVehicles[0].indexLine].points[appVehicles[0].bogies[i].after].y << ")" << std::endl;
                }
                std::cout << "________" << std::endl;*/
            }
        }

        else
        {
            std::cout << "- Tchoutchou : You have not defined ways or vehicles, I can't go further, abort :(" << std::endl;
        }
    }

    bool FollowRails::InitVehicleBogies(Vehicle *vehicle)
    {
        const unsigned int indexWay = vehicle->indexWay;

        if(indexWay > ways.size()-1)
        {
            std::cout << "- Tchoutchou : You have an error with index of line, please check that, abort :(" << std::endl;
            return false;
        }

        Way way = ways[indexWay];

        const unsigned int indexPointFirstBogy = vehicle->indexPointFirstBogy;

        if(indexPointFirstBogy > way.points.size()-1)
        {
            std::cout << "- Tchoutchou : You have an error with index of point of way, please check that, abort :(" << std::endl;
            return false;
        }

        if(vehicle->posInitBogies.size() == 0)
        {
            std::cout << "- Tchoutchou : You have no defined positions of bogies, abort :(" << std::endl;
            return false;
        }

        Bogy bogy;

        bogy.pos.x = (way.points[indexPointFirstBogy].x + way.points[indexPointFirstBogy+1].x) / 2;
        bogy.pos.y = (way.points[indexPointFirstBogy].y + way.points[indexPointFirstBogy+1].y) / 2;
        bogy.pos.z = (way.points[indexPointFirstBogy].z + way.points[indexPointFirstBogy+1].z) / 2;

        bogy.before = indexPointFirstBogy;
        bogy.after = indexPointFirstBogy+1;

        vehicle->bogies.push_back(bogy);

        //unsigned int indexPointBogyPrev = indexPointFirstBogy;
        unsigned int indexPointPrev = indexPointFirstBogy;

        for(unsigned int i=1; i < vehicle->posInitBogies.size(); i++)
        {
            Demisphere demiSphere;

            demiSphere.center = vehicle->bogies[i-1].pos;
            demiSphere.radius = fabs(vehicle->posInitBogies[i] - vehicle->posInitBogies[i-1]);

            std::vector<glm::vec3> inters;

            demiSphere.direction = way.points[indexPointPrev-1] - way.points[indexPointPrev];

            //std::cout << "Direction demi-sphere : " << demiSphere.direction.x << " " << demiSphere.direction.y << " " << demiSphere.direction.z << std::endl;

            for(unsigned int j = way.points.size()-1; j >= 1; j--)
            {
                Segment seg;

                seg.v1 = way.points[j-1];
                seg.v2 = way.points[j];

                betweenDemisphereSegment(&demiSphere, &seg, inters);

                //std::cout << "Bogy " << i << " point " << j << " : " << inters.size() << std::endl;

                if(inters.size() != 0)
                {
                    bogy.pos = inters[0];

                    bogy.before = j-1;
                    bogy.after = j;

                    vehicle->bogies.push_back(bogy);
                    indexPointPrev = j-1;

                    break;
                }
            }

        }

        return true;
    }

    void betweenDemisphereSegment(Demisphere *demiSphere, Segment *seg, std::vector<glm::vec3> &intersections)
    {
        const double xV = seg->v2.x - seg->v1.x;
        const double yV = seg->v2.y - seg->v1.y;
        const double zV = seg->v2.z - seg->v1.z;

        const double x0 = seg->v1.x;
        const double y0 = seg->v1.y;
        const double z0 = seg->v1.z;

        const double xC = demiSphere->center.x;
        const double yC = demiSphere->center.y;
        const double zC = demiSphere->center.z;
        const double R = demiSphere->radius;

        const double a = xV * xV + yV * yV + zV * zV;
        const double b = 2 * x0 * xV - 2 * xC * xV + 2 * y0 * yV - 2 * yC * yV + 2 * z0 * zV - 2 * zC* zV;
        const double c = x0 * x0 + y0 * y0 + z0 * z0 - 2 * xC * x0 - 2 * yC * y0 - 2 * zC * z0 + xC * xC + yC * yC + zC * zC - R * R;

        const double delta = b * b - 4 * a * c;

        double t;

        glm::vec3 inter1, inter2;

        if(delta == 0)
        {
            t = (-b) / (2 * a);

            glm::vec3 miSeg;

            miSeg.x = (seg->v1.x + seg->v2.x) / 2;
            miSeg.y = (seg->v1.y + seg->v2.y) / 2;
            miSeg.z = (seg->v1.z + seg->v2.z) / 2;

            inter1.x = x0 + t * xV;
            inter1.y = y0 + t * yV;
            inter1.z = z0 + t * zV;

            const double segLength = glm::length(seg->v1 - seg->v2);
            const double distSolMiSeg = glm::length(inter1 - miSeg);

            if(distSolMiSeg <= segLength/2)
            {
                glm::vec3 vecCenterInter = inter1 - demiSphere->center;

                if(glm::dot(demiSphere->direction, vecCenterInter) >= 0)
                {
                    intersections.push_back(inter1);
                }
            }
        }

        else
        if(delta > 0)
        {
            t = (-b - sqrt(delta)) /( 2 * a);

            glm::vec3 miSeg;

            miSeg.x = (seg->v1.x + seg->v2.x) / 2;
            miSeg.y = (seg->v1.y + seg->v2.y) / 2;
            miSeg.z = (seg->v1.z + seg->v2.z) / 2;

            double segLength = glm::length(seg->v1 - seg->v2);

            inter1.x = x0 + t * xV;
            inter1.y = y0 + t * yV;
            inter1.z = z0 + t * zV;

            double distSolMiSeg = glm::length(inter1 - miSeg);

            if(distSolMiSeg <= segLength/2)
            {
                glm::vec3 vecCenterInter = inter1 - demiSphere->center;

                if(glm::dot(demiSphere->direction, vecCenterInter) >= 0)
                {
                    intersections.push_back(inter1);
                }
            }

            ////////////////////////////

            t = (-b + sqrt(delta)) / (2 * a);

            inter2.x = x0 + t * xV;
            inter2.y = y0 + t * yV;
            inter2.z = z0 + t * zV;

            distSolMiSeg = glm::length(inter2 - miSeg);

            if(distSolMiSeg <= segLength / 2)
            {
                glm::vec3 vecCenterInter = inter2 - demiSphere->center;

                if(glm::dot(demiSphere->direction, vecCenterInter) >= 0)
                {
                    intersections.push_back(inter2);
                }
            }
        }
    }
}
