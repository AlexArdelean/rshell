#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//simple Base class
class Base{
    public:
    int status;

    Base() {};

    virtual bool execute() = 0;
};

//classes that inherit Base
class Commands : public Base{
    public:
    char *cmd[256];

    Commands(char *line, int s){
	status = s;
	int i = 0;

	char *chold = strtok(line, " ");
	while(chold != NULL){
	    cmd[i] = chold;
	    chold = strtok(NULL, " ");
	    i++;
	}

	cmd[i] = '\0';
    }

    bool execute(){
	if(cmd[0] == "exit")
	    exit(EXIT_SUCCESS);

	pid_t pid = fork();
	int ihold;

	if(pid == -1){
	    cout << "Forking Error!\n";
	    exit(EXIT_FAILURE);
	}

	else if(pid == 0){
	    if(execvp(*cmd, cmd) < 0){
		cout << "Execution Error!\n";
		exit(EXIT_FAILURE);
	    }
	}

	else
	   pid = wait(&ihold);

	return true;
    }
};

class Command_Line : public Base{
    public:
    vector<Base*> line;
    
    void get_vinfo(vector<int> &vstatus, vector<int> &vpos, char *chold){
	for(int i = 0; i < strlen(chold); i++){
	    if(chold[i] == ';'){
		vstatus.push_back(0);
		vpos.push_back(i+1);
		chold[i] = '\0';
	    }

	    else if(chold[i] == '&' && chold[i+1] == '&'){
		vstatus.push_back(1);
		vpos.push_back(i+2);
		chold[i] = '\0';
	    }

	    else if(chold[i] == '|' && chold[i+1] == '|'){
		vstatus.push_back(2);
		vpos.push_back(i+2);
		chold[i] = '\0';
	    }
	}
    }

    Command_Line(char *input){
	vector<int> vstatus;
	vector<int> vpos;
	get_vinfo(vstatus, vpos, input);

	line.push_back(new Commands(input, 0));
	for(int i = 0; i < vstatus.size(); i++)
	    line.push_back(new Commands(input + vpos[i], vstatus[i]));
    }
    
    bool execute(){
	bool last_cmd_passed;

	for(int i = 0; i < line.size(); i++){
	    if(line[i]->status == 0)
		last_cmd_passed= line[i]->execute();

	    else if(line[i]->status == 1)
		if(last_cmd_passed)
		    last_cmd_passed= line[i]->execute();

	    else if(line[i]->status == 2)
		if(!last_cmd_passed)
		    last_cmd_passed= line[i]->execute();
	
	    else
		return false;
	}

	return true;
    }
};

void rm_comments(char *input){
    for(int i = 0; input[i] != '\0'; i++)
	if(input[i] == '#')
	    input[i] = '\0';
}

int main(){
    bool run = true;

    while(run){
	//output propt (include username/location for extra credit)
	cout << "$ ";

	//get arguement(s)
    	char input[1024];
	cin.getline(input, 1000, '\n');
	
	//remove comments
	rm_comments(input);
	    
	//use input to construct a Command_Line
    	Command_Line cmd_ln(input);
	
	//execute commands
	run = cmd_ln.execute();
    }
 
    return 0;
}
