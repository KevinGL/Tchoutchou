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

    struct Vehicle
    {
        std::vector<float> posInitBogies;
        std::vector<glm::vec3> posBogies;
        std::vector<float> carsMass;        //kg
        float maxPower;                     //W
        float throttle=0.0f;                //[-1.0 , 1.0]
        float speed=0.0f;                   //m/frame
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

        public :

        void Init(std::vector<Line> &appLines,std::vector<Vehicle> &appVehicles,const unsigned int appTimeFrame);
    };

    void betweenDemisphereSegment(Demisphere *demiSphere,Segment *seg,std::vector<glm::vec3> &intersections);
}
