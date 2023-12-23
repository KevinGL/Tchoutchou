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

    //void Network::calculAngles(const Point p1, const Point p2, const glm::vec3 posBogy, float *angleZ, float *angleY)
    void Network::calculAngles(const bool isTramway, const Point p1, const Point p2, std::vector<Bogy> &bogies, size_t indexBogy)
    {
        /*const glm::vec3 vecSeg = glm::normalize(p2.pos - p1.pos);
        const glm::vec3 vecSegProj = glm::vec3(vecSeg.x, vecSeg.y, 0.0f);

        *angleZ = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), vecSeg)) * 180/PI;

        if(vecSeg.y > 0)
        {
            *angleZ *= -1;
        }

        *angleY = atan(vecSeg.z / glm::length(vecSegProj)) * 180/PI;*/

        if(isTramway)
        {
            const glm::vec3 pos2 = p1.pos;
            const glm::vec3 pos3 = p2.pos;
            glm::vec3 pos1, pos4;

            const glm::vec3 vecSeg2 = glm::normalize(pos3 - pos2);
            const glm::vec3 vecSegProj2 = glm::vec3(vecSeg2.x, vecSeg2.y, 0.0f);

            if(p1.indexBefore.size() != 0)
            {
                pos1 = points[p1.indexBefore[p1.switchBefore]].pos;
            }
            else
            {
                pos1 = pos2 - vecSeg2;
            }

            if(p2.indexAfter.size() != 0)
            {
                pos4 = points[p2.indexAfter[p2.switchAfter]].pos;
            }
            else
            {
                pos4 = pos3 + vecSeg2;
            }

            const glm::vec3 vecSeg1 = glm::normalize(pos2 - pos1);
            const glm::vec3 vecSegProj1 = glm::vec3(vecSeg1.x, vecSeg1.y, 0.0f);

            const glm::vec3 vecSeg3 = glm::normalize(pos4 - pos3);
            const glm::vec3 vecSegProj3 = glm::vec3(vecSeg3.x, vecSeg3.y, 0.0f);

            ///////////////////////////////////////////////////////////////////////////////

            float alphaZ = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), vecSeg1)) * 180/PI;

            if(vecSeg1.y > 0)
            {
                alphaZ *= -1;
            }

            const float alphaY = atan(vecSeg1.z / glm::length(vecSegProj1)) * 180/PI;

            ///////////////////////////////////////////////////////////////////////////////

            float betaZ = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), vecSeg2)) * 180/PI;

            if(vecSeg2.y > 0)
            {
                betaZ *= -1;
            }

            const float betaY = atan(vecSeg2.z / glm::length(vecSegProj2)) * 180/PI;

            ///////////////////////////////////////////////////////////////////////////////

            float gammaZ = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), vecSeg3)) * 180/PI;

            if(vecSeg3.y > 0)
            {
                gammaZ *= -1;
            }

            const float gammaY = atan(vecSeg3.z / glm::length(vecSegProj3)) * 180/PI;

            ///////////////////////////////////////////////////////////////////////////////

            const float thetaZ = (alphaZ + betaZ) / 2;
            const float thetaY = (alphaY + betaY) / 2;

            const float deltaZ = (betaZ + gammaZ) / 2;
            const float deltaY = (betaY + gammaY) / 2;

            ///////////////////////////////////////////////////////////////////////////////

            const float lgSeg = glm::length(pos3 - pos2);
            const float dist = glm::length(bogies[indexBogy].pos - pos2);

            float coef = (deltaZ - thetaZ) / lgSeg;
            bogies[indexBogy].angleZ = coef * dist + thetaZ;

            coef = (deltaY - thetaY) / lgSeg;
            bogies[indexBogy].angleY = coef * dist + thetaY;

            ///////////////////////////////////////////////////////////////////////////////

            /*if(indexBogy == 0)
            {
                std::cout << "(" << thetaZ << " " << deltaZ << ") " << dist << " / " << lgSeg << " => " << bogies[indexBogy].angleZ << std::endl;
            }*/
        }

        else
        {
            //
        }
    }

    bool crossed(const glm::vec3 bogyPos, const glm::vec3 before, const glm::vec3 after)
    {
        glm::vec3 middle = before + after;
        middle /= 2;

        const float lg = glm::length(after - before);

        if(glm::length(bogyPos - middle) > lg/2)
        {
            return true;
        }

        return false;
    }

    void Network::recalculateBeAf(const glm::vec3 bogyPos, int *indexBe, int *indexAf)
    {
        for(size_t i=0; i<points.size(); i++)
        {
            const Point p1 = points[i];

            if(p1.indexAfter.size() != 0)
            {
                const size_t switchAfter = p1.switchAfter;
                const Point p2 = points[p1.indexAfter[switchAfter]];

                glm::vec3 middle = p1.pos + p2.pos;
                middle /= 2;

                const float lg = glm::length(p2.pos - p1.pos);

                const glm::vec3 vecP1P2 = glm::normalize(p2.pos - p1.pos);
                const glm::vec3 vecP1Bogy = glm::normalize(bogyPos - p1.pos);

                if(glm::dot(vecP1P2, vecP1Bogy) >= 0.99f && glm::length(bogyPos - middle) <= lg/2)
                {
                    *indexBe = i;
                    *indexAf = p1.indexAfter[switchAfter];
                }
            }
        }
    }
}
