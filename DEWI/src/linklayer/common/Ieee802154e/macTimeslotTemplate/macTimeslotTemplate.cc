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
/*
 * @author: Conrad Dandelski
 * Copyright(c) 2014: Conrad Dandelski
 * Copyright(c) 2014: Nimbus Centre for Embedded Systems Research, Cork, Ireland
 * Copyright(c) 2014: Cork Institute of Technology, Cork, Ireland
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <sstream>
#include <macTimeslotTemplate.h>

Define_Module(macTimeslotTemplate);

#define TEMPLATEIDS_START 0

std::ostream& operator<<(std::ostream& os, const macTimeslotTemplateEntry& e)
{
    os << e.info();
    return os;
}

macTimeslotTemplate::macTimeslotTemplate()
{
    nb = NULL;
    tmpNumTemplates = -1;
    tmpTemplateList = NULL;

}

macTimeslotTemplate::~macTimeslotTemplate()
{
    for (int i = 0; i < (int) idToTemplate.size(); i++)
        delete idToTemplate[i];
    delete[] tmpTemplateList;
}

void macTimeslotTemplate::initialize(int stage)
{
    if (stage == 0)
    {
        nb = NotificationBoardAccess().get();
        nb->subscribe(this, NF_TEMPLATE_CREATED);
        nb->subscribe(this, NF_TEMPLATE_DELETED);
        nb->subscribe(this, NF_TEMPLATE_CONFIG_CHANGED);
        nb->subscribe(this, NF_TEMPLATE_STATE_CHANGED);
    }
    else if (stage == 1)
    {
        WATCH_PTRVECTOR(idToTemplate);
        updateDisplayString();
    }
}

void macTimeslotTemplate::updateDisplayString()
{
    if (!ev.isGUI())
        return;
    char buf[80];
    sprintf(buf, "%d Templates", getNumTemplates());
    getDisplayString().setTagArg("t", 0, buf);
}

void macTimeslotTemplate::handleMessage(cMessage *msg)
{
    throw cRuntimeError("this module doesn't process messages");
}

void macTimeslotTemplate::receiveChangeNotification(int category, const cObject *details)
{
    Enter_Method_Silent
    ();
    printNotificationBanner(category, details);
}

cModule *macTimeslotTemplate::getHostModule()
{
    return findContainingNode(this);
}

int macTimeslotTemplate::getNumTemplates()
{
    if (tmpNumTemplates == -1)
    {
        int n = 0;
        int maxId = idToTemplate.size();
        for (int i = 0; i < maxId; i++)
            n++;
        tmpNumTemplates = n;
    }
    return tmpNumTemplates;
}

macTimeslotTemplateEntry *macTimeslotTemplate::getTemplate(int pos)
{
    int n = getNumTemplates(); //also fills tmpNumTemplates;

    if (pos < 0 || pos >= n)
        throw cRuntimeError("getTemplate(): Template index %d out of range 0..%d", pos, n - 1);

    if (!tmpTemplateList)
    {
        tmpTemplateList = new macTimeslotTemplateEntry *[n];
        int k = 0;
        int maxId = idToTemplate.size();

        for (int i = 0; i < maxId; i++)
        {
            if (idToTemplate[i])
                tmpTemplateList[k++] = idToTemplate[i];
        }
    }

    return tmpTemplateList[pos];
}

macTimeslotTemplateEntry *macTimeslotTemplate::getTemplateById(int id)
{
    id -= TEMPLATEIDS_START;
    return (id < 0 || id >= (int) idToTemplate.size()) ? NULL : idToTemplate[id];
}

void macTimeslotTemplate::addTemplate(macTimeslotTemplateEntry *entry)
{
    if (!nb)
        throw cRuntimeError("TimeslotTemplate must precede all node Link blablabla");
    //insert Template
    entry->setTemplateId(TEMPLATEIDS_START + idToTemplate.size());

    //check name is unique
    if (getTemplateById(entry->getTemplateId()) != NULL)
        throw cRuntimeError("addTemplate(): Template '%d' already registered", entry->getTemplateId());

    idToTemplate.push_back(entry);
    invalidTmpTemplateList();

    nb->fireChangeNotification(NF_TEMPLATE_CREATED, entry);
    updateDisplayString();
}

void macTimeslotTemplate::editTimeslotTemplate(macTimeslotTemplateEntry *entry)
{
    bool tmp = false;

    if (getTemplateById(entry->getTemplateId()) == NULL)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << "not possible to edit link:"
                    << entry->getTemplateId() << ", Template doesn't exist!" << endl;

        return;
    }

    //Notification for change in template
    nb->fireChangeNotification(NF_TEMPLATE_CONFIG_CHANGED, getTemplateById(entry->getTemplateId()));

    //change CCA offset if different and if not -1
    if (entry->getCCAOffset() != getTemplateById(entry->getTemplateId())->getCCAOffset() && entry->getCCAOffset() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed CCA Offset" << endl;
        getTemplateById(entry->getTemplateId())->setCCAOffset(entry->getCCAOffset());
        tmp = true;
    }

    //change CCA if different and if not -1
    if (entry->getCCA() != getTemplateById(entry->getTemplateId())->getCCA() && entry->getCCA() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed CCA" << endl;
        getTemplateById(entry->getTemplateId())->setCCA(entry->getCCA());
        tmp = true;
    }

    //change TxOffset if different and if not -1
    if (entry->getTxOffset() != getTemplateById(entry->getTemplateId())->getTxOffset() && entry->getTxOffset() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed TxOffset" << endl;
        getTemplateById(entry->getTemplateId())->setTxOffset(entry->getTxOffset());
        tmp = true;
    }

    //change RxOffset if different and if not -1
    if (entry->getRxOffset() != getTemplateById(entry->getTemplateId())->getRxOffset() && entry->getRxOffset() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed RxOffset" << endl;
        getTemplateById(entry->getTemplateId())->setRxOffset(entry->getRxOffset());
        tmp = true;
    }

    //change RxAckDelay if different and if not -1
    if (entry->getRxAckDelay() != getTemplateById(entry->getTemplateId())->getRxAckDelay()
            && entry->getRxAckDelay() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed RxAckDelay" << endl;
        getTemplateById(entry->getTemplateId())->setRxAckDelay(entry->getRxAckDelay());
        tmp = true;
    }

    //change TxAckDelay if different and if not -1
    if (entry->getTxAckDelay() != getTemplateById(entry->getTemplateId())->getTxAckDelay()
            && entry->getTxAckDelay() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed TxAckDelay" << endl;
        getTemplateById(entry->getTemplateId())->setTxAckDelay(entry->getTxAckDelay());
        tmp = true;
    }

    // change RxWait if different and if not -1
    if (entry->getRxWait() != getTemplateById(entry->getTemplateId())->getRxWait() && entry->getRxWait() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed RxWait" << endl;
        getTemplateById(entry->getTemplateId())->setRxWait(entry->getRxWait());
        tmp = true;
    }

    //change AckWait if different and if not -1
    if (entry->getAckWait() != getTemplateById(entry->getTemplateId())->getAckWait() && entry->getAckWait() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed AckWait" << endl;
        getTemplateById(entry->getTemplateId())->setAckWait(entry->getAckWait());
        tmp = true;
    }

    //change RxTx turnaround
    if (entry->getRxTx() != getTemplateById(entry->getTemplateId())->getRxTx() && entry->getRxTx() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed RxTx turnaround" << endl;
        getTemplateById(entry->getTemplateId())->setRxTx(entry->getRxTx());
        tmp = true;
    }

    //change Max Ack if different and if not -1
    if (entry->getMaxAck() != getTemplateById(entry->getTemplateId())->getMaxAck() && entry->getMaxAck() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed maxAck" << endl;
        getTemplateById(entry->getTemplateId())->setMaxAck(entry->getMaxAck());
        tmp = true;
    }

    //change maxTx if different and if not -1
    if (entry->getMaxTx() != getTemplateById(entry->getTemplateId())->getMaxTx() && entry->getMaxTx() != -1)
    {
        if (ev.isGUI())
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                    << "; changed maxTx" << endl;
        getTemplateById(entry->getTemplateId())->setMaxTx(entry->getMaxTx());
        tmp = true;
    }

    if (entry->getTimeslotLength() != getTemplateById(entry->getTemplateId())->getTimeslotLength()
            && entry->getTimeslotLength() != -1)
    {
        if(ev.isGUI())
        {
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited template: " << entry->getTemplateId()
                                << "; changed TimeslotLength" << endl;
        }
        getTemplateById(entry->getTemplateId())->setTimeslotLength(entry->getTimeslotLength());
        tmp = true;
    }

        if (tmp)
        {
            nb->fireChangeNotification(NF_TEMPLATE_CONFIG_CHANGED, getTemplateById(entry->getTemplateId()));
        }
        else if (ev.isGUI())
        {
            EV << getParentModule()->getParentModule()->getFullName() << ": Edited Template: " << entry->getTemplateId()
                    << "; Nothing changed" << endl;
        }

}

void macTimeslotTemplate::deleteTemplate(macTimeslotTemplateEntry *entry)
{
    int id = entry->getTemplateId();

    if (entry != getTemplateById(id))
        throw cRuntimeError("deleteTemplate(): template '%d' not found in template table", entry->getTemplateId());

    nb->fireChangeNotification(NF_TEMPLATE_DELETED, entry);
    idToTemplate[id - TEMPLATEIDS_START] = NULL;
    delete entry;
    invalidTmpTemplateList();
    updateDisplayString();
}

void macTimeslotTemplate::templateChanged(macTimeslotTemplateEntry *entry, int category)
{
    nb->fireChangeNotification(category, entry);
    if (ev.isGUI())
        updateDisplayString();
}

void macTimeslotTemplate::resetTemplates()
{
    int n = idToTemplate.size();
    for (int i = 0; i < n; i++)
    {
        if (idToTemplate[i])
            idToTemplate[i]->resetTemplate();
    }
}

void macTimeslotTemplate::invalidTmpTemplateList()
{
    tmpNumTemplates = -1;
    delete[] tmpTemplateList;
    tmpTemplateList = NULL;
}

