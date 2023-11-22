#include "tchoutchou.h"
#include <iostream>

namespace tch
{
    void World::addVehicle(std::vector<float> posBogies, const size_t indexPoint)
    {
        Vehicle v;

        for(float p : posBogies)
        {
            Bogy b;

            b.posInit = p;

            v.bogies.push_back(b);
        }

        vehicles.push_back(v);
    }
}
