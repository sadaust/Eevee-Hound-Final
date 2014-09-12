#include "RenVector.h"

RenVector::RenVector() {
	used = 0;
	allocated = 0;
	data = 0;
}

void RenVector::clear() {
	RenInfo temp;
	used = 0;
	temp.type = none;
	temp.asset = 0;
	temp.locCamNum = -1;
	for(int i = 0; i < allocated;++i)
		data[i] = temp;
}

void RenVector::setSize(int size) {
	RenInfo* tempData;
	int smallest;
	if(allocated != 0) {
		tempData = new RenInfo[size];
		smallest = size;
		if(smallest > allocated)
			smallest = allocated;
		for(int i = 0;i < smallest;++i)
			tempData[i] = data[i];
		delete [] data;
		data = tempData;
	} else {
		data = new RenInfo[size];
	}
	if(used>size)
		used = size;
	allocated = size;
}

void RenVector::add(RenInfo& newRen) {
	if(used >= allocated) {
		setSize(allocated+10);
	}
	data[used].asset = newRen.asset;
	data[used].type = newRen.type;
	data[used].locCamNum = newRen.locCamNum;
	++used;
}

RenInfo* RenVector::get(int loc) {
	if(loc < allocated&&loc>=0)
		return &data[loc];
	else
		return 0;
}

RenVector::~RenVector() {
	if(data)
		delete [] data;
	allocated = 0;
	used = 0;
}