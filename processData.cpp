/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */
#include "eventLib.h"
#include "dbLib.h"

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

//Ham xu ly theo tung id
void DataEvent::Process_Ninja()
{
	L1Item<NinjaInfo_t> *pCur = p,
		*pPre = NULL, *pre = NULL;

	bool preBool = true, //true di chuyen, false dung lai
		boolEvent5 = true,
		boolEvent14 = true;

	L1List<NinjaInfo_t> List;
	List.push_back(p->data);

	while (pCur)
	{
		if (strcmp(id, pCur->data.id) == 0)
		{
			if (pPre == NULL)
			{
				pre = pPre = pCur;
				pCur = pCur->pNext;
				continue;
			}
			if (distanceEarth(pre->data.latitude, pre->data.longitude, pCur->data.latitude, pCur->data.longitude)*1000.0 <= 5.0)
			{
				if (preBool == true)
				{
					TheLast_Move = pre;
					Number_UnMove++;
				}
				Time_UnMove += pCur->data.timestamp - pPre->data.timestamp;
				preBool = false;
			}
			else
			{
				if (boolEvent5)
				{
					boolEvent5 = false;
					if (preBool == true)
					{
						TheFirst_Move = pre;
					}
					else
					{
						TheFirst_Move = pCur;
					}
				}
				if (boolEvent14)
				{
					L1Item<NinjaInfo_t> *pL = List.get_Head();
					while (pL)
					{
						if (distanceEarth(pL->data.latitude, pL->data.longitude, pCur->data.latitude, pCur->data.longitude)*1000.0 <= 5.0)
						{
							Ninja_Lost = true;
							boolEvent14 = false;
						}
						pL = pL->pNext;
					}
					List.push_back(pCur->data);
				}
				Time_Move += pCur->data.timestamp - pPre->data.timestamp;
				preBool = true;
				pre = pCur;
			}
			S_Move += distanceEarth(pPre->data.latitude, pPre->data.longitude, pCur->data.latitude, pCur->data.longitude);
		}
		else
			break;
		pPre = pCur;
		pCur = pCur->pNext;
	}
	List.Destroy();
}

inline bool operator==(DataEvent& lhs, DataEvent& rhs) {
	return strcmp(lhs.id, rhs.id) == 0;
}

inline bool operator!=(DataEvent& lhs, DataEvent& rhs) {
	return !(lhs == rhs);
}

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

//Ham luu lai thoi diem dau tien va cuoi cung duoc luu tru
void Data::Save()
{
	if (!Data_Event.isEmpty())
	{
		Id_First_Save = new L1Item<NinjaInfo_t>(Data_Event.get_Head()->data.p->data);
		Id_Last_Save = new L1Item<NinjaInfo_t>(Data_Event.get_Tail()->data.p->data);
	}
}

//Ham xu ly du lieu
void Data::Process()
{
	L1Item<DataEvent> *p = Data_Event.get_Head();
	while (p)
	{
		p->data.Process_Ninja();
		p = p->pNext;
	}
}

//Ham nhap du lieu
void Data::Load_Data(L1List<NinjaInfo_t> DataList)
{
	L1Item<NinjaInfo_t> *pCur = DataList.get_Head(),
		*pPre = NULL;
	while (pCur)
	{
		if (pPre == NULL || pPre->data != pCur->data)
		{
			DataEvent a(pCur, pCur->data.id);
			Data_Event.push_back(a);
		}
		pPre = pCur;
		pCur = pCur->pNext;
	}
}

//Ham nhap danh sach su kien
void Data::Load_event(L1List<ninjaEvent_t> EventList)
{
	L1Item<ninjaEvent_t> *p = EventList.get_Head();
	while (p)
	{
		ninjaEvent a(p->data);
		Event.push_back(a);
		p = p->pNext;
	}
}

//Cat n ki tu dau tien cua event
char *coverEvent(char a[], int n)
{
	string str(a);
	string str1 = str.substr(n, strlen(a) - n);
	char *id = new char[ID_MAX_LENGTH];
	strcpy(id, str1.c_str());
	return id;
}

