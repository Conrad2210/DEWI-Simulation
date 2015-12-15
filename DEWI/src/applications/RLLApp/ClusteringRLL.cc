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

#include <ClusteringRLL.h>
Define_Module(ClusteringRLL);

ClusteringRLL::ClusteringRLL()
{
	// TODO Auto-generated constructor stub

}

ClusteringRLL::~ClusteringRLL()
{
	// TODO Auto-generated destructor stub
}
void ClusteringRLL::initialize(int stage)
{
	RLLApp::initialize(stage);
}
void ClusteringRLL::finish()
{
	RLLApp::finish();
}
void ClusteringRLL::checkAssociation()
{

	if (m_isLightSwitch)
	{
		if (dataCenter->allAssociated())
		{

			simulation.callFinish();
			endSimulation();

		}

		else
		{
			std::cout << endl << "Run number: " << ev.getConfigEx()->getActiveRunNumber() << endl;
			std::cout << "Ass Nodes: " << dataCenter->getNumAssNodes() << " of " << dataCenter->getNumRegisteredAssVectors() << endl;
			double percent = (double) dataCenter->getNumAssNodes() / (double) dataCenter->getNumRegisteredAssVectors() * 100.0;
			std::cout << percent << "%" << endl;
//			if (AssTimer->isScheduled())
//				cancelEvent(AssTimer);
//
//			scheduleAt(simTime() + 5, AssTimer);
		}
	}
}

