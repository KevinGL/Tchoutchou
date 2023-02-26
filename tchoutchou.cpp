#include "tchoutchou.h"
#include <iostream>

namespace tchoutchou
{
    void FollowRails::Init(std::vector<Line> &appLines,std::vector<Vehicle> &appVehicles,const unsigned int appTimeFrame)
    {
        lines=appLines;
        //vehicles=appVehicles;
        timeFrame=appTimeFrame;

        std::cout << "- Tchoutchou : Hello I am Tchoutchou, your best physic engine for trains, tramways, metros ..." << std::endl;

        if(appVehicles.size()!=0 && lines.size()!=0)
        {
            std::cout << "- Tchoutchou : I am positionning your vehicles on their lines, please wait ..." << std::endl;

            bool initVehicleOk=true;

            for(unsigned int i=0;i<appVehicles.size();i++)
            {
                if(!InitVehicleBogies(&appVehicles[i]))
                {
                    initVehicleOk=false;
                    break;
                }
            }

            if(initVehicleOk)
            {
                std::cout << "- Tchoutchou : Your vehicles has been positionned on their lines :)" << std::endl;

                /*for(unsigned int i=0;i<appVehicles[0].posBogies.size();i++)
                {
                    std::cout << appVehicles[0].posBogies[i].x << " " << appVehicles[0].posBogies[i].y << " " << appVehicles[0].posBogies[i].z << std::endl;
                }
                std::cout << "________" << std::endl;*/
            }
        }

        else
        {
            std::cout << "- Tchoutchou : You have not defined lines or vehicles, I can't go further, abort :(" << std::endl;
        }
    }

    bool FollowRails::InitVehicleBogies(Vehicle *vehicle)
    {
        const unsigned int indexLine=vehicle->indexLine;

        if(indexLine>lines.size()-1)
        {
            std::cout << "- Tchoutchou : You have an error with index of line, please check that, abort :(" << std::endl;
            return false;
        }

        Line line=lines[indexLine];

        const unsigned int indexPointFirstBogy=vehicle->indexPointFirstBogy;

        if(indexPointFirstBogy>line.points.size()-1)
        {
            std::cout << "- Tchoutchou : You have an error with index of point of line, please check that, abort :(" << std::endl;
            return false;
        }

        if(vehicle->posInitBogies.size()==0)
        {
            std::cout << "- Tchoutchou : You have no defined positions of bogies, abort :(" << std::endl;
            return false;
        }

        vehicle->posBogies.push_back(line.points[indexPointFirstBogy]);

        unsigned int indexPointBogyPrev=indexPointFirstBogy;
        unsigned int indexPointPrev=indexPointFirstBogy;

        for(unsigned int i=1;i<vehicle->posInitBogies.size();i++)
        {
            Demisphere demiSphere;

            demiSphere.center=vehicle->posBogies[i-1];
            demiSphere.radius=fabs(vehicle->posInitBogies[i]-vehicle->posInitBogies[i-1]);

            std::vector<glm::vec3> inters;

            if(vehicle->forth)
            {
                demiSphere.direction=line.points[indexPointPrev-1]-line.points[indexPointPrev];

                //std::cout << "Direction demi-sphere : " << demiSphere.direction.x << " " << demiSphere.direction.y << " " << demiSphere.direction.z << std::endl;

                for(unsigned int j=indexPointFirstBogy;j>=1;j--)
                {
                    Segment seg;

                    seg.v1=line.points[j];
                    seg.v2=line.points[j-1];

                    betweenDemisphereSegment(&demiSphere,&seg,inters);

                    //std::cout << "Bogy " << i << " point " << j << " : " << inters.size() << std::endl;

                    if(inters.size()!=0)
                    {
                        vehicle->posBogies.push_back(inters[0]);
                        indexPointPrev=j;

                        break;
                    }
                }
            }

            else
            {
                demiSphere.direction=line.points[indexPointPrev+1]-line.points[indexPointPrev];

                //std::cout << "Direction demi-sphere : " << demiSphere.direction.x << " " << demiSphere.direction.y << " " << demiSphere.direction.z << std::endl;

                for(unsigned int j=0;j<line.points.size()-1;j++)
                {
                    Segment seg;

                    seg.v1=line.points[j];
                    seg.v2=line.points[j+1];

                    betweenDemisphereSegment(&demiSphere,&seg,inters);

                    //std::cout << "Bogy " << i << " point " << j << " : " << inters.size() << std::endl;

                    if(inters.size()!=0)
                    {
                        vehicle->posBogies.push_back(inters[0]);
                        indexPointPrev=j;

                        break;
                    }
                }
            }
        }

        return true;
    }

