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

package dewi.applications.RLLApp;


import  dewi.applications.trafgen.TrafGenPar;

simple RLLApp extends TrafGenPar
{
   @class(RLLApp);
   bool debug;
   
   bool LightSwitch = default(false);
   
   double burstDuration @unit(s) = default(2s);
   double interArrivalTime @unit(s) = default(0.1s);
   double BurstPause @unit(s) = default(10s);
   double StartTime @unit(s) = default(5s);
   int numberOfBursts  = default(5000);
   
   bool rec_scaRxMsg = default(false);
   bool rec_scaTxMsg = default(false);
   bool rec_E2E = default(false);
   bool rec_receiveMsg = default(false);
   
}