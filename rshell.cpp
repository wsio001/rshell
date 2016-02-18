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

	if(command.find('#') != string::npos)
	{
		command.erase(command.find('#'),(command.size() - command.find('#')));//delete everything after # since anything after # is comment
	}
	
	con_vec(command, connector_v);
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

	int status;
	int j = 0;//to keep track of the command_v
	vector<char*> temp_v;
	bool first_command = true;
	for(int i = 0; j < command_v.size()||i<connector_v.size(); ++i)
	{
		if(first_command == true)
		{	
			if (connector_v.at(i) == 0)
			{
				temp_v.push_back(command_v.at(j));
				j++;
				temp_v.push_back(command_v.at(j));
				j++;
				temp_v.push_back(NULL);
				char** c_temp = &temp_v[0];
				pid_t pid = fork();
				if (pid == 0)
				{
					//child;
		
					if (execvp(c_temp[0], c_temp) == -1)
					{
						status = -1;
						perror("exec");
					}
				}
				if (pid > 0)
				{
					//paretn;
					if(wait(0) == -1)
					{			
						perror("wait");
					}		
				}		
				temp_v.clear();
			}
			else
			{
				temp_v.push_back(command_v.at(j));
				j++;
				temp_v.push_back(NULL);
				char** a_temp = &temp_v[0];
				pid_t pid = fork();
				if (pid == 0)
				{
					//child;
		
					if (execvp(a_temp[0], a_temp) == -1)
					{
					status = -1;
					perror("exec");
					}
				}
				if (pid > 0)
				{
					//paretn;
					if(wait(0) == -1)
					{			
						perror("wait");
					}		
				}
				temp_v.clear();
				i--;
			}
			first_command = false;
		}
		else if (!first_command)	
		{
			if (connector_v.at(i) == 1)
			{
				if(status == -1)
				{
					if (connector_v.at(i + 1) == 0)
					{
						temp_v.push_back(command_v.at(j));
						j++;
						temp_v.push_back(command_v.at(j));
						j++;
						temp_v.push_back(NULL);
						char** c_temp = &temp_v[0];
						pid_t pid = fork();
						if (pid == 0)
						{
							//child;
				
							if (execvp(c_temp[0], c_temp) == -1)
							{
								status = -1;
								perror("exec");
							}
						}
						if (pid > 0)
						{
							//paretn;
							if(wait(0) == -1)
							{			
								perror("wait");
							}		
						}		
						temp_v.clear();
					}
					else
					{
						temp_v.push_back(command_v.at(j));
						j++;
						temp_v.push_back(NULL);
						char** a_temp = &temp_v[0];
						pid_t pid = fork();
						if (pid == 0)
						{
							//child;
							if (execvp(a_temp[0], a_temp) == -1)
							{
								status = -1;
								perror("exec");
							}
						}
						if (pid > 0)
						{
							//paretn;
							if(wait(0) == -1)
							{			
								perror("wait");
							}		
						}
						temp_v.clear();	
					}
				}
				else
				{
					j++;
					j++;
				}
			}
		/*	else if (connector_v.at(i) == 3)
			{
				if(status != -1)
				{
					temp_v.push_back(command_v.at(j));
					j++;
					temp_v.push_back(NULL);
					char** a_temp = &temp_v[0];
					pid_t pid = fork();
					if (pid == 0)
					{
						//child;
						if (execvp(a_temp[0], a_temp) == -1)
						{
							status = -1;
							perror("exec");
						}
					}
					if (pid > 0)
					{
						//paretn;
						if(wait(0) == -1)
						{			
							perror("wait");
						}		
					}
				temp_v.clear();	
				}
				else
				{
					j++;
				}
			}
			else if (connector_v.at(i) == 2)
			{
				{
					temp_v.push_back(command_v.at(j));
					j++;
					temp_v.push_back(NULL);
					char** a_temp = &temp_v[0];
					pid_t pid = fork();
					if (pid == 0)
					{
						//child;
						if (execvp(a_temp[0], a_temp) == -1)
						{
							status = -1;
							perror("exec");
						}
					}
					if (pid > 0)
					{
						//paretn;
						if(wait(0) == -1)
						{			
							perror("wait");
						}		
					}
					temp_v.clear();	
				}
			}
		*/}
	}		
	return 0;
}	
	
