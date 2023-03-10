#include "tchoutchou.h"
#include <iostream>
#include <conio.h>
#include <math.h>

namespace tchoutchou
{
    void FollowRails::vehicleCinetic(Vehicle *vehicle,bool *forth)
    {
        const float maxStrength = (vehicle->maxPower*1000)/(vehicle->maxSpeed*(1000.0f/3600));        //N
        const float maxSpeed = vehicle->maxSpeed*(1000/3600000.0f)*timeFrame;     //m/frame

        float strength;

        if(vehicle->throttle >= 0.0f)
        {
            const float strength0 = vehicle->throttle*maxStrength;     //Accelerate

            const float coef = -strength0/maxSpeed;//-log(100.0f)/maxSpeed;

            strength = coef * vehicle->speed + strength0;//strength0*exp(coef*vehicle->speed);

            if(strength < 0.0f)
            {
                strength = 0.0f;
            }

            /*if(strength <= 0.01f*strength0)
            {
                strength = 0.0f;
            }*/

            //std::cout << strength << " " << 0.0001f*strength0 << std::endl;
        }
        else
        {
            strength = 2*vehicle->throttle*maxStrength;   //Brake
        }

        //std::cout << (vehicle->speed/timeFrame)*3600 << " " << vehicle->maxSpeed << std::endl;

        const float PI = 3.1415926535897932384626433832795f;

        const glm::vec2 vecWeight = glm::vec2(0.0f, -vehicle->mass*1000*9.81f);
        glm::vec2 vecReact = glm::vec2(0.0f, 0.0f);
        const float weightByBogy = (vehicle->mass * 1000 * 9.81f)/vehicle->bogies.size();

        for(unsigned int i=0; i<vehicle->bogies.size(); i++)
        {
            glm::vec4 react = glm::vec4(0.0f, weightByBogy, 0.0f, 1.0f);

            const unsigned int before = vehicle->bogies[i].before;
            const unsigned int after = vehicle->bogies[i].after;
            const glm::vec3 pointBefore = ways[vehicle->indexWay].points[before];
            const glm::vec3 pointAfter = ways[vehicle->indexWay].points[after];

            const float lgSegment = glm::length(pointAfter-pointBefore);
            const float alpha = asin((pointAfter.z-pointBefore.z)/lgSegment) * 180/PI;

            const glm::mat4 rotation = glm::rotate(alpha, 0.0f, 0.0f, 1.0f);

            react = rotation * react;

            vecReact.x += react.x;
            vecReact.y += react.y;
        }

        glm::vec2 trainingByGravity = vecWeight + vecReact;

        //std::cout << vecReact.x << " " << vecReact.y << std::endl;

        if(vehicle->forth)
        {
            if(vehicle->inverter == 1)
            {
                *forth = true;
            }
            else
            if(vehicle->inverter == -1)
            {
                *forth = false;
            }
        }
        else
        {
            if(vehicle->inverter == 1)
            {
                *forth = false;
            }
            else
            if(vehicle->inverter == -1)
            {
                *forth = true;
            }
        }

        if(vehicle->inverter != 0)
        {
            //Training by gravity if no brake
            /*if(*forth)
            {
                if(trainingByGravity.x < 0 && glm::length(trainingByGravity) > strength)
                {
                    *forth = false;
                }
            }
            else
            {
                if(trainingByGravity.x > 0 && glm::length(trainingByGravity) > strength)
                {
                    *forth = true;
                }
            }*/

            if(*forth)
            {
                if(trainingByGravity.x > 0)
                {
                    strength += glm::length(trainingByGravity);
                }
                else
                {
                    strength-=glm::length(trainingByGravity);
                }
            }
            else
            {
                if(trainingByGravity.x > 0)
                {
                    strength -= glm::length(trainingByGravity);
                }
                else
                {
                    strength += glm::length(trainingByGravity);
                }
            }

            float acceleration = strength/(vehicle->mass*1000);       //m/s2

            //std::cout << acceleration << std::endl;

            acceleration = (acceleration/1000)*timeFrame;     //m/s/frame
            acceleration = (acceleration/1000)*timeFrame;     //m/frame2

            //std::cout << acceleration << std::endl;

            vehicle->speed += acceleration;

            if(vehicle->speed < 0)
            {
                vehicle->speed = 0.0f;
            }
        }
    }
}
