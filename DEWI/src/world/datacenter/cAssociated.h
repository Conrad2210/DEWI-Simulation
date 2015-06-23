/*
 * cAssociated.h
 *
 *  Created on: 22 Jun 2015
 *      Author: R00118979
 */

#ifndef CASSOCIATED_H_
#define CASSOCIATED_H_

class cAssociated
{
    public:
	cAssociated();
	cAssociated(int ix, const char* na, bool as, int st, int parix, const char* parna);
	virtual ~cAssociated();

    private:
	int index;
	const char *name;
	bool associated;
	int stage;
	int parentIndex;
	const char * parentName;


    public:
	int getIndex(){return index;}
	const char * getName(){return name;}
	bool getAssosicated(){return associated;}
	int getStage(){return stage;}
	int getParentIndex(){return parentIndex;}
	const char *getParentName(){return parentName;}

	void setIndex(int i){index = i;}
	void setName(const char* c){name = c;}
	void setAssociated(bool b){associated = b;}
	void setStage(int i){stage = i;}
	void setParentIndex(int i){parentIndex = i;}
	void setParentName(const char* c){parentName = c;}


};

#endif /* CASSOCIATED_H_ */
