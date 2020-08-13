#include "Date.hpp"
#include <cstdio>
using namespace std;

bool isleapyear(int );
int distances(const Date &date1,const Date &date2);

int distances(const Date &date1,const Date &date2){
	if(date1.getYear()==date2.getYear()){
		if(date1.getMonth()==date2.getMonth()){
			if(date1.getDay()==date2.getDay()){
				if(date1.getHour()==date2.getHour()){
					if(date1.getMinute()==date2.getMinute()){
						return 0;
					}else{
						if(date1.getMinute()>date2.getMinute()) return 1;
						if(date1.getMinute()<date2.getMinute()) return -1;
					}
				}else{
					if(date1.getHour()>date2.getHour()) return 1;
					if(date1.getHour()<date2.getHour()) return -1;
				}
			}else{
				if(date1.getDay()>date2.getDay()) return 1;
				if(date1.getDay()<date2.getDay()) return -1;
			}
		}else{
			if(date1.getMonth()>date2.getMonth()) return 1;
			if(date1.getMonth()<date2.getMonth()) return -1;
		}
	}else{
		if(date1.getYear()>date2.getYear()) return 1;
		if(date1.getYear()<date2.getYear()) return -1;
	}
}

Date::Date(){
	this->m_year=0;
	this->m_month=0;
	this->m_day=0;
	this-> m_hour=0;
	this-> m_minute=0;      
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    m_year=t_year;
    m_month=t_month;
    m_day=t_day;
    m_hour=t_hour;
    m_minute=t_minute;
}

Date::Date(const std::string &dateString){
	Date t_date;
	sscanf(dateString.data(),"%d-%d-%d/%d:%d",&t_date.m_year,&t_date.m_month,&t_date.m_day,&t_date.m_hour,&t_date.m_minute);
	
	this->m_year=t_date.m_year;
	this->m_month=t_date.m_month;
	this->m_day=t_date.m_day;
	this->m_hour=t_date.m_hour;
	this->m_minute=t_date.m_minute;	 
}

int Date:: getYear(void) const{
	return m_year;
}

void Date::setYear(const int t_year){
	m_year=t_year;
}

int Date::getMonth(void) const{
	return m_month;
}

void Date::setMonth(const int t_month){
	m_month=t_month;
}

int Date::getDay(void) const{
	return m_day;
}

void Date::setDay(const int t_day){
	m_day=t_day;
}

int Date::getHour(void) const{
	return m_hour;
}

void Date::setHour(const int t_hour){
	m_hour=t_hour;
}

int Date::getMinute(void) const{
	return m_minute;
}

void Date::setMinute(const int t_minute){
	m_minute=t_minute;
}

bool isleapyear (int T){
	if(T%400==0 ||(T%4==0 && T%100!=0) ) return true;
	else return false;
}

bool Date::isValid(const Date &t_date){
	if(t_date.m_year<=9999 && t_date.m_year>=1000 &&t_date.m_month<=12 && t_date.m_month>=1 && 
		t_date.m_hour>=0 && t_date.m_hour<=23 && t_date.m_minute>=0 && t_date.m_minute<=59){
			if((t_date.m_month==1 || t_date.m_month==3 || t_date.m_month==5 || t_date.m_month==7 || t_date.m_month==8 || t_date.m_month==10 || t_date.m_month==12)&&(t_date.m_day>=1&&t_date.m_day<=31)){
				return true;
			}
			else if((t_date.m_month==4 || t_date.m_month==6 || t_date.m_month==9 || t_date.m_month==11 )&&(t_date.m_day>=1&&t_date.m_day<=30)){
				return true;
			}
			else if(isleapyear(t_date.m_year)&&t_date.m_month==2&&(t_date.m_day>=1&&t_date.m_day<=29)){
				return true;
			}
			else if (! isleapyear(t_date.m_year)&&t_date.m_month==2&&(t_date.m_day>=1&&t_date.m_day<=28)){
				return true;
			}
			else{
				return false;
			}

		}
}

Date Date::stringToDate(const std::string &t_dateString){
	Date t_date;
	if(t_dateString.size()==16)
		sscanf(t_dateString.data(),"%d-%d-%d/%d:%d",&t_date.m_year,&t_date.m_month,&t_date.m_day,&t_date.m_hour,&t_date.m_minute);
	return t_date;
}

string Date::dateToString(const Date &t_date){
	if(isValid(t_date)){
		string str=to_string(t_date.m_year)+"-"+to_string(t_date.m_month)+"-"+to_string(t_date.m_day)+"/"+to_string(t_date.m_hour)+":"+to_string(t_date.m_minute);
		return str;
	}else{
		string str="0000-00-00/00:00";
		return str;
	}
}


Date &Date::operator= (const Date &t_date){
	Date temp;
    temp.setYear(t_date.getYear());
    temp.setMonth(t_date.getMonth());
    temp.setDay(t_date.getDay());
    temp.setHour(t_date.getHour());
    temp.setMinute(t_date.getMinute()) ;
	return temp;	  
}


bool Date::operator==(const Date &t_date) const{
if(distances(*this,t_date)==0) return true;
else return false;
}


bool Date::operator>(const Date &t_date) const{
	if(distances(*this , t_date)==1) return true;
	else return false;
}


bool Date::operator<(const Date &t_date) const{
	if(distances(*this,t_date)==-1) return true;
	else return false;
}

bool Date::operator>=(const Date &t_date) const{
	if(distances(*this,t_date)>-1) return true;
	else return false;
}

bool Date::operator<=(const Date &t_date) const{
	if(distances(*this,t_date)<1) return true;
	else return false;
}