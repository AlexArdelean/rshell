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
    ~Base() {};

    virtual bool execute() = 0;
};

//classes that inherit Base
class Commands : public Base{
    public:
    char *cmd[256];

    Commands(char *line, int s){
	status = s;
	int i = 0;
	
	//splits up text even further into a format usable by execvp
	char *chold = strtok(line, " ");
	while(chold != NULL){
	    cmd[i] = chold;
	    chold = strtok(NULL, " ");
	    i++;
	}
	cmd[i] = '\0';
    }

    ~Commands(){
	for(int i = 0; i < 256; i++)
	    delete cmd[i];
    }

    bool execute(){
	//exit called
	if(strcmp(*cmd, "exit") == 0)
	    exit(0);

	pid_t pid = fork();
	int ihold;
	
	//if forking failed
	if(pid == -1){
	    perror("Forking Failure");
	    exit(EXIT_FAILURE);
	}
	
	//if child
	else if(pid == 0){
	    if(execvp(*cmd, cmd) < 0){
		perror("Execvp Failure");
		exit(EXIT_FAILURE);
	    }
	}
	
	//if parent
	else
	   pid = wait(&ihold);
	
	//if the cmd worked (ie. execvp executed properly)
	if(ihold == 0)
	    return true;

	//if cmd failed (eg. <mkdir test> when the directory, test, already exists)
	return false;
    }
};

class Command_Line : public Base{
    public:
    vector<Base*> line;
    
    void get_vinfo(vector<int> &vstatus, vector<int> &vpos, char *chold){
	//breaks text into segments by setting ;/&&/|| into '\0' chars and saving status/pos info
	int quoted = 1; //1 for non quotes -1 for quotes
	for(int i = 0; chold[i] != '\0'; i++){
	    if(chold[i] == '\"')
		quoted *= -1;

	    if(quoted == 1){
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
    }

    Command_Line(char *input){
	//cmds status'/position vectors (also count of how many cmds)
	vector<int> vstatus;
	vector<int> vpos;
	get_vinfo(vstatus, vpos, input);
	
	//creates vector of commands to execute for the line
	line.push_back(new Commands(input, 0));
	for(unsigned int i = 0; i < vstatus.size(); i++)
	    line.push_back(new Commands(input + vpos[i], vstatus[i]));
    }

    ~Command_Line(){
	while (line.size() > 0){
	    delete line[line.size() - 1];
	    line.pop_back();
	}
    }
    
    bool execute(){
	bool last_cmd_passed;
	
	//execute vector of commands one by one
	for(unsigned int i = 0; i < line.size(); i++){
	    //if the first command or first after a semicolon
	    if(line[i]->status == 0)
		last_cmd_passed= line[i]->execute();
	    
	    //if after &&
	    else if(line[i]->status == 1){
		if(last_cmd_passed)
		    last_cmd_passed= line[i]->execute();
		else
		    last_cmd_passed= false;
	    }
	    
	    //if after ||
	    else if(line[i]->status == 2){
		if(!last_cmd_passed)
		    last_cmd_passed= line[i]->execute();
		else
		    last_cmd_passed= false;
	    }
	}
	return true;
    }
};

//removes comments from input string
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
