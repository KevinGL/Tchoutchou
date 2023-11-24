#include "tchoutchou.h"
#include <iostream>
#include <conio.h>
#include <math.h>

namespace tch
{
    void Network::commandVehicle(Vehicle *vehicle, const float weight, const float maxSpeed, const float maxStrength, const float traction, const int inverter, const bool reverse)
    {
        glm::vec2 vecTotalStrength = glm::vec2(0.0f);

        for(const Bogy bogy : vehicle->bogies)              //Bilan des forces pour chaque bogie
        {
            const glm::vec3 p1 = points[bogy.indexBefore].pos;
            const glm::vec3 p2 = points[bogy.indexAfter].pos;
            const float lgSeg = glm::length(p2 - p1);

            glm::vec2 vecSegProj;

            vecSegProj.y = p2.z - p1.z;
            vecSegProj.x = sqrt(pow(lgSeg, 2) - pow(vecSegProj.y, 2));

            const float weightByBogy = weight / vehicle->bogies.size();
            const glm::vec2 vecWeight = glm::vec2(0.0f, -weightByBogy * 9.81f);

            glm::vec4 vecReact = glm::vec4(0.0f, weightByBogy * 9.81f, 0.0f, 1.0f);

            const float alpha = atan(vecSegProj.y / vecSegProj.x) *180/PI;

            const glm::mat4 rotation = glm::rotate(alpha, 0.0f, 0.0f, 1.0f);

            vecReact = rotation * vecReact;

            const glm::vec2 vecStrength = vecWeight + glm::vec2(vecReact.x, vecReact.y);

            vecTotalStrength += vecStrength;
        }

        float accel = glm::length(vecTotalStrength) / weight;

        if(vecTotalStrength.x < 0)
        {
            accel *= -1;
        }

        if(inverter == 0)       //Frein de stationnement
        {
            accel = 0.0f;
        }

        const float speedKmh = (vehicle->speed / frameTime) * 3600;

        const float beta = log(0.01f) / maxSpeed;
        float engineStrength = traction * maxStrength * exp(beta * speedKmh);

        if(inverter == 0)               //Neutre
        {
            engineStrength = 0.0f;
        }
        else
        if(inverter == -1)              //Marche arrière
        {
            engineStrength *= -1;
        }

        if(reverse)                     //Sens retour
        {
            engineStrength *= -1;
        }

        accel += engineStrength / weight;

        //std::cout << "Inverter : " << inverter << " traction : " << traction << " strength (N) : " << engineStrength << " acceleration (m/s2) : " << accel << std::endl;
    }
}
