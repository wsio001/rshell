#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <ios>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <vector>
using namespace std;

void con_vec (string command, vector <int> &empty_v)
{
//index:
//1 means ||
//3 means &&
//2 means ;
//0 means SPACE
//-1 means fail

	int counter = 0;
	
	if (command.at(0) == ';' || command.at(0) == '|' || command.at(0) == '&')
	{
		empty_v.push_back(-1);
		return;
	}
	for (int i = 0; i < command.size(); ++i)
	{
		if (i == command.size() - 1)
		{	if (command.at(i) == ' ')
			{
				return;
			}
			if (command.at(i) == ';' ||  command.at(i) == '|' || command.at(i) == '&')
			{
				empty_v.push_back(-1); // -1 means error;
				return;//since it is at the end, we do not want to keep checking, so return
			}
		}

			
		if (command.at(i) == '|')
		{
			if(counter == 1)//counter init is 0, so it will go to the else case, counter means which '|' we are in, 0 means first one, 1 means second one.
			{
				if(command.at(i + 1) == '#' || command.at(i + 1) == ';' ||  command.at(i + 1) == '&')
				{
					empty_v.push_back(-1);
					return;
				}	
				else
				{
					empty_v.push_back(1); //1means '|'
					counter--;
				}
			}		

			else
			{
				if (command.at(i + 1) != '|')
				{
					empty_v.push_back(-1);
					return;
				}
				counter++;
			}
		}
		
		else if (command.at(i) == '&')
		{	
			if(counter == 1)//counter init is 0, so it will go to the else case, counter means which '&' we are in, 0 means first one, 1 means second one.
			{
				if(command.at(i + 1) == '#' || command.at(i + 1) == ';' ||  command.at(i + 1) == '|')
				{
					empty_v.push_back(-1);
					return;
				}	
				else
				{
					empty_v.push_back(3); //3means '&'
					counter--;
				}
			}		

			else
			{
				if (command.at(i + 1) != '&')
				{
					empty_v.push_back(-1);
					return;
				}
				counter++;
			}	
			
		}
		else if (command.at(i) == ';')
		{
			empty_v.push_back(2); //2 means semi colon
			if (command.at(i + 1) == ';')
			{
				empty_v.push_back(-1);
				return;
			}
		}
		
		else if (command.at(i) == ' ')
		{
			if((command.at(i + 1) != '#' && command.at(i + 1) != ';' &&  command.at(i + 1) != '|' && command.at(i + 1) != '&')&&(command.at(i - 1) != '#' && command.at(i - 1) != ';' &&  command.at(i - 1) != '|' && command.at(i - 1) != '&'))

			{
				empty_v.push_back(0); //0 means SPACE;
			}
		}
	}	
	return ;
}

int run ( const char* file, char* const args[])
{
	pid_t pid = fork();
	if(pid == 0)
	{
		if(execvp(file, args) == -1)
		{
			perror("exec");
			return -1;
		}
	}
	if (pid > 0)
	{
		if (wait(0) == -1)
		{
			perror("wait");
		}
	}
	return 1;
} 
		
