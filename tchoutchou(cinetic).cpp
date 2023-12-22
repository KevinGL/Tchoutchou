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

        const float speedKmh = (fabs(vehicle->speed) / frameTime) * 3600;

        float tractionStrength;
        const float beta = log(0.01f) / maxSpeed;

        if(traction >= 0.0f)
        {
            tractionStrength = traction * maxStrength * exp(beta * speedKmh);
        }
        else
        {
            tractionStrength = -10.0f * maxStrength;
        }

        if(inverter == 0)               //Neutre
        {
            tractionStrength = 0.0f;
        }

        const bool wayDirection = isWayDirection(reverse, inverter);        //Dans le sens de la voie ou non selon itinéraire et inverseur

        if(!wayDirection)
        {
            tractionStrength *= -1;
        }

        accel += tractionStrength / weight;

        float acceleration = accel;

        acceleration /= 1000;           //m/s/ms
        acceleration *= frameTime;      //m/s/frame
        acceleration /= 1000;           //m/ms2
        acceleration *= frameTime;      //m/frame2

        vehicle->speed += acceleration;

        if(traction < 0.0f)             //Freinage
        {
            if(wayDirection && vehicle->speed < 0.0f)
            {
                vehicle->speed = 0.0f;
            }
            else
            if(!wayDirection && vehicle->speed > 0.0f)
            {
                vehicle->speed = 0.0f;
            }
        }

        //std::cout << (vehicle->speed / frameTime) * 3600 << " km/h" << std::endl;

        //std::cout << "Inverter : " << inverter << " traction : " << traction << " strength (N) : " << tractionStrength << " acceleration (m/s2) : " << accel << std::endl;
        //std::cout << "Inverter : " << inverter << " traction : " << traction << " sensway : " << wayDirection << " strength (N) : " << tractionStrength << " accel (m/s2) : " << accel << " speed (km/h) : " << (vehicle->speed / frameTime) * 3600 << std::endl;

        movingVehicle(vehicle);
    }

    bool isWayDirection(const bool reverse, const int inverter)
    {
        bool res;

        if(!reverse)
        {
            if(inverter == 1)
            {
                res = true;
            }

            else
            if(inverter == -1)
            {
                res = false;
            }
        }

        else
        {
            if(inverter == 1)
            {
                res = false;
            }

            else
            if(inverter == -1)
            {
                res = true;
            }
        }

        return res;
    }
}
