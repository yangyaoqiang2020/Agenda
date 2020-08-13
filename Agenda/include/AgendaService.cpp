    #include"AgendaService.hpp"
    #include"Date.hpp"
    #include"User.hpp"
    #include"Storage.hpp"
    #include"Meeting.hpp"
    #include<vector>
    #include<iostream>
    using namespace std;
    
    
    AgendaService::AgendaService(){
        startAgenda();
    }


    AgendaService::~AgendaService(){
        quitAgenda();
    }


    bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
        auto filter=[userName,password](const User &user){
            if(userName==user.getName()&&password==user.getPassword()) return true;
            else return false;
        };
        list<User>temp=m_storage->queryUser(filter);
        int size=temp.size();
        if(size>0) return true;
        else return false;
    }


    bool AgendaService::userRegister(const std::string &userName, const std::string &password,const std::string &email, const std::string &phone){
        User t_user(userName,password,email,phone);
        auto filter=[userName](const User &user){
            if(userName==user.getName()) return  true;
            else return false;
        };
        list<User>temp=m_storage->queryUser(filter);
        if(temp.empty()){
            m_storage->createUser(t_user);
            return true;
        }
        else return false;
    }


    bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
        auto filter=[userName,password](const User &user){
            if(userName==user.getName()&&password==user.getPassword()) return true;
            else return false;
        };
        deleteAllMeetings(userName);
        auto plist=listAllParticipateMeetings(userName);
        for(auto it=plist.begin();it!=plist.end();it++){
            quitMeeting(userName,(*it).getTitle());
            int size=m_storage->deleteUser(filter);
            if(size>0)return true;
            else return false;
        }
    }


    std::list<User> AgendaService::listAllUsers(void) const{
        auto filter=[](const User user){
            return true;
        };
        list<User>tmp=m_storage->queryUser(filter);
        return tmp;
    }


    bool AgendaService::createMeeting(const std::string &userName, const std::string &title, const std::string &startDate, const std::string &endDate,const std::vector<std::string> &participator){
        Date start(startDate);
        Date end(endDate);
        Meeting t_meeting(userName,participator,start,end,title);
        if(participator.empty()){
            return false;
        }
        if(start.isValid(start)==false||end.isValid(end)==false||start>=end){
            return false;
        }
        int flag=0;
        list<User>tmp=listAllUsers();
        for(auto it=tmp.begin();it!=tmp.end();it++){
            if(userName==(*it).getName()){
                flag=1;break;
            }
        }
        if(flag==0) return false;
        auto filter=[title](const Meeting &t_meeting){
            if(t_meeting.getTitle()==title){
                return true;
            }else{
                return false;
            }
        };
        list<Meeting>tmp2=m_storage->queryMeeting(filter);
        if(tmp2.size()==0) return false;   

        auto filter2=[userName](const Meeting &t_meeting){
            if(userName==t_meeting.getSponsor()){
                return true;
            }else{
                return false;
            }
        };
        list<Meeting>tmp3=listAllMeetings(userName);
        for(auto it=tmp3.begin();it!=tmp3.end();it++){
            if((*it).getEndDate()<=start||(*it).getStartDate()>=end){
                return false;
            }
        }
        vector<string>part;
        m_storage->createMeeting(t_meeting);
        for(auto it=participator.begin();it!=participator.end();it++){
            if(addMeetingParticipator(userName,title,(*it))){
                part.push_back(*it);
            }else{
                m_storage->deleteMeeting(filter);
                return false;
            }
        }
        return true;
    }


    bool AgendaService::addMeetingParticipator(const std::string &userName,const std::string &title,const std::string &participator){
        auto filter=[participator](const User &t_user){
            if(participator==t_user.getName()){
                return true;
            }else{
                return false;
            }
        };
        list<User>tmp=m_storage->queryUser(filter);
        if(tmp.size()!=1){
            return false;
        }
        auto filter1=[&](const Meeting &t_meeting){
            if(t_meeting.getSponsor()==userName && t_meeting.getTitle()==title){
                list<Meeting>tmp1=m_storage->queryMeeting([&](const Meeting &tt_meeting){
                                                                                                                        if(tt_meeting.getSponsor()==participator||tt_meeting.isParticipator(participator)){
                                                                                                                            if(t_meeting.getStartDate()>=tt_meeting.getEndDate()||t_meeting.getEndDate()<=tt_meeting.getStartDate()){
                                                                                                                                return false;
                                                                                                                            }else {
                                                                                                                                return true;
                                                                                                                            }
                                                                                                                        }
                                                                                                                        return false;
                                                                                                                    });
                if(tmp1.empty()){
                    return true;
                }
                return false;
            }
            return false;
        };
        auto switcher=[participator](Meeting &t_meeting){
            t_meeting.addParticipator(participator);
        };
        int number=m_storage->updateMeeting(filter1,switcher);
        if(number >0) return true;
        else return false;
    }


    bool AgendaService::removeMeetingParticipator(const std::string &userName,const std::string &title, const std::string &participator){
        if(userName==participator) return false;
        auto filter=[userName](const User &t_user){
            if(t_user.getName()==userName){
                return true;
            }else return false;
        };
        auto filter1=[participator](const User &tt_user){
            if(tt_user.getName()==participator){
                return true;
            }else return false;
        };
        list<User>u_list1=m_storage->queryUser(filter);
        list<User>u_list2=m_storage->queryUser(filter1);

        if(u_list1.empty()||u_list2.empty()){
            return false;
        }
        auto filter2=[userName,title](const Meeting &t_meeting){
            if(t_meeting.getTitle()==title&&t_meeting.getSponsor()==userName){
                return true;
            }else return false;
        };

        list<Meeting> mm_list=m_storage->queryMeeting(filter2);
        if(mm_list.size()!=1) return false;
        Meeting tmp=(*mm_list.begin());
        if(!tmp.isParticipator(participator)) return false;

        auto switcher=[participator](Meeting &t_meeting){
            t_meeting.removeParticipator(participator);
        };
        int number=m_storage->updateMeeting(filter2,switcher);
        if(number>0) return true;
        else return false;
    }


    bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
        auto filter=[userName,title](const Meeting &t_meeting){
            if(t_meeting.isParticipator(userName)&&t_meeting.getTitle()==title){
                return true;
            }else return false;
        };
        auto switcher=[userName](Meeting &t_meeting){
            t_meeting.removeParticipator(userName);
        };
        int number=m_storage->updateMeeting(filter,switcher);

        auto filter1=[](const Meeting &t_meeting){
            if(t_meeting.getParticipator().empty()){
                return true;
            }else return false;
        };
        m_storage->deleteMeeting(filter1);

        if(number>0) return true;
        else return false;
    }


    std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,const std::string &title) const{
        list<Meeting>tmp;
        auto filter=[userName](const User &t_user){
            if(t_user.getName()==userName){
                return true;
            }else return false;
        };
        list<User>u_list=m_storage->queryUser(filter);

        if(u_list.empty()){
            return tmp;
        }
        auto filter1=[userName,title](const Meeting &t_meeting){
            if((t_meeting.isParticipator(userName)||userName==t_meeting.getSponsor())&&title==t_meeting.getTitle()){
                return true;
            }else return false;
        };
        tmp=m_storage->queryMeeting(filter1);
        return tmp;
    }


    std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate, const std::string &endDate) const{
        Date start(startDate);
        Date end(endDate);
        list<Meeting> tmp;
        if(start.isValid(start)==false||end.isValid(end)==false||start>end) return tmp;

        auto filter=[userName](const User &t_user){
            if(t_user.getName()==userName){
                return true;
            }else return false;
        };
        list<User>u_list=m_storage->queryUser(filter);
        if(u_list.empty()) return tmp;

        auto filter1=[userName,start,end](const Meeting &t_meeting){
            if((t_meeting.getSponsor()==userName||t_meeting.isParticipator(userName))&&end>=t_meeting.getStartDate()&&start<=t_meeting.getEndDate()){
                return true;
            }else return false;
        };
        tmp=m_storage->queryMeeting(filter1);
        return tmp;
    }


    std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
        auto filter=[userName](const Meeting &t_meeting){
            if(t_meeting.isParticipator(userName)||t_meeting.getSponsor()==userName){
                return true;
            }else return false;
        };
        list<Meeting>tmp=m_storage->queryMeeting(filter);
        return tmp;
    }


    std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
        auto filter=[userName](const Meeting &t_meeting){
            if(t_meeting.getSponsor()==userName){
                return true;
            }else return false;
        };
        list<Meeting>tmp=m_storage->queryMeeting(filter);
        return tmp;
    }


    std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const{
        auto filter=[userName](const Meeting &t_meeting){
            if(t_meeting.isParticipator(userName)){
                return true;
            }else return false;
        };
        list<Meeting>tmp=m_storage->queryMeeting(filter);
        return tmp;
    }


    bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
        auto filter=[userName,title](const Meeting &t_meeting){
            if(userName==t_meeting.getSponsor() && title==t_meeting.getTitle()){
                return true;
            }else return false;
        };
        int number=m_storage->deleteMeeting(filter);
        if(number>0)return true;
        else return false;
    }


    bool AgendaService::deleteAllMeetings(const std::string &userName){
        auto filter=[userName](const Meeting &t_meeting){
            if(userName==t_meeting.getSponsor()){
                return true;
            }else return false;
        };
        int number=m_storage->deleteMeeting(filter);
        if(number>0)return true;
        else return false;
    }

    
    void AgendaService::startAgenda(void){
        m_storage=Storage::getInstance();
    }


    void AgendaService::quitAgenda(void){
        m_storage->sync();
    }
