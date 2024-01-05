#ifndef TCHOUTCHOU_H
#define TCHOUTCHOU_H
#endif // TCHOUTCHOU_H

#include <vector>
#include <string>
#include <map>
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#define MIN_SPEED 0.005f
#define PI 3.1415926535897932384626433832795

namespace tch
{
    struct Bogy
    {
        float posInit;
        glm::vec3 pos;
        float angleZ;
        float angleY;
        int indexBefore;
        int indexAfter;
    };

    struct Vehicle
    {
        std::vector<Bogy> bogies;
        float speed = 0.0f;
        int declivity;
        bool isTramway;
    };

    struct Point
    {
        glm::vec3 pos;
        std::vector<size_t> indexAfter;
        std::vector<size_t> indexBefore;
        size_t switchAfter = 0;
        size_t switchBefore = 0;
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
        unsigned int frameTime;
        //std::vector<Vehicle> vehicles;

        void movingVehicle(Vehicle *vehicle);
        void posNextBogies(Vehicle *vehicle, size_t indexPoint, size_t indexAfter);
        void recalculateBeAf(const glm::vec3 bogyPos, int *indexBe, int *indexAf);
        //void calculAngles(const Point p1, const Point p2, const glm::vec3 posBogy, float *angleZ, float *angleY);
        void calculAngles(const bool isTramway, const Point p1, const Point p2, std::vector<Bogy> &bogies, size_t indexBogy);

        public :

        void addPoint(Point p)
        {
            points.push_back(p);
        }

        void InitPoints(std::vector<Point> array)
        {
            points = array;
        }

        void setSwitchAfter(const size_t indexPoint, const size_t value)
        {
            if(indexPoint <= points.size())
            {
                if(value <= points[indexPoint].indexAfter.size()-1)
                {
                    points[indexPoint].switchAfter = value;
                }
            }
        }

        void setSwitchBefore(const size_t indexPoint, const size_t value)
        {
            if(indexPoint <= points.size())
            {
                if(value <= points[indexPoint].indexBefore.size()-1)
                {
                    points[indexPoint].switchBefore = value;
                }
            }
        }

        void LoadNetwork(const std::string path);
        void addVehicle(Vehicle *vehicle, size_t indexPoint);
        void commandVehicle(Vehicle *vehicle, const float weight, const float maxSpeed, const float maxStrength, const float traction, const int inverter, const bool reverse);
        Network(const unsigned int fTime)
        {
            frameTime = fTime;
        };
    };

    bool interSemiSphereSeg(SemiSphere sph, Segment seg, glm::vec3 &inter);
    bool isWayDirection(const bool reverse, const int inverter);
    bool crossed(const glm::vec3 bogyPos, const glm::vec3 before, const glm::vec3 after);
}
