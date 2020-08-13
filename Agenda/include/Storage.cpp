#include "Storage.hpp"
#include"Path.hpp"
#include"Date.hpp"
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;


Storage::Storage(){
    m_dirty=false;
    readFromFile();
}


Storage::~Storage(){
    sync();
}

bool Storage::readFromFile(void){
    ifstream file1(Path::meetingPath);
    ifstream file2(Path::userPath);
    if(file1.is_open()==false){
        cout<<"wrong"<<endl;
        return false;
    }
    if(file2.is_open()==false){
        cout<<"wrong"<<endl;
        return false;
    }
    string str1 ;
    while(getline(file1,str1)){
        if(str1.size()==0){
            continue;
        }
    int len=str1.size();
    vector<int>pos;
    for(int i=0;i<len;i++){
        if(str1[i]=='"'){
            pos.push_back(i);
        } 
    }

    string name;
    string pass;
    string email;
    string phone;
    
    for(int i = pos[0]+1 ; i< pos[1] ; i++)
        name.push_back(str1[i]);
    for(int i= pos[2]+1 ; i <pos[3] ; i++)
        pass.push_back(str1[i]);
    for(int i = pos[4]+1 ; i< pos[5] ; i++)
        email.push_back(str1[i]);
    for(int i = pos[6]+1 ; i< pos[7] ; i++)
        phone.push_back(str1[i]);
    User t_user(name,pass,email,phone);
    m_userList.push_back(t_user);
    }
    file1.close();
    string str2;
    while(getline(file2,str2)){
        if(str2.size()==0){
            continue;
        }
        int len=str2.size();
        vector<int> pos;

		for(int i = 0; i < len ; i++){
            if(str2[i] == '"')
                pos.push_back(i);
        }

        string sponsor;
        string participators;
        string startDate;
        string endDate;
        string title;

        for(int i = pos[0]+1 ; i < pos[1] ; i++){
            sponsor.push_back(str2[i]);
        }
        for(int i = pos[2]+1 ; i < pos[3] ; i++){
            participators.push_back(str2[i]);
        }
        for(int i = pos[4]+1 ; i < pos[5] ; i++){
            startDate.push_back(str2[i]);
        }	
        for(int i = pos[6]+1 ; i < pos[7] ; i++){
            endDate.push_back(str2[i]);
        }
        for(int i = pos[8]+1 ; i < pos[9] ; i++){
            title.push_back(str2[i]);
        }
        vector<string>t_participators;
        int lens=t_participators.size();
        string part;
        for(int i=0;i<lens;i++){
            if(participators[i]!='&'){
                part.push_back(participators[i]);
            }else{
                string tmp=part;
                t_participators.push_back(tmp);
                part.clear();
            }
        }
        t_participators.push_back(part);
        Date start(startDate);
		Date end(endDate);
		Meeting meeting(sponsor, t_participators, start, end, title);
		m_meetingList.push_back(meeting);
    }
    file2.close();
    return true;
}


bool Storage::writeToFile(void){
    ofstream file1(Path::userPath);
    ofstream file2(Path::meetingPath);
    if(!file1.is_open()){
        return false;
    }
    if(!file2.is_open()){
        return false;
    }
    for(auto it=m_userList.begin();it!=m_userList.end();it++){
        file1<<'"'<<(*it).getName()<<'"'<<','<<'"'<<(*it).getPassword()<<'"'<<','<<'"'<<(*it).getEmail()<<'"'<<','<<'"'<<(*it).getPhone()<<'"';
        file1<<"\n";
    }
    file1.close();
    for(auto it=m_meetingList.begin();it!=m_meetingList.end();it++){
        file2<<'"'<<(*it).getSponsor()<<'"'<<','<<'"';
        vector<string>part=(*it).getParticipator();
        int len=part.size();
        for(int i=0;i<len-1;i++){
            file2<<part[i]<<'&';
        }
        file2<<part[len-1];
        file2<<'"'<<','<<'"'<<(*it).getStartDate().dateToString((*it).getStartDate())<<'"'<<','<<'"'<<(*it).getEndDate().dateToString((*it).getEndDate())<<'"'<<','<<'"'<<(*it).getTitle()<<'"';
        file2<<"\n";
    }
    file2.close();
    return true;
}


std::shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance==nullptr){
        m_instance=shared_ptr<Storage>(new Storage());
    }
    return m_instance;
}


void Storage::createUser(const User &t_user){
    m_userList.push_back(t_user);
    m_dirty = true;
}


list<User> Storage::queryUser(std::function<bool(const User &t_user)> filter) const{
    list<User>users;
    for(auto itr=m_userList.begin();itr!=m_userList.end();itr++){
        if(filter(*itr)){
            users.push_back(*itr);
        }
    }
    return users;
}


int Storage::updateUser(std::function<bool(const User &)> filter , std::function<void(User &)> switcher){
    int count=0;
    for(auto itr=m_userList.begin();itr!=m_userList.end();itr++){
        if(filter(*itr)){
            switcher(*itr);
            count++;
        }
    }
    if(count != 0) m_dirty = true;
    return count;
}


int Storage::deleteUser(std::function<bool(const User &)> filter){
    int count=0;
    for(auto itr=m_userList.begin();itr!=m_userList.end();itr++){
        if(filter(*itr)){
            m_userList.push_back(*itr);
            count++;
        }
    }
    if(count != 0) m_dirty = true;
return count;
}


void Storage::createMeeting(const Meeting &t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty=true;
}


std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &t_meeting)> filter) const{
    list<Meeting>meetings;
    for(auto itr=m_meetingList.begin();itr!=m_meetingList.end();itr++){
        if(filter(*itr)){
            meetings.push_back(*itr);
        }
    }
    return meetings;
}


int Storage::updateMeeting(std::function<bool(const Meeting &)> filter , std::function<void(Meeting &)> switcher){
    int count=0;
    for(auto itr=m_meetingList.begin();itr!=m_meetingList.end();itr++){
        if(filter(*itr)){
            switcher(*itr);
            count++;
        }
    }
    if(count != 0) m_dirty = true;
    return count;
}


int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter){
    int count = 0;
    for (auto itr = m_meetingList.begin(); itr != m_meetingList.end(); itr++) {
        if (filter(*itr)) {
            m_meetingList.erase(itr);
            count++;
        }
    }
    if(count != 0) m_dirty = true;
    return count;
}


bool Storage::sync(void){
    if(m_dirty == false) return false;
    else{
        bool flag = writeToFile();
        if(flag == false) return false;
        }
    return true;
}
std::shared_ptr<Storage>Storage:: m_instance=nullptr;

