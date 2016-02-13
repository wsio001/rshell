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
int main()
{
	
	char* name;                    	//get username
	struct passwd* pass;
	pass = getpwuid(getuid());
	name = pass->pw_name;
	printf("%s$ ", name);
	////////////////////////////////////// username ///////////////////////////////////////

	string command;
	getline(cin,command);
/*	for (int i = 0; i < command.size(); ++i)
	{	
		com_ar[i] = command.at(i);
	}
	com_ar[command.size()] = NULL;
        
	char* command_str;
	vector <char*> command_vec;
	command_str = strtok(com_ar,"|#&;");
	while(command_str != NULL)
	{
		command_vec.push_back(command_str);
		command_str = strtok(NULL,"|#&;");
	}
	command_vec.push_back(NULL);
	char** com = &command_vec[0];
	cout << com[0][1] << endl;	
*/	vector <string> string_vec;
	vector <char*> char_vec
	char** tempcom;					//temporary vecter for execvp
	int j = 0;					//counter temporary array for execvp
	int status = 0;					//status for the previous command (fail or success)
	char* comsub;

	for (int i = 0; command.at(i) != '\0' ; ++i)
	{	
		if (i == command.size() - 1)
		{
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("forkfail");
			}
			if (pid == 0)
			{   
		 
				string tempstring;
				strcpy(tempstring, command.substr(j, i - j + 1).c_str());
				string_vec.push(tempstring);
				tempcom_vec.push_back(NULL);
				tempcom = &string_vec[0];
				status = execvp(tempcom[0],tempcom);
				if(status== -1 )
				{
					perror("execvperror");
				}
				string__vec.clear();
				j = i + 1;
				exit(0);
			}

			if (pid > 0)	
			{	
				if (wait(0) == -1)
				{
					perror("waitfail");
				}
				exit(0);
			}	
			
			return 0;
		}

		if (command.at(i) == ' ')
		{	
			string tempstring;
			strcpy(tempstring, command.substr(j, i - j).c_str());
			string_vec.push_back(tempstring);
			j =  i + 1;
		}

/*		if (command.at(i) == ';')
		{
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("forkfail");
			}
			if (pid == 0)
			{
				comsub=(char*)((command.substr(j, i - j)).c_str());		 
				tempcom_vec.push_back(comsub);
				tempcom_vec.push_back(NULL);
				tempcom = &tempcom_vec[0];
				status = execvp(tempcom[0],tempcom);
				if(status== -1 )
				{
					perror("execvperror");
				}
				tempcom_vec.clear();
				j = i + 1;
				exit(0); 
			}

			if (pid > 0)	
			{	
				if (wait(0) == -1)
				{
					perror("waitfail");
				}
				exit(0);
			}
			 
		}


		else if (command.at(i) == '|' && command.at(i + 1 )== '|') 
		{
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("forkfail");
			}
			if (pid == 0)
			{
				if(status = -1)
				{
					comsub=(char*)((command.substr(j, i - j)).c_str()); 
					tempcom_vec.push_back(comsub);
					tempcom = &tempcom_vec[0];
					status = execvp(tempcom[0],tempcom);
					if(status== -1 )
					{
						perror("execvperror");
					}
					tempcom_vec.clear();
					i = i + 1;
					j = i + 1;
					exit(0);
				} 
				else
				{
					return 0;
				}
			}

			if (pid > 0)	
			{	
				if (wait(0) == -1)
				{
					perror("waitfail");
				}
				exit(0);
			}	
		}

		else if (command.at(i) == '&' && command.at(i + 1) == '&')
		{
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("forkfail");
			}
			if (pid == 0)
			{
				if (status != -1)
				{
					comsub=(char*)((command.substr(j, i - j)).c_str()); 
					tempcom_vec.push_back(comsub);
					tempcom = &tempcom_vec[0];
					status = execvp(tempcom[0],tempcom);
					if(status == -1 )
					{
						perror("execvperror");
					}
					tempcom_vec.clear();
					i = i + 1;
					j = i + 1;
					exit(0); 
				}
				else
				{
					return 0;
				}
			}

			if (pid > 0)	
			{	
				if (wait(0) == -1)
				{
					perror("waitfail");
				}
				exit(0);
			}
		}
		
		else if (command.at(i) == '#')
		{
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("forkfail");
			}
			if (pid == 0)
			{
				comsub =(char*)(( command.substr(j, i - j)).c_str()); 
				tempcom_vec.push_back(comsub);
				tempcom_vec.push_back(NULL);
				tempcom = &tempcom_vec[0];
				status = execvp(tempcom[0],tempcom);
				if(status == -1 )
				{
					perror("execvperror");
				}
				tempcom_vec.clear();
				return 0; 
			}

			if (pid > 0)	
			{	
				if (wait(0) == -1)
				{
					perror("waitfail");
				}
				exit(0);
			}	
		}
*/
	}			
	return 0;

}
