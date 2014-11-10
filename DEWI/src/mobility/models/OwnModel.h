//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef OWNMODEL_H_
#define OWNMODEL_H_

#include <StationaryMobility.h>

class Node
{
private:
    double m_positionX;
    double m_positionY;
    double m_positionZ;

public:
    Node(double positionX = 0.0, double positionY = 0.0, double positionZ = 0.0);
    ~Node();
    void setPositionX(double X);
    void setPositionY(double Y);
    void setPositionZ(double Z);
    double getPositionX();
    double getPositionY();
    double getPositionZ();
};

class INET_API OwnModel: public StationaryMobility {

protected:
    double m_marginX;
    double m_marginY;
    double m_marginZ;
    double m_SeparationX;
    double m_SeparationY;
    double m_distanceWall;
    double m_distanceCeiling;
    int m_maxRow;
    int m_maxCol;
    int m_numHosts;

    Node *m_node;

public:
    OwnModel();

protected:
    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int stage);

    /** @brief Initializes the position according to the mobility model. */
    virtual void initializePosition();

    /** @brief Save the host position. */
    virtual void finish();


};

#endif /* OWNMODEL_H_ */
