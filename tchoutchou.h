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

    class World
    {
        private :

        std::vector<Point> points;
        std::vector<Vehicle> vehicles;

        public :

        void addPoint(Point p)
        {
            points.push_back(p);
        }

        void InitPoints(std::vector<Point> array)
        {
            points = array;
        }

        void addVehicle(std::vector<float> posBogies, const size_t indexPoint);
    };
}
