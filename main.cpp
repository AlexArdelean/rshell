#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <signal.h>

using namespace std;

//simple Base class
class Base{
    public:
    Base() {};

    virtual bool execute() = 0;
};

//classes that inherit Base
class Operand : public Base{
    string data;

    public:
    string get_data(){
	return data;
    }
};

class Command_Line : public Base{
    public:
    vector<Base*> line;
    
    Command_Line(string shold){
	//create vector of Base* using shold
    	

    }
    
    int size(){
	return line.size();
    }
};

class Commands : public Base{
    public:
    int status;
    vector<Base*> options;
    virtual bool execute() = 0;
};

//classes the inherit Commands
class LS_cmd : public Commands{
    public:
    bool execute(){
	cout << "LS_cmd: ok" << endl;
	return true;
    }
};

class EXIT_cmd : public Commands{
    public:
    bool execute(){
	cout << "EXIT_cmd: ok" << endl;
	return true;
    }
};

class MKDIR_cmd : public Commands{
    public:
    bool execute(){
	cout << "MKDIR_cmd: ok" << endl;
	return true;
    }
};

class CD_cmd : public Commands{
    public:
    bool execute(){
	cout << "CD_cmd: ok" << endl;
	return true;
    }
};

class ECHO_cmd : public Commands{
    public:
    bool execute(){
	cout << "ECHO_cmd: ok" << endl;
	return true;
    }
};

void rm_comments(string &shold){
    if(shold.find("#") != -1)
	shold = shold.substr(0, shold.find());
}

int main(){
    string input;
    bool last_cmd_passed;

    while(true){
	//output propt (include username/location for extra credit)
	cout << "$ ";

	//get arguement(s)
    	getline(cin, input);

	//remove comments
	rm_comments(input);

	//use input to construct a Command_Line
	Command_Line cmd_ln(input);

	for(int i = 0; i < cmd_ln.size(); i++){
	    if(cmd_ln.at(i)->status == 0)
		last_cmd_passed= cmd_ln.at(i)->execute();

	    else if(cmd_ln.at(i)->status == 1)
		if(last_cmd_passed)
		    last_cmd_passed= cmd_ln.at(i)->execute();

	    else
		if(!last_cmd_passed)
		    last_cmd_passed= cmd_ln.at(i)->execute();
	}
    }
 
    return 0;
}
