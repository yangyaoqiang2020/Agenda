#include "AgendaUI.hpp"
using namespace std;

AgendaUI::AgendaUI(){
    startAgenda();
}


void AgendaUI::OperationLoop(void){
    string operation;
    do{
        cout<<"__________________________Agenda__________________________"<<endl;
        cout<<"Action   :"<<endl;
        cout<<"l          -     log in Agenda by user name and password"<<endl;
        cout<<"r          -     register an Agenda account"<<endl;
        cout<<"q          -     quit Agenda"<<endl;
        cout<<"__________________________________________________________"<<endl;
        cout<<"Agenda   :~$";
        cin>>operation;
        switch(*(operation.begin())){
            case 'l':{
                userLogIn();
                break;
            }
            case 'r':{
                userRegister();
                break;
            }
            case 'q':{
                quitAgenda();
                break;
            }
            default:break;
        }
    }while(*(operation.begin())!='q');
}


void AgendaUI::startAgenda(void){
    m_agendaService.startAgenda();
}


std::string AgendaUI::getOperation(){
    string operation;
    cout<<"__________________________Agenda__________________________"<<endl;
    cout<<"Action   :"<<endl;
    cout<<"o          -     log out of Agenda"<<endl;
    cout<<"dc          -     delete Agenda account"<<endl;
    cout<<"lu          -     list all Agenda user"<<endl;
    cout<<"cm          -     create a meeting"<<endl;
    cout<<"amp          -     add meeting participator"<<endl;
    cout<<"rmp          -     remove meeting participator"<<endl;
    cout<<"rqm          -     request to quit meeting"<<endl;
    cout<<"la          -     list all meetings"<<endl;
    cout<<"las          -     list all sponsor meetings"<<endl;
    cout<<"lap          -     list all participator meetings"<<endl;
    cout<<"qm          -     query meeting by title"<<endl;
    cout<<"qt          -     query meeting by time interval"<<endl;
    cout<<"dm          -     delete meeting by title"<<endl;
    cout<<"da          -     delete all meetings"<<endl;
    cout<<"__________________________________________________________"<<endl<<endl;;
    cout<<"Agenda@"<<m_userName<<"   :~#";
    cin>>operation;
    return operation;
}


bool AgendaUI::executeOperation(const std::string &t_operation){
    if(t_operation=="o"){
        userLogOut();
        return false;
    }else if (t_operation=="dc"){
        deleteUser();
        return false;
    }else if(t_operation=="lu"){
        listAllUsers();
        return true;
    }else if(t_operation=="cm"){
        createMeeting();
        return true;
    }else if(t_operation=="amp"){
        addMeetingParticipator();
        return true;
    }else if(t_operation=="rmp"){
        removeMeetingParticipator();
        return true;
    }else if(t_operation=="rqm"){
        quitMeeting();
        return true;
    }else if(t_operation=="la"){
        listAllMeetings();
        return true;
    }else if(t_operation=="las"){
        listAllSponsorMeetings();
        return true;
    }else if(t_operation=="lap"){
        listAllParticipateMeetings();
        return true;
    }else if(t_operation=="qm"){
        quitMeeting();
        return true;
    }else if(t_operation=="qt"){
        queryMeetingByTimeInterval();
        return true;
    }else if(t_operation=="dm"){
        deleteMeetingByTitle();
        return true;
    }else if(t_operation=="da"){
        deleteAllMeetings();
        return true;
    }
    return true;
}


void AgendaUI::userLogIn(void){
    string username, password;
    cout<< "[log in] [user name] [password]"<<endl;;
    cout<< "[log in] ";
    cin>>username>>password;
    if(m_agendaService.userLogIn(username,password)){
        m_userName=username;
        m_userPassword=password;
        cout << "[log in] succeed!"<<endl;
        while(executeOperation(getOperation()));
    }else{
        cout<<"[log in] password error or user doesn't exist"<<endl;
    }
}


void AgendaUI::userRegister(void){
    string username,password,email,phone;
    cout<<"[register] [username] [password] [email] [phone]"<<endl;
    cout<<"[register] ";
    cin>>username>>password>>email>>phone;
    if(m_agendaService.userRegister(username,password,email,phone)){
        m_userName=username;
        m_userPassword=password;
        cout << "[register] succeed!"<<endl;
        while(executeOperation(getOperation()));
    }else{
        cout<<"[register] This username has been registered!"<<endl;
    }
}


void AgendaUI::userLogOut(void){
    m_userName.clear();
    m_userPassword.clear();
}


void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
}


void AgendaUI::deleteUser(void){
    m_agendaService.deleteUser(m_userName,m_userPassword);
    userLogOut();
}


void AgendaUI::listAllUsers(void){
    cout<<endl<<"[list all users]"<<endl;
    cout << "name\t" << "email\t" << "phone\n";
    auto users=m_agendaService.listAllUsers();
    for(auto it=users.begin();it!=users.end();it++){
        cout<<it->getName()<<"\t"<<it->getEmail()<<"\t"<<it->getPhone()<<endl;
    }
}


