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


void (string command, vector <int> &empty_v)
{
//index:
//0 means ||
//1 means &&
//2 means ;
//3 means #
//4 means SPACE
//-1 means error
	for (int i = 0; i < command.size(); ++i)
	{
		if (i == command.size() - 1)
		{	
			if (command.at(i) == ';' ||  command.at(i) == '|' || command.at(i) == '&' || command.at(i) == ' ')
			{
				empty_v.push_back(-1); // -1 means error;
				return//since it is at the end, we do not want to keep checking, so return
			}
		}

			
		if (command.at(i) == '|' )
		{
			if( command.at(i + 1) =! '|')
			{
				empty_v.push_back(-1);
			}
			else if (command.at(i + 1) == '|')
			{
				empty_v.push_back(0);// 0 means ||
				i++;// since we know it is two |s next to each other, we can just skip the next character	
			}
		}
		
		else if (command.at(i) == '&')
		{	
			if (command.at(i + 1) =! '&')
			{
				empty_v.push_back(-1);
			}
			else if ( command.at(i + 1) == '&')
			{
				empty_v.push_back(1);// 1 means &&
				i++; //since it is two &s next to each other, I can just skip the next one.
			}	
		}
		
		else if (command.at(i) == ';')
		{
			empty_v.push_back(2); //2 means semi colon
			if (command.at(i + 1) == ';')
			{
				empty_v.push_back(-1);
				i++;//since we check the next one, we can just skip the next one);
			}
		}
		
		else if (command.at(i) == '#')
		{
			empty_v.push_back(3);
			return //we know that # means comment, so we do not read anytning after #, so return
		}
		
		else if (command.at(i) == ' ')
		{
			if(command.at(i + 1) == '#' || command.at(i + 1) == ';' ||  command.at(i + 1) == '|' || command.at(i + 1) == '&' || command.at(i + 1) == ' ')
			{
				empty_v.push_back(-1);
				i++;
			}
			else
			{
				empty_v.push_back(4); //4 means SPACE;
			}
		}
	}	
	return;
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
	vector <int> connector;
	con_vec(command, connector);
	
	for (int i = 0; i < connector.size(); i++) // test to see if the con_vec funtion is working
	{
		cout << connector.at(i) << " ";
	}
	cout << endl;
	
	return 0;
}	
	
