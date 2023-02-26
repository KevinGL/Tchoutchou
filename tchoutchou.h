#ifndef TCHOUTCHOU_H
#define TCHOUTCHOU_H
#endif // TCHOUTCHOU_H

#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

namespace tchoutchou
{
    struct Line
    {
        std::vector<glm::vec3> points;
    };

    struct Bogy
    {
        glm::vec3 pos;
        unsigned int before;
        unsigned int after;
    };

    struct Vehicle
    {
        std::vector<float> posInitBogies;
        std::vector<Bogy> bogies;
        std::vector<float> carsMass;        //kg
        float maxPower;                     //W
        float throttle=0.0f;                //[-1.0 , 1.0]
        float speed=0.2f;                   //m/frame
        unsigned int indexLine;
        unsigned int indexPointFirstBogy;
        bool forth;
    };

    struct Demisphere
    {
        glm::vec3 center;
        float radius;
        glm::vec3 direction;
    };

    struct Segment
    {
        glm::vec3 v1;
        glm::vec3 v2;
    };

    class FollowRails
    {
        private :

        std::vector<Line> lines;
        //std::vector<Vehicle> vehicles;
        unsigned int timeFrame;

        bool InitVehicleBogies(Vehicle *vehicle);
        bool detectOverflowSeg(const glm::vec3 pointBefore,const glm::vec3 pointAfter,const glm::vec3 posBogy,float *overflow);
        void moveHeadBogy(Vehicle *vehicle,const bool forth);
        void moveOthersBogies(Vehicle *vehicle,const bool forth);

        public :

        void Init(std::vector<Line> &appLines,std::vector<Vehicle> &appVehicles,const unsigned int appTimeFrame);
        void moveVehicle(Vehicle *vehicle);
    };

    void betweenDemisphereSegment(Demisphere *demiSphere,Segment *seg,std::vector<glm::vec3> &intersections);
}
