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

	unsigned counter = 0;
	
	if (command.at(0) == ';' || command.at(0) == '|' || command.at(0) == '&')
	{
		empty_v.push_back(-1);
		return;
	}
	for (unsigned i = 0; i < command.size(); ++i)
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


	 

void getname()// this function is to get loginname and host name;
{
	char* name;                    	
	struct passwd* pass;
	pass = getpwuid(getuid());
	name = pass->pw_name;
	printf("%s@", name);
	
	char host[1024];
	host[1023] = '\0';
	gethostname(host,1023);
	printf("%s$ ", host);
	return;
}
	
void tokenizer(string command, vector<char*> &command_v)//this function push command_v with all the commands and parameter next to each other
{	
	char* temp;
	/*for (int i = 0; i < command.size(); i++)
	{
		temp[i] = command.at(i);
	}*/
	
	temp = new char[command.size()];
	strcpy(temp, command.c_str());
	
	char* pieces;
	pieces = strtok(temp, " |&;");
	while (pieces != NULL)
	{
		command_v.push_back(pieces);
		pieces = strtok(NULL, " |&;");
	}
	return;
		
}
int main()
{
	while (true)
	//check if the command is exit, empty command, error command, single command, multiply command, or exit.
	{	
		string command;
		getname();
		getline(cin, command);
		if (command == "exit")
		{
			cout << "Bye" << endl;
			return 0;
			
		}	

		else if (!command.empty()) //check to see if it is a empty command, if it is not, continue.
		{	
			vector <int> connector_v; //vector to store connectors representation number
			int counter = 0; //to see if there is any -1 in the connector vector, if there is, give out a error message.
			vector <char*> command_v; //vector to store the commands next to each other individually
			
			con_vec(command, connector_v);

			for (unsigned i = 0; i < connector_v.size(); i++) 
			{
				if (connector_v.at(i) == -1)
				{
					counter = -1;
				}
				//cout << connector_v.at(i) << " ";
			}

			if (counter != -1) //see if it is error command, if not, continue. now the command must be either single or multiply
			{
				if (command.find('#') != string::npos) //trim everything after #, including #
				{
					command.erase(command.find('#'), (command.size() - command.find('#')));
				}
		
				tokenizer(command,command_v); //call tokenizer to break the string apart.

				if(connector_v.empty()) //check to see if it is a single command, if yes, run it and return back
				{
					vector <char*> noconnect_v;
					noconnect_v.push_back(command_v.at(0));
					noconnect_v.push_back(NULL);
					char** noconnect;
					noconnect = &noconnect_v[0];
					pid_t pid = fork();
					if(pid == 0)
					{
						if(execvp(noconnect[0], noconnect) == -1)
						{
							perror("exec");
						}
						exit(0);
					}
					if (pid > 0)
					{
						if (wait(0) == -1)
						{
							perror("wait");
						}
					}
				}
				else //now this is multiply command
				{
					int status = 0;
					bool firstcommand = true;
					vector<char*> temp_v;
					unsigned j = 0; // it is to keep track of command_v
					temp_v.push_back(command_v.at(j)); //push back the first command anyways
					j++;
					char** temp_com;
					for (unsigned i = 0; ((i < connector_v.size()) && (j < command_v.size())); i++)
					{		
						if (connector_v.at(i) == 0)
						{
							temp_v.push_back(command_v.at(j)); //if it is space, add the next command as the parameter
							j++;
						}
						else if (connector_v.at(i) == 1)
						{
							if (firstcommand)
							{
								temp_v.push_back(NULL);
								temp_com = &temp_v[0];
								pid_t pid = fork();
								if(pid == 0)
								{
									if(execvp(temp_com[0], temp_com) == -1)
									{
										status = -1;
										perror("exec");
									}
									exit(0);
								}
								if (pid > 0)
								{
									if (wait(0) == -1)
									{
										perror("wait");
									}
								}
	
								firstcommand = false;
								temp_v.clear();
							}

							if (status == -1)
							{
								if (connector_v.at(i + 1) == 0)
								{	
									temp_v.push_back(command_v.at(j)); //if it is space after, add the next command as the parameter
									j++;
									firstcommand = true;
								}
								else
								{
									temp_v.clear();
									temp_v.push_back(command_v.at(j));
									temp_v.push_back(NULL);
									j++;
									temp_com = &temp_v[0];
									pid_t pid = fork();
									if(pid == 0)
									{
										if(execvp(temp_com[0], temp_com) == -1)
										{
											status = -1;
											perror("exec");
										}
										exit(0);
									}
									if (pid > 0)
									{
										if (wait(0) == -1)
										{
											perror("wait");
										}
									} // run the next command, and set the status
									temp_v.clear();
								}
							}
							else if (status != -1)
							{	
								if (i != connector_v.size() - 1)
								{
									if (connector_v.at(i + 1) == 0)
									{
										unsigned f = i + 1;
										unsigned count = 0;
										while (connector_v.at(f) == 0)
										{	
											++f;
											++count;
										}
										i = i + count;
										j = j + count + 1;
													
									}
								}
								else
								{
									j++;
								}
							}
						}
						else if (connector_v.at(i) == 2)
						{
							if (firstcommand)
							{
								temp_v.push_back(NULL);
								temp_com = &temp_v[0]; //if it is the first command 
 								pid_t pid = fork();
								if(pid == 0)
								{
									if(execvp(temp_com[0], temp_com) == -1)
									{
										status = -1;
										perror("exec");
									}
									exit(0);
								}
								if (pid > 0)
								{
									if (wait(0) == -1)
									{
										perror("wait");
									}
								}//run commmand in the temp_v, no need to check status
								firstcommand = false;
								temp_v.clear();
							}
							
							if (i != connector_v.size() - 1)
							{
								if (connector_v.at(i + 1) == 0)
								{
									temp_v.push_back(command_v.at(j)); //if it is space after, add the next command as the parameter
									j++;
									firstcommand = true;
								}
							}
							else
							{
								temp_v.clear();
								temp_v.push_back(command_v.at(j));
								temp_v.push_back(NULL);
								j++;
								temp_com = &temp_v[0];
								pid_t pid = fork();
								if(pid == 0)
								{
									if(execvp(temp_com[0], temp_com) == -1)
									{
										status = -1;
										perror("exec");
									}

									exit(0);
								}
								if (pid > 0)
								{
									if (wait(0) == -1)
									{
										perror("wait");
									}
								} //run the next command, and set the status
								temp_v.clear();			
							}
						}
						else if (connector_v.at(i) == 3)
						{
							if (firstcommand)
							{
								temp_v.push_back(NULL);
								temp_com = &temp_v[0];
								
								pid_t pid = fork();
								if(pid == 0)
								{
									if(execvp(temp_com[0], temp_com) == -1)
									{
										status = -1;
										perror("exec");
									}

									exit(0);
								}
								if (pid > 0)
								{
									if (wait(0) == -1)
									{
										perror("wait");
									}
								}
								firstcommand = false;
								temp_v.clear();
							}

							if (status != -1)
							{
								if (i != connector_v.size() - 1)
								{
									if (connector_v.at(i + 1) == 0)
									{
										temp_v.push_back(command_v.at(j)); //if it is space after, add the next command as the parameter
										j++;
										firstcommand = true;
									}
								}
								else
								{
									temp_v.clear();
									temp_v.push_back(command_v.at(j));
									temp_v.push_back(NULL);
									j++;
									temp_com = &temp_v[0];
									pid_t pid = fork();
									if(pid == 0)
									{
										if(execvp(temp_com[0], temp_com) == -1)
										{
											status = -1;
											perror("exec");
										}
										exit(0);
									}
									if (pid > 0)
									{
										if (wait(0) == -1)
										{
											perror("wait");
										}
									} // run the next command, and set the status
									temp_v.clear();
								}
							}
							else if (status == -1)
							{	
								if (i != connector_v.size() - 1)
								{
									if (connector_v.at(i + 1) == 0)
									{
										unsigned f = i + 1;
										unsigned count = 0;
										while (connector_v.at(f) == 0)
										{	
											++f;
											++count;
										}
										i = i + count;
										j = j + count + 1;
									}
								}
								else
								{
									j++;
								}
							}
						}
					}

					if(!temp_v.empty())
					{
						temp_v.push_back(NULL);//if thiere isnt any other connecter beside space, just run all of them.
						temp_com = &temp_v[0];
						pid_t pid = fork();
						if(pid == 0)
						{
							if(execvp(temp_com[0], temp_com) == -1)
							{
								perror("exec");
							}
							exit(0);
						}
						if (pid > 0)
						{	
							if (wait(0) == -1)
							{
								perror("wait");
							}
						}
						temp_v.clear();
					}
				}
			}
			else
			{
				cout << "Cannot run the command becasue it has error in it, re-enter your command" << endl;
			}
		}
		else
		{
			cout << "Error, empty command, re_enter your command" << endl;
		}
	}

	return 0;
}	
	
