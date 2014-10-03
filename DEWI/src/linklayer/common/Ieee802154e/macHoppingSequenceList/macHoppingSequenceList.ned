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

// @author: Stefan Reis
// Copyright(c) 2014: Stefan Reis
// Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
// Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland

package dewi.linklayer.common.Ieee802154e.macHoppingSequenceList;

simple macHoppingSequenceList
{
	parameters:
        bool readHoppingSequenceListFromFile = default(false);
        string hoppingSequenceListSubFolder = default("R:\\00_GIT\\tables\\HoppingSequenceList\\");
        string hoppingSequenceListFilename = default("hoppingSequenceList");
        @display("i=block/table");
}