void AgendaUI::createMeeting(void){
    string title, participator, startDate, endDate;
    cout<<"[create meeting] [the number of participators]"<<endl;
    cout<<"[create meeting] ";
    int number;
    cin>>number;
    vector<string>t_participator;
    for(int i=1;i<=number;i++){
        cout<<"[create meeting] [please enter the participator "<<i<<" ]"<<endl;
        cin>>participator;
        t_participator.push_back(participator);
    }
    cout<< "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"<<endl;
    cout<< "[create meeting] ";
    cin>>title>>startDate>>endDate;
    if(m_agendaService.createMeeting(m_userName,title,startDate,endDate,t_participator)){
        cout<<"[create meeting] succeed!"<<endl;
    }else{
        cout<<"[create meeting] error!"<<endl;
    }
}


void AgendaUI::addMeetingParticipator(void){
    cout<<"[add participator] [meeting title] [participator username]"<<endl;
    string title,part_name;
    cout<<"[add participator] ";
    cin>>title>>part_name;
    if(m_agendaService.addMeetingParticipator(m_userName,title,part_name)){
        cout<<"[add participator] succeed!"<<endl;
    }else{
        cout<<"[add participator] error!"<<endl;
    }
}


void  AgendaUI::removeMeetingParticipator(void){
    cout<<"[remove participator] [meeting title] [participator username]"<<endl;
    string title,part_name;
    cout<<"[add participator] ";
    cin>>title>>part_name;
    if(m_agendaService.removeMeetingParticipator(m_userName,title,part_name)){
        cout<<"[remove participator] succeed!"<<endl;
    }else{
        cout<<"[remove participator] error!"<<endl;
    }
}


void  AgendaUI::quitMeeting(void){
    cout<<"[quit meeting] [meeting title]"<<endl;
    cout<<"[quit meeting] "<<endl;
    string title;
    cin>>title;
    if(m_agendaService.quitMeeting(m_userName,title)){
        cout<<"[quit meeting] succeed!"<<endl;
    }else{
        cout<<"[quit meeting] error!"<<endl;
    }
}


void  AgendaUI::listAllMeetings(void){
    cout<<"list all meetings"<<endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}


void  AgendaUI::listAllSponsorMeetings(void){
    cout<<"list all sponsor meetings"<<endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}


void  AgendaUI::listAllParticipateMeetings(void){
    cout<<"list all participate meetings"<<endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}


void  AgendaUI::queryMeetingByTitle(void){
    string title;
    cout<<"[query meeting] [title]:"<<endl;
    cout<<"[query meeting] ";
    cin>>title;
    list<Meeting>t_meetings=m_agendaService.meetingQuery(m_userName,title);
    cout << "title\t" << "sponsor\t" << "start time\t" << "end time\t"<< "participators\n";
    for(auto it=t_meetings.begin();it!=t_meetings.end();it++){
        cout<<it->getTitle()<<"\t"<<it->getSponsor()<<"\t"<<Date::dateToString(it->getStartDate())<<"\t"<<Date::dateToString(it->getEndDate())<<"\t";
        cout<<*(it->getParticipator().begin());
        auto index=it->getParticipator().begin()+1;
        while(index!=it->getParticipator().end()){
            cout<<","<<*index;
        }
        cout<<endl;
    }
}


void  AgendaUI::queryMeetingByTimeInterval(void){
    string start_date, end_date;
    cout<<"[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"<<endl;
    cout<<"[query meetings] ";
    cin>>start_date>>end_date;
    list<Meeting>t_meetings=m_agendaService.meetingQuery(m_userName,start_date,end_date);
    cout << "title\t" << "sponsor\t" << "start time\t" << "end time\t"<< "participators\n";
    for(auto it=t_meetings.begin();it!=t_meetings.end();it++){
        cout<<it->getTitle()<<"\t"<<it->getSponsor()<<"\t"<<Date::dateToString(it->getStartDate())<<"\t"<<Date::dateToString(it->getEndDate())<<"\t";
        cout<<*(it->getParticipator().begin());
        auto index=it->getParticipator().begin()+1;
        while(index!=it->getParticipator().end()){
            cout<<","<<*index;
        }
        cout<<endl;
    }
}


void  AgendaUI::deleteMeetingByTitle(void){
    string title;
    cout<<"[delete meeting] [title]"<<endl;
    cout<<"[delete meeting] ";
    cin>>title;
    if(m_agendaService.deleteMeeting(m_userName,title)){
        cout<<"[delete meeting] succeed!"<<endl;
    }else{
        cout<<"[delete meeting] delete meeting fail!"<<endl;
    }
}


void  AgendaUI::deleteAllMeetings(void){
    if(m_agendaService.deleteAllMeetings(m_userName)){
        cout<<"[delete all meetings] succeed!"<<endl;
    }
}


void  AgendaUI::printMeetings(const std::list<Meeting> &t_meetings){
    cout << "title\t" << "sponsor\t" << "start time\t" << "end time\t"<< "participators\n";
    for(auto it=t_meetings.begin();it!=t_meetings.end();it++){
        cout<<it->getTitle()<<"\t"<<it->getSponsor()<<"\t"<<Date::dateToString(it->getStartDate())<<"\t"<<Date::dateToString(it->getEndDate())<<"\t";
        cout<<*(it->getParticipator().begin());
        auto index=it->getParticipator().begin()+1;
        while(index!=it->getParticipator().end()){
            cout<<","<<*index;
        }
        cout<<endl;
    }
}