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
vector <int> con_vec (string command, vector <int> empty_v)
{
	for (int i = 0; i < command.size(); ++i)
	{
		if (i == command.size() - 1)
		{	
			if ( command.at(i) == '|' ||command.at(i) == '#' ||command.at(i) == '&' ||command.at(i) == ';' ||command.at(i) == ' ')
			
		if (command.at(i) == '|' && command.at(i + 1) == '|')
	
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
	
	
	return 0;
}	
	
