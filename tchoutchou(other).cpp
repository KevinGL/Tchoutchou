#include "tchoutchou.h"
#include <iostream>
#include <conio.h>

namespace tch
{
    bool interSemiSphereSeg(SemiSphere sph, Segment seg, glm::vec3 &inter)
    {
        const float xC = sph.center.x;
        const float yC = sph.center.y;
        const float zC = sph.center.z;

        const float R = sph.radius;

        const float x0 = seg.p1.x;
        const float y0 = seg.p1.y;
        const float z0 = seg.p1.z;

        const float xV = seg.p2.x - seg.p1.x;
        const float yV = seg.p2.y - seg.p1.y;
        const float zV = seg.p2.z - seg.p1.z;

        const float a = xV * xV + yV * yV + zV * zV;
        const float b = 2 * x0 * xV - 2 * xV * xC + 2 * y0 * yV  - 2 * yV * yC + 2 * z0 * zV - 2 * zV * zC;
        const float c = x0 * x0 + y0 * y0 + z0 * z0 + xC * xC + yC * yC + zC * zC - 2 * x0 * xC - 2 * y0 * yC - 2 * z0 * zC - R * R;

        const float delta = b * b - 4 * a * c;

        /*std::cout << xC << " " << yC << " " << zC << " " << R << std::endl;
        std::cout << x0 << " " << y0 << " " << z0 << std::endl;
        std::cout << xV << " " << yV << " " << zV << std::endl;
        std::cout << a << " " << b << " " << c << std::endl;
        std::cout << delta << std::endl << std::endl;
        getch();*/

        /*std::cout << delta << std::endl << std::endl;
        getch();*/

        bool res;

        if(delta < 0)
        {
            res = false;
        }

        else
        if(delta == 0)
        {
            const float t = -b / (2*a);

            glm::vec3 cand;

            cand.x = x0 + t * xV;
            cand.y = y0 + t * yV;
            cand.z = z0 + t * zV;

            glm::vec3 miSeg = seg.p1 + seg.p2;
            miSeg /= 2;

            const float lgSeg = glm::length(seg.p1 - seg.p2);

            const float distInterMi = glm::length(cand - miSeg);

            glm::vec3 vecCenterInter = glm::normalize(cand - sph.center);

            if(distInterMi < lgSeg/2 && glm::dot(sph.dir, vecCenterInter) >= 0)
            {
                inter = cand;
                res = true;
            }
            else
            {
                 res = false;
            }
        }

        else
        {
            const float t1 = (-b - sqrt(delta)) / (2*a);

            glm::vec3 cand;

            cand.x = x0 + t1 * xV;
            cand.y = y0 + t1 * yV;
            cand.z = z0 + t1 * zV;

            glm::vec3 miSeg = seg.p1 + seg.p2;
            miSeg /= 2;

            const float lgSeg = glm::length(seg.p1 - seg.p2);

            const float distInterMi = glm::length(cand - miSeg);

            glm::vec3 vecCenterInter = glm::normalize(cand - sph.center);

            if(distInterMi < lgSeg/2 && glm::dot(sph.dir, vecCenterInter) >= 0)
            {
                inter = cand;
                res = true;
            }
            else
            {
                const float t2 = (-b + sqrt(delta)) / (2*a);

                glm::vec3 cand;

                cand.x = x0 + t2 * xV;
                cand.y = y0 + t2 * yV;
                cand.z = z0 + t2 * zV;

                glm::vec3 miSeg = seg.p1 + seg.p2;
                miSeg /= 2;

                const float lgSeg = glm::length(seg.p1 - seg.p2);

                const float distInterMi = glm::length(cand - miSeg);

                glm::vec3 vecCenterInter = glm::normalize(cand - sph.center);

                if(distInterMi < lgSeg/2 && glm::dot(sph.dir, vecCenterInter) >= 0)
                {
                    inter = cand;
                    res = true;
                }
                else
                {
                    res = false;
                }
            }
        }

        return res;
    }

    void calculAngles(const glm::vec3 p1, const glm::vec3 p2, float *angleZ, float *angleY)
    {
        const glm::vec3 vecSeg = glm::normalize(p2 - p1);
        const glm::vec3 vecSegProj = glm::vec3(vecSeg.x, vecSeg.y, 0.0f);

        *angleZ = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), vecSeg)) * 180/PI;

        if(vecSeg.y < 0)
        {
            *angleZ *= -1;
        }

        *angleY = atan(vecSeg.z / glm::length(vecSegProj)) * 180/PI;
    }
}