int main()
{

/////////////////////////////////////// get usernmae and hostname /////////////////////////////////////////	
	char* name;                    	
	struct passwd* pass;
	pass = getpwuid(getuid());
	name = pass->pw_name;
	printf("%s@", name);
	
	char host[1024];
	host[1023] = '\0';
	gethostname(host,1023);
	printf("%s$ ", host);

////////////////////////////////////// making the connector array///////////////////////////////////////
	string command;
	getline(cin,command);	
	vector <int> connector_v;
	if(command.empty())
	{
		return 0;
	}

	if(command.find('#') != string::npos)
	{
		command.erase(command.find('#'),(command.size() - command.find('#')));//delete everything after # since anything after # is comment
	}
		
	int counter = 0;//to see if there is any -1 in the connector vector, if there is, give out a error message.
	for (int i = 0; i < connector_v.size(); i++) // test to see if the con_vec funtion is working
	{
		if(connector_v.at(i) == -1)
		{
			counter = -1;
		}
		cout << connector_v.at(i) << " ";
	}
	cout << endl;
	
	if(counter == -1)//check if there is any -1, if there is, terminate
	{
		cout << "your command has some error." << endl;
		return 0;
	}
//////////////////////////////////stringtokenize the command////////////////////////////////////////////////////	
	char* temp;
/*	for (int i = 0; i < command.size(); i++)
	{
		temp[i] = command.at(i);
	}*/
	
	temp = new char[command.size()];
	strcpy(temp, command.c_str());
	
	vector <char*> command_v;
	char* pieces;
	pieces = strtok(temp, " |&;");
	while (pieces != NULL)
	{
		command_v.push_back(pieces);
		pieces = strtok(NULL, " |&;");
	}
	
	for (int i = 0; i < command_v.size(); i++)// cout command_v
	{
		cout << command_v.at(i) << endl;
	}
	
	if (connector_v.empty())// if it is single command, then just push back and run it and return.
	{
		vector <char*> noconnect_v;
		noconnect_v.push_back(command_v.at(0));
		noconnect_v.push_back(NULL);
		char** noconnect;
		noconnect = &noconnect_v[0];
		run(noconnect[0], noconnect);
		return 0;
	}
		

	int status = 0;
	bool firstcommand = true;
	// j is to keep track of the command_v
	vector<char*> temp_v;
	int j = 0;
	temp_v.push_back(command_v.at(j));//push back the first command anyways
	j++;//now J is 1
	char** temp_com;
	for(int i = 0; (i < connector_v.size() && j < command_v.size()); i++)
	{
		
		if(connector_v.at(i) == 0)
		{
			temp_v.push_back(command_v.at(j));//if it is space, add the next command as the parameter
			j++;
		}
		else if (connector_v.at(i) == 1)
		{
			if(firstcommand)
			{
				temp_v.push_back(NULL);
				temp_com = &temp_v[0];//if it is the first command
				cout << temp_v.at(i) << endl << "////////////" << endl;
				status = run(temp_com[0], temp_com);//run the command that is in the temp_v already, and check the status
				firstcommand = false;
			}
			if(status == -1)
			{
				temp_v.clear();
				temp_v.push_back(command_v.at(j));
				temp_v.push_back(NULL);
				j++;
				temp_com = &temp_v[0];
				status = run(temp_com[0], temp_com);//if status is -1, run the next command, and set the status
			}
			else if (status != -1)
			{
				j++;
			}	
		}
		else if (connector_v.at(i) == 2)
		{
			if(firstcommand)
			{
				temp_v.push_back(NULL);
				temp_com = &temp_v[0];//if it is the first command 
				status = run(temp_com[0],temp_com);//run the commmand that is in the temp_v already, no need to check status
				firstcommand = false;
			}
			temp_v.clear();//clear the temp_v;
			temp_v.push_back(command_v.at(j));
			temp_v.push_back(NULL);
			j++;
			temp_com = &temp_v[0];
			status = run(temp_com[0], temp_com);//run the next command, and set the status
		}

		else if (connector_v.at(i) == 3)
		{
			if(firstcommand)
			{
				temp_v.push_back(NULL);
				temp_com = &temp_v[0];
				status = run (temp_com[0], temp_com);	
				firstcommand = false;
			}
			if(status != -1)
			{
				temp_v.clear();
				temp_v.push_back(command_v.at(j));
				temp_v.push_back(NULL);
				j++;
				temp_com = &temp_v[0];
				status = run(temp_com[0], temp_com); // run the next command, and set the status
			}
			else if(status == -1)
			{
				j++;
			}
		}
		return 0;
	}
	temp_v.push_back(NULL);
	temp_com = &temp_v[0];
	status = run(temp_com[0], temp_com);
	return 0;
}	
	
