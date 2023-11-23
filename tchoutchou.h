#ifndef TCHOUTCHOU_H
#define TCHOUTCHOU_H
#endif // TCHOUTCHOU_H

#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#define MIN_SPEED 0.005f

namespace tch
{
    struct Bogy
    {
        float posInit;
        glm::vec3 pos;
        float angle;
    };

    struct Vehicle
    {
        std::vector<Bogy> bogies;
    };

    struct Point
    {
        glm::vec3 pos;
        int indexAfter = -1;
        int indexBefore = -1;
    };

    struct SemiSphere
    {
        glm::vec3 center;
        float radius;
        glm::vec3 dir;
    };

    struct Segment
    {
        glm::vec3 p1;
        glm::vec3 p2;
    };

    class Network
    {
        private :

        std::vector<Point> points;
        //std::vector<Vehicle> vehicles;

        public :

        void addPoint(Point p)
        {
            points.push_back(p);
        }

        void InitPoints(std::vector<Point> array)
        {
            points = array;
        }

        void addVehicle(Vehicle *vehicle, const size_t indexPoint);
        void commandVehicle(Vehicle *vehicle, const float weight, const float maxSpeed, const float traction);

        /*Vehicle* getVehicle(const size_t index)
        {
            return (index < vehicles.size() ? &vehicles[index] : nullptr);
        }*/
    };

    bool interSemiSphereSeg(SemiSphere sph, Segment seg, glm::vec3 &inter);
}
