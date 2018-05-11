/*
* =========================================================================================
* Name        : dbLib.cpp
* Author      : Duc Dung Nguyen, Nguyen Hoang Minh
* Email       : nddung@hcmut.edu.vn
* Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
* Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
*               This file implements functions used for database management
* =========================================================================================
*/

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

struct DataEvent
{
	L1Item<NinjaInfo_t> *p;
	char id[ID_MAX_LENGTH];

	L1Item<NinjaInfo_t> * TheFirst_Move = NULL, //Thoi diem dau tien ninja di chuyen
		*TheLast_Move = NULL; //Thoi diem cuoi cung ninja di chuyen
	int Number_UnMove = 0; //So lan dung lai
	double S_Move = 0; //Quang duong di chuyen
	time_t Time_Move, Time_UnMove;  //Thoi gian di chuyen va thoi gian dung lai
	bool Ninja_Lost = false;  //Ninja bi lac

	DataEvent() :p(NULL) {
		strcpy(id, "");
	}
	DataEvent(L1Item<NinjaInfo_t> *q) {
		p = q;
	}
	DataEvent(char id[ID_MAX_LENGTH]) {
		strcpy(this->id, id);
	}
	DataEvent(L1Item<NinjaInfo_t>*p, char id[ID_MAX_LENGTH]) {
		this->p = p;
		strcpy(this->id, id);
	}
	void Process_Ninja();
};

class Data
{
public:
	L1List<DataEvent> Data_Event;    //List du lieu theo cac id
	L1List<ninjaEvent_t> Event;		//List Event
	L1Item<NinjaInfo_t> *Id_First_Save = NULL;   //Con tro toi id dau tien
	L1Item<NinjaInfo_t> *Id_Last_Save = NULL;    //Con tro toi id cuoi cung
	Data() :Data_Event() { }
	void Load_Data(L1List<NinjaInfo_t> DataList);
	void Load_event(L1List<ninjaEvent_t> EventList);
	void Load(L1List<NinjaInfo_t> DataList, L1List<ninjaEvent_t> EventList)
	{
		Load_Data(DataList);
		Load_event(EventList);
	}
	void Process();
	void Save();
};

void    strPrintTime(char* des, time_t& t) {
	tm *pTime = localtime(&t);
	strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}

void loadNinjaDB(char* fName, L1List<NinjaInfo_t> &db) {
	// TODO: write code to load information from file into db
	fstream f(fName);
	if (!f)
	{
		cout << "The file is not found!";
		return;
	}
	string str;
	getline(f, str);
	while (!f.eof())
	{
		getline(f, str,',');
		if (str.find("1526")!=-1)
		{
			NinjaInfo_t ninja;
			getline(f, str, ',');
			int mo, dd, yy, hh, mm, ss;
			sscanf(str.c_str(), "%d/%d/%d %d:%d:%d", &mo, &dd, &yy, &hh, &mm, &ss);
			struct tm tm;
			tm.tm_hour = hh;
			tm.tm_mday = dd;
			tm.tm_min = mm;
			tm.tm_mon = mo - 1;
			tm.tm_sec = ss;
			tm.tm_year = yy - 1900;
			ninja.timestamp = mktime(&tm);

			getline(f, str, ',');
			char id[ID_MAX_LENGTH];
			strcpy(id, str.data());
			if (strlen(id) == 1)
			{
				char id1[ID_MAX_LENGTH] = { '0','0','0',id[0] };
				strcpy(ninja.id, id1);
			}
			else if (strlen(id) == 2)
			{
				char id1[ID_MAX_LENGTH] = { '0','0',id[0],id[1] };
				strcpy(ninja.id, id1);
			}
			else if (strlen(id) == 3)
			{
				char id1[ID_MAX_LENGTH] = { '0',id[0],id[1],id[2] };
				strcpy(ninja.id, id1);
			}
			else
			{
				strcpy(ninja.id, id);
			}

			getline(f, str, ',');
			stringstream ss1(str);
			ss1 >> ninja.longitude;

			getline(f, str, ',');
			stringstream ss2(str);
			ss2 >> ninja.latitude;

			getline(f, str);
			db.push_back(ninja);
		}
		else
		{
			getline(f, str);
		}
	}
	f.close();
}

bool parseNinjaInfo(char* pBuf, NinjaInfo_t& nInfo) {
	// TODO: write code to parse information from a string buffer, ignore if you don't use it
	return true;
}

void process(L1List<ninjaEvent_t>& eventList, L1List<NinjaInfo_t>& bList) {
	void*   pGData = NULL;
	if (initNinjaGlobalData(&pGData))
	{
		Data *data = (Data*)pGData;
		data->Load(bList, eventList); //Goi ham load du lieu
		data->Save(); //Goi Ham luu lai id dau va cuoi
		data->Process(); //Goi ham xu ly du lieu
	}
	while (!eventList.isEmpty()) {
		if (!processEvent(eventList[0], bList, pGData))
			cout << eventList[0].code << " is an invalid event\n";
		eventList.removeHead();
	}

	releaseNinjaGlobalData(pGData);
}

bool initNinjaGlobalData(void** pGData) {
	/// TODO: You should define this function if you would like to use some extra data
	/// the data should be allocated and pass the address into pGData
	*pGData = new Data();
	return true;
}

void releaseNinjaGlobalData(void* pGData) {
	/// TODO: You should define this function if you allocated extra data at initialization stage
	/// The data pointed by pGData should be released
	delete static_cast<Data*>(pGData);
	return;
}

void printNinjaInfo(NinjaInfo_t& b) {
	printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}


/// This function converts decimal degrees to radians
double deg2rad(double deg) {
	return (deg * pi / 180);
}

///  This function converts radians to decimal degrees
double rad2deg(double rad) {
	return (rad * 180 / pi);
}

/**
* Returns the distance between two points on the Earth.
* Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
* @param lat1d Latitude of the first point in degrees
* @param lon1d Longitude of the first point in degrees
* @param lat2d Latitude of the second point in degrees
* @param lon2d Longitude of the second point in degrees
* @return The distance between the two points in kilometers
*/
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
	double lat1r, lon1r, lat2r, lon2r, u, v;
	lat1r = deg2rad(lat1d);
	lon1r = deg2rad(lon1d);
	lat2r = deg2rad(lat2d);
	lon2r = deg2rad(lon2d);
	u = sin((lat2r - lat1r) / 2);
	v = sin((lon2r - lon1r) / 2);
	return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}