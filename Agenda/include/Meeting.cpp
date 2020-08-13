#include "Meeting.hpp"
using  namespace std;

Meeting::Meeting(const std::string &t_sponsor,
    const std::vector<std::string> &t_participator,
    const Date &t_startTime, const Date &t_endTime,
    const std::string &t_title){
        m_sponsor=t_sponsor;
        m_participators.assign(t_participator.begin(),t_participator.end());
        m_startDate=t_startTime;
        m_endDate=t_endTime;
        m_title=t_title;
}


Meeting::Meeting(const Meeting &t_meeting){
    m_sponsor=t_meeting.m_sponsor;
    m_participators.assign(t_meeting.m_participators.begin(),t_meeting.m_participators.end());
    m_startDate=t_meeting.m_startDate;
    m_endDate=t_meeting.m_endDate;
}


std::string Meeting::getSponsor(void) const{
    return m_sponsor;
}


void Meeting:: setSponsor(const std::string &t_sponsor){
    m_sponsor=t_sponsor;
}


std::vector<std::string> Meeting::getParticipator(void) const{
    return m_participators;
}


void Meeting::setParticipator(const std::vector<std::string> &t_participators){
      m_participators.assign(t_participators.begin(),t_participators.end());
}


void Meeting::addParticipator(const std::string &t_participator){
    m_participators.push_back(t_participator);
}


void Meeting::removeParticipator(const std::string &t_participator){
    for(vector<string>::iterator it=m_participators.begin();it!=m_participators.end();){
        if(*it==t_participator){
            it=m_participators.erase(it);
        }else{
            ++it;
        }
    }
}


Date Meeting::getStartDate(void) const{
    return m_startDate;
}


void Meeting::setStartDate(const Date &t_startTime){
    m_startDate=t_startTime;
}


Date Meeting::getEndDate(void) const{
    return m_endDate;
}


void Meeting::Meeting::setEndDate(const Date &t_endTime){
    m_endDate=t_endTime;
}


std::string Meeting::getTitle(void) const{
    return m_title;
}


void Meeting::setTitle(const std::string &t_title){
    m_title=t_title;
}


bool Meeting::isParticipator(const std::string &t_username) const{
   auto first=m_participators.cbegin();
   auto last=m_participators.cend();
   while(first!=last){
       if(*first==t_username) return true;
       else first++;
   }
    return false;
}