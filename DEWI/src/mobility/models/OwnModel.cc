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

#include <OwnModel.h>
Define_Module(OwnModel);
Node::Node(double positionX, double positionY, double positionZ)
{
    m_positionX = positionX;
    m_positionY = positionY;
    m_positionZ = positionZ;
}
Node::~Node()
{

}

void Node::setPositionX(double X)
{
    m_positionX = X;
}

void Node::setPositionY(double Y)
{
    m_positionY = Y;
}

void Node::setPositionZ(double Z)
{
    m_positionZ = Z;
}

double Node::getPositionX()
{
    return m_positionX;
}

double Node::getPositionY()
{
    return m_positionY;
}

double Node::getPositionZ()
{
    return m_positionZ;
}

OwnModel::OwnModel()
{
    // TODO Auto-generated constructor stub
    m_SeparationX = 0.0;
    m_SeparationY = 0.0;
    m_distanceCeiling = 0.0;
    m_distanceWall = 0.0;
    m_marginX = 0.0;
    m_marginY = 0.0;
    m_marginZ = 0.0;
    m_numHosts = 0;
}

void OwnModel::initialize(int stage)
{
    MobilityBase::initialize(stage);
    EV << "initializing OwnMobilityModel stage " << stage << endl;
    if (stage == 0)
    {
        m_numHosts = par("numHosts");
        m_marginX = par("marginX");
        m_marginY = par("marginY");
        m_marginZ = par("marginZ");
        m_distanceWall = par("distanceToWall");
        m_distanceCeiling = par("distanceToCeiling");
        m_maxCol = par("maxCol");
        m_maxRow = par("maxRow");
        m_node = new Node[m_numHosts];

        constraintAreaMin.x = 0;
        constraintAreaMax.x = m_marginX;

        constraintAreaMin.y = 0;
        constraintAreaMax.y = m_marginY;

        constraintAreaMin.z = 0.0;
        constraintAreaMax.z = m_marginZ;

        //OwnModel::CalcPosition();
    }
}
void OwnModel::calcSeparation()
{


}

void OwnModel::CalcPosition()
{


}

void OwnModel::initializePosition()
{
    int index = visualRepresentation->getIndex();
//    int maxRow = 0;
//    int maxCol = 0;
    int size = (int)ceil(sqrt((double)m_numHosts));
    int col = index % size;
    int row = ((int)floor((double)index/(double)size));
//    for(int i = 0; i < m_numHosts; i++)
//    {
//        if(((int)floor((double)i/(double)size))>maxRow)
//            maxRow = ((int)floor((double)i/(double)size));
//
//        if((i % size)>maxCol)
//            maxCol = i % size;
//    }

    m_SeparationX = ((m_marginX - m_distanceWall) - m_distanceWall)/m_maxCol;
    m_SeparationY = ((m_marginY - m_distanceWall) - m_distanceWall)/m_maxRow;

    lastPosition.x = m_distanceWall + col * m_SeparationX;
    lastPosition.y = m_distanceWall + row * m_SeparationY;
    lastPosition.z = constraintAreaMax.z - m_distanceCeiling;
}

void OwnModel::finish()
{
    MobilityBase::finish();
//    recordScalar("x", lastPosition.x);
//    recordScalar("y", lastPosition.y);
}