    void betweenDemisphereSegment(Demisphere *demiSphere,Segment *seg,std::vector<glm::vec3> &intersections)
    {
        const float xV=seg->v2.x-seg->v1.x;
        const float yV=seg->v2.y-seg->v1.y;
        const float zV=seg->v2.z-seg->v1.z;

        const float x0=seg->v1.x;
        const float y0=seg->v1.y;
        const float z0=seg->v1.z;

        const float xC=demiSphere->center.x;
        const float yC=demiSphere->center.y;
        const float zC=demiSphere->center.z;
        const float R=demiSphere->radius;

        const float a=xV*xV + yV*yV + zV*zV;
        const float b=2*x0*xV - 2*xC*xV + 2*y0*yV - 2*yC*yV + 2*z0*zV - 2*zC*zV;
        const float c=x0*x0 + y0*y0 + z0*z0 - 2*xC*x0 - 2*yC*y0 - 2*zC*z0 + xC*xC + yC*yC + zC*zC - R*R;

        const float delta=b*b - 4*a*c;

        float t;

        glm::vec3 inter1,inter2;

        if(delta==0)
        {
            t=(-b)/(2*a);

            glm::vec3 miSeg;

            miSeg.x=(seg->v1.x+seg->v2.x)/2;
            miSeg.y=(seg->v1.y+seg->v2.y)/2;
            miSeg.z=(seg->v1.z+seg->v2.z)/2;

            inter1.x=x0+t*xV;
            inter1.y=y0+t*yV;
            inter1.z=z0+t*zV;

            const float segLength=glm::length(seg->v1-seg->v2);
            const float distSolMiSeg=glm::length(inter1-miSeg);

            if(distSolMiSeg<=segLength/2)
            {
                glm::vec3 vecCenterInter=inter1-demiSphere->center;

                if(glm::dot(demiSphere->direction,vecCenterInter)>=0)
                {
                    intersections.push_back(inter1);
                }
            }
        }

        else
        if(delta>0)
        {
            t=(-b-sqrt(delta))/(2*a);

            glm::vec3 miSeg;

            miSeg.x=(seg->v1.x+seg->v2.x)/2;
            miSeg.y=(seg->v1.y+seg->v2.y)/2;
            miSeg.z=(seg->v1.z+seg->v2.z)/2;

            float segLength=glm::length(seg->v1-seg->v2);

            inter1.x=x0+t*xV;
            inter1.y=y0+t*yV;
            inter1.z=z0+t*zV;

            float distSolMiSeg=glm::length(inter1-miSeg);

            if(distSolMiSeg<=segLength/2)
            {
                glm::vec3 vecCenterInter=inter1-demiSphere->center;

                if(glm::dot(demiSphere->direction,vecCenterInter)>=0)
                {
                    intersections.push_back(inter1);
                }
            }

            ////////////////////////////

            t=(-b+sqrt(delta))/(2*a);

            inter2.x=x0+t*xV;
            inter2.y=y0+t*yV;
            inter2.z=z0+t*zV;

            distSolMiSeg=glm::length(inter2-miSeg);

            if(distSolMiSeg<=segLength/2)
            {
                glm::vec3 vecCenterInter=inter2-demiSphere->center;

                if(glm::dot(demiSphere->direction,vecCenterInter)>=0)
                {
                    intersections.push_back(inter2);
                }
            }
        }
    }
}