bool processEvent(ninjaEvent_t& event, L1List<NinjaInfo_t>& nList, void* pGData) {
	// TODO: Your code comes here
	/// NOTE: The output of the event will be printed on one line
	/// end by the endline character.

	Data *data = (Data*)pGData;  //Con tro toi vung du lieu Data

	switch (event.code[0])
	{
	case '0':
		//event 0
	{
		L1Item<ninjaEvent_t> *p = data->Event.get_Head();
		cout << event.code << ":";
		while (p)
		{
			cout << " " << p->data.code;
			p = p->pNext;
		}
		cout << endl;
		return true;
	}
	case '1':
	{
		//event 1
		if (strlen(event.code) == 1)
		{
			cout << event.code << ": ";
			L1Item<NinjaInfo> *p = data->Id_First_Save;
			if (p == NULL)
				cout << "empty" << endl;
			else
				cout << p->data.id << endl;
			return true;
		}
		else
		{
			switch (event.code[1])
			{
			case '0':
				//event 10
			{
				if (strlen(event.code) != 2)
					return false;
				cout << event.code << ": ";
				L1Item<DataEvent>* p = data->Data_Event.get_Head();
				if (p == NULL)
					cout << "-1" << endl;
				else
				{
					time_t Max_Time_Move = p->data.Time_Move;
					char id_Max_TimeMove[ID_MAX_LENGTH];
					while (p)
					{
						if (Max_Time_Move < p->data.Time_Move)
							Max_Time_Move = p->data.Time_Move;
						p = p->pNext;
					}
					p = data->Data_Event.get_Head();
					while (p)
					{
						if (Max_Time_Move == p->data.Time_Move)
							strcpy(id_Max_TimeMove, p->data.id);
						p = p->pNext;
					}
					cout << id_Max_TimeMove << endl;
				}
				return true;
			}
			case '1':
				//event 11
			{
				if (strlen(event.code) <= 2)
					return false;
				char even1[EVENT_CODE_SIZE];
				strcpy(even1, coverEvent(event.code, 2));
				cout << event.code << ": ";
				L1Item<DataEvent> *p = data->Data_Event.get_Head();
				L1Item<DataEvent> *q = NULL;
				while (p)
				{
					if (strcmp(even1, p->data.id) > 0)
					{
						if (q == NULL || strcmp(p->data.id, q->data.id) > 0)
						{
							q = p;
						}
					}
					p = p->pNext;
				}
				if (q == NULL)
				{
					cout << "-1" << endl;
					return true;
				}
				else
				{
					char id[ID_MAX_LENGTH];
					strcpy(id, q->data.id);
					cout << id << endl;
					NinjaInfo a(id);
					DataEvent b(id);
					nList.removeItiem(a);
					data->Data_Event.removeItiem(b);
					return true;
				}
			}
			case '2':
				//event 12
			{
				if (strlen(event.code) != 2)
					return false;
				cout << event.code << ": ";
				L1Item<DataEvent>*p = data->Data_Event.get_Head();
				if (p == NULL)
					cout << "-1" << endl;
				else
				{
					char id_MaxTimeUnMove[ID_MAX_LENGTH];
					time_t Max_Time_UnMove = p->data.Time_UnMove;
					while (p)
					{
						if (Max_Time_UnMove < p->data.Time_UnMove)
							Max_Time_UnMove = p->data.Time_UnMove;
						p = p->pNext;
					}
					p = data->Data_Event.get_Head();
					while (p)
					{
						if (Max_Time_UnMove == p->data.Time_UnMove)
							strcpy(id_MaxTimeUnMove, p->data.id);
						p = p->pNext;
					}
					cout << id_MaxTimeUnMove << endl;
				}
				return true;
			}
			case '3':
				//event 13
			{
				if (strlen(event.code) == 2)
					return false;
				else if (strlen(event.code) > 2)
					return true;
			}
			case '4':
				//event 14
			{
				if (strlen(event.code) != 2)
					return false;
				cout << event.code << ":";
				L1Item<DataEvent> *p = data->Data_Event.get_Head();
				if (p == NULL)
					cout << " -1" << endl;
				else
				{
					bool booler = false;
					while (p)
					{
						if (p->data.Ninja_Lost == true)
						{
							booler = true;
							cout << " " << p->data.id;
						}
						p = p->pNext;
					}
					if (booler == false)
						cout << " -1";
					cout << endl;
				}
				return true;
			}
			default:
				return false;
			}
		}
	}
	case '2':
		//event 2
	{
		if (strlen(event.code) != 1)
			return false;
		cout << event.code << ": ";
		L1Item<NinjaInfo_t> *p = data->Id_Last_Save;
		if (p == NULL)
			cout << "empty" << endl;
		else
			cout << p->data.id << endl;
		return true;
	}
	case '3':
		//event 3
	{
		if (strlen(event.code) != 1)
			return false;
		cout << event.code << ": " << data->Data_Event.getSize() << endl;
		return true;
	}
	case '4':
		//event 4
	{
		if (strlen(event.code) != 1)
			return false;
		L1Item<DataEvent> *p = data->Data_Event.get_Head();
		cout << event.code << ": ";
		if (p == NULL)
		{
			cout << "empty" << endl;
			return true;
		}
		else
		{
			char id_Max[ID_MAX_LENGTH];
			strcpy(id_Max, p->data.id);
			while (p)
			{
				if (strcmp(id_Max, p->data.id) < 0)
					strcpy(id_Max, p->data.id);
				p = p->pNext;
			}
			cout << id_Max << endl;
		}
		return true;
	}
	case '5':
		//event 5
	{
		if (strlen(event.code) <= 1)
			return false;
		char even1[EVENT_CODE_SIZE];
		strcpy(even1, coverEvent(event.code, 1));
		cout << event.code << ": ";
		L1Item<DataEvent> *p = data->Data_Event.get_Head();
		while (p)
		{
			if (strcmp(p->data.id, even1) == 0)
			{
				break;
			}
			p = p->pNext;
		}
		if (p == NULL)
		{
			cout << "-1" << endl;
			return true;
		}
		else if (p->data.p->pNext == NULL || strcmp(p->data.id, p->data.p->pNext->data.id) != 0)
		{
			char time[26];
			strPrintTime(time, p->data.p->data.timestamp);
			cout << time << endl;
			return true;
		}
		else if (p->data.TheFirst_Move)
		{
			char time[26];
			strPrintTime(time, p->data.TheFirst_Move->data.timestamp);
			cout << time << endl;
			return true;
		}
		else
		{
			cout << "empty" << endl;
			return true;
		}
	}
	case '6':
		//event 6
	{
		if (strlen(event.code) <= 1)
			return false;
		char even1[EVENT_CODE_SIZE];
		strcpy(even1, coverEvent(event.code, 1));
		cout << event.code << ": ";
		L1Item<DataEvent> *p = data->Data_Event.get_Head();
		while (p)
		{
			if (strcmp(p->data.id, even1) == 0)
			{
				break;
			}
			p = p->pNext;
		}
		if (p == NULL)
		{
			cout << "-1" << endl;
			return true;
		}
		L1Item<NinjaInfo_t> *q = p->data.TheLast_Move;
		if (q == NULL)
		{
			cout << "Non-stop" << endl;
			return true;
		}
		else
		{
			char time1[26];
			strPrintTime(time1, q->data.timestamp);
			cout << time1 << endl;
			return true;
		}
	}
	case '7':
		//event 7
	{
		if (strlen(event.code) <= 1)
			return false;
		char even1[EVENT_CODE_SIZE];
		strcpy(even1, coverEvent(event.code, 1));
		cout << event.code << ": ";

		L1Item<DataEvent> *p = data->Data_Event.get_Head();
		while (p)
		{
			if (strcmp(p->data.id, even1) == 0)
			{
				break;
			}
			p = p->pNext;
		}
		if (p == NULL)
		{
			cout << "-1" << endl;
		}
		else
			cout << p->data.Number_UnMove << endl;
		return true;
	}
	//event 8
	case '8':
		//event 8
	{
		if (strlen(event.code) <= 1)
			return false;
		char even1[EVENT_CODE_SIZE];
		strcpy(even1, coverEvent(event.code, 1));
		cout << event.code << ": ";

		L1Item<DataEvent> *p = data->Data_Event.get_Head();
		while (p)
		{
			if (strcmp(p->data.id, even1) == 0)
			{
				break;
			}
			p = p->pNext;
		}
		if (p == NULL)
		{
			cout << "-1" << endl;
		}
		else
		{
			cout << p->data.S_Move << endl;
		}
		return true;
	}
	case '9':
		//event 9
	{
		if (strlen(event.code) != 1)
			return false;
		cout << event.code << ": ";
		L1Item<DataEvent>*p = data->Data_Event.get_Head();
		if (p == NULL)
			cout << "-1" << endl;
		else
		{
			double max_S_Move = p->data.S_Move;
			char id_Max_SMove[ID_MAX_LENGTH];
			while (p)
			{
				if (max_S_Move < p->data.S_Move)
				{
					max_S_Move = p->data.S_Move;
				}
				p = p->pNext;
			}
			p = data->Data_Event.get_Head();
			while (p)
			{
				if (max_S_Move == p->data.S_Move)
					strcpy(id_Max_SMove, p->data.id);
				p = p->pNext;
			}
			cout << id_Max_SMove << endl;
		}
		return true;
	}
	default:
		return false;
	}
	/// NOTE: The output of the event will be printed on one line
	/// end by the endline character.
}
