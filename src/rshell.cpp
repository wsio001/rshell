#include <iostream>
#include <string>
#include <string.h>
#include <typeinfo>
#include <boost/tokenizer.hpp>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <stack>
#include <sys/stat.h>
#include <pwd.h>

using namespace std;
using namespace boost;


void no_end(string &line, bool &e, int &count) // take care of the paranthesis
{
    for(unsigned i = 0; i < line.size(); ++i)
    {
        if(line.at(i) == ')')
        {
            --count;
        }
        if(line.at(i) == '(')
        {
            ++count;
        }
        if((line.at(i) == ')') && (count == 0))
        {
            line.erase(line.begin()+i);                 
            e = true;
        }
    }
}

void Build(vector<string> &cmd_parent, int &size, int &s) // make the parenthesis command
{
    string temp = cmd_parent.at(0);             
    temp.erase(temp.begin());
    cmd_parent.at(0) = temp;  
    
    vector<string> new_p;
    bool end = false;
    int count = 1;
    for(int i = 0; i < size; ++i)
    {
        if(end == false)
        {
            string fend = cmd_parent.at(i);
            no_end(fend, end, count);
            new_p.push_back(fend);
            s = i + 1;
        }
        else
        {
            string find_finish = cmd_parent.at(i);
            new_p.push_back(find_finish);
        }
    }
    cmd_parent = new_p;
    return;
}
    
int checkFlag(vector<string> &cmd)
{                     
    int counter = 0;
    if(cmd.at(0) == "-e"){
        counter = 1;
        cmd.erase(cmd.begin());
    }
    else if(cmd.at(0) == "-f")
    {
        counter = 2;
        cmd.erase(cmd.begin());
    }
    else if(cmd.at(0) == "-d")
    {
        counter = 3;
        cmd.erase(cmd.begin());
    }
    return counter;
}

int test(vector<string> ParsedString, unsigned index, bool c, int con, int p_count, bool parent_point, int parent_counter_p, bool in_par){
if(in_par == true){                                         //checks paranthesis
    
    if((parent_point == false) && (con != 2) && (parent_counter_p != 2))  //Case
    {
        return 1;   
    }
    if((parent_point == true) && (parent_counter_p == 2)){                //Case
        return 1;
    }
}
   if((c == false) && (p_count != 2)){                        //Case
        return 1;             
    }
    if((c == true) && (p_count == 2)){                        //Case
      return 0;
    }
    int flag = checkFlag(ParsedString);                     //get flag
    unsigned end = ParsedString.size();
    if(index > 0){
       end = index;
    }
    // check what's in vector
    for(unsigned i = 0; i < end; ++i){
        //cout << ParsedString.at(i) << ' ';
    }

    char *path;
    path = strdup((ParsedString.at(0)).c_str());
    struct stat s;
    if(flag == 0 || flag == 1){
        if(stat(path, &s) == 0){                      
            cout << "(TRUE)" << endl;
            return 0;
        }
        else{
            cout << "(FALSE)" << endl;
            return 1;
        }
    }
    else if(flag == 2){
        if((stat(path, &s) == 0) && (s.st_mode & S_IFREG)){     //file               
            cout << "(TRUE)" << endl;
            return 0;
        }
        else{
            cout << "(FALSE)" << endl;
            return 1;
        }
    }
    else if(flag == 3){
        if((stat(path, &s) == 0) && (s.st_mode & S_IFDIR)){    //directory
            cout << "(TRUE)" << endl;
            return 0;
        }
        else{
            cout << "(FALSE)" << endl;
            return 1;
        } 
    }
    else{
        cout << "Error" << endl;
        exit(-1);
    }
}

int exec(vector<string> cmd, bool ok, int counter, int count_p, bool paren_point, int paren_count, bool num_par, int size)
{ 
    char** ary = new char*[1024];    //creates new array to store the characters
    for(int i = 0; i < size; i++)
    {
        char * temp = strdup(cmd.at(i).c_str());
        ary[i] = temp;  
    }
    ary[size] = '\0'; 
   
    if(num_par == true)
    {                                             
        if((paren_point == false) && (counter != 2) && (paren_count != 2))      
        {
            return EXIT_FAILURE;   
        }
        if((paren_point == true) && (paren_count == 2))
        {                  
            return EXIT_FAILURE;
        }
    }
        if((ok == false) && (count_p != 2))
        {                          
            return EXIT_FAILURE;             
        }
        if((ok == true) && (count_p == 2))
        {                       
            return 0;
        }
        if (strcmp(ary[0], "exit") == 0)
        {                           
            exit(0);                             
        }
        pid_t pid = fork();                        
        if (pid == -1)
        {                         
            perror("fork");
            return EXIT_FAILURE;
        }
        else if(pid == 0)
        {  
            if(execvp(ary[0], ary) == -1)
            {        
                perror("execvp");                   
    	          exit(1);
    	          return EXIT_FAILURE;
            }
        }
        int status;          
        if ( waitpid(pid, &status, 0) == -1 )
        {     
            perror("waitpid");
            exit(1);
            return EXIT_FAILURE;
        }
        if(WIFEXITED(status))
        {               
            const int exitflag = WEXITSTATUS(status);   
    	    if(exitflag != 0)
    	    {                           
    		    return EXIT_FAILURE;
            }
        }
    return EXIT_SUCCESS;                             
}

void Parse(vector<string> &command, int cmd_size, bool ok, bool pointer, 
int point_count, bool paren_pointer, int Paren_count, int Paren_count_point, 
int Paren_count_n, int &Par_size, bool in_paren)
{
    if(cmd_size < 1)
    {  
        return;
    }
    if((point_count == 3) && (in_paren != true))
    {                      
        paren_pointer = true;
    }
    
    if(Par_size <= 0)
    {
        in_paren = false;
        Par_size = 0;
    }
    
    if(in_paren == false)
    {
        Paren_count_point = point_count; 
    }
    while(command.at(0).at(0) == '(')
    {                   
        in_paren = true;
        Build(command, cmd_size, Par_size);               
    }
    
    int P_size;                                      
    string con;                               
    
    int point_count_p = point_count;                          
    
    unsigned i = 0;                                  
    vector<string> tempvect;                              
    
    while((i < command.size()) && (command.at(i) != ";") && (command.at(i) != "||")                  
    && (command.at(i) != "&&"))
    {               
        tempvect.push_back(command.at(i));
        ++i;
        P_size = i;
    }
    
    string first = command.at(0);          
    
    if((first == ";") || (first == "||") || (first == "&&"))
    {
        command.erase(command.begin(), command.end());  
        cout << "bash: syntax error near unexpected token '" <<  first << "'" << endl;
        return;
    }
    
    if(i == ((command.size())))
    {          
        con = "0";
        point_count = 0;                        
    }                                          
    else if(command.at(i) == "&&")
    {        
        con = "&&";
        point_count = 1;
    }
    else if(command.at(i) == "||")
    {         
        con = "||"; 
        point_count = 2;
    }
    else if(command.at(i) == ";")
    {        
        con = ";";
        point_count = 3;
    }
    
    
    if(P_size > 126)
    {                             
        cout << "Command too long" << endl;
        return;
    }
    
    int temp;
    unsigned index = 0;
    unsigned iterator = 0;
    bool match = false;
    bool p1 = false;
    bool p2 = false;
    bool if_test = true;
    bool if_p = false;
    
    if(tempvect.at(0) == "]")
    {
        cout << "Error: no match bracket" << endl;
        return;
    }
    if((tempvect.at(0) == "test") || (tempvect.at(0) == "["))
    {
        vector<string> Tempvec2 = tempvect;
        if(tempvect.at(0) == "test")
        {
            if_test = true;
            while(index < tempvect.size())
            {
                if(tempvect.at(index) == "[" || tempvect.at(index) == "]" )
                {
                    if_p = true;
                    break;
                }
                ++index;
            }
        }
        if(tempvect.at(0) == "[")
        {
            if_p = true;
            if_test = false;
            p1 = true;
        }
        while(iterator < tempvect.size())
        {
            if(tempvect.at(iterator) == "]")
            {
                if_p = true;
                if_test = false;
                p2 = true;
                break;
            }
            ++iterator;
        }
        if(p1 && p2)
        {
            match = true;
        }
        if( (if_test == false && match == false) || 
        (if_test == true && if_p == true) )
        {
            if(match == false)
            {
                cout << "Error: no match bracket" << endl;
                return;
            }
        }
        Tempvec2.erase(Tempvec2.begin());
    
        temp = test(Tempvec2, 0, ok, point_count, point_count_p, paren_pointer, Paren_count_point, in_paren);
    
    }
    else
    {
        temp = exec(tempvect, ok, point_count, point_count_p, paren_pointer, Paren_count_point, in_paren, P_size);  
    }
    
    pointer = ok;  
    
    if(temp == 0)
    {
        ok = true;
    }
    else
    {
        ok = false;
    }
    
    if(in_paren != true)
    {                           
        Paren_count_n = point_count_p;
    }
    
    if(in_paren != true)
    {
        paren_pointer = ok;
    }
    if(con == "0")
    {      
        command.erase(command.begin(), command.begin() + i);
        if(in_paren == true)
        {
            Par_size = Par_size - i;
            if(Par_size <= 0)
            {
                Paren_count_point = Paren_count_n;
                Paren_count_n = 0;
                paren_pointer = ok;
            }
        }
    
        ok = true;
        Parse(command, command.size(), ok, pointer, point_count, paren_pointer, Paren_count, Paren_count_point,  Paren_count_n, Par_size, in_paren);
    }
    
    if(con == "&&")
    {    
        command.erase(command.begin(), command.begin() + i + 1);
        if(in_paren == true)
        {
            Par_size = Par_size - i - 1;
            if(Par_size <= 0)
            {
                Paren_count_point = Paren_count_n;
                Paren_count_n = 1;
                paren_pointer = ok;
            }
        }
    
        Parse(command, command.size(), ok, pointer, point_count, paren_pointer, Paren_count, Paren_count_point,  Paren_count_n, Par_size, in_paren);
    }
    
    if(con == "||")
    {
        command.erase(command.begin(), command.begin() + i + 1);
        if(in_paren == true)
        {
            Par_size = Par_size - i - 1;
            if(Par_size <= 0)
            {
                Paren_count_point = Paren_count_n;
                Paren_count_n = 2;
                paren_pointer = ok;
            }
        }
    
        if((pointer == true) && (point_count_p == 2))
        {
            ok = true;
        }
        Parse(command, command.size(), ok, pointer, point_count, paren_pointer, Paren_count, Paren_count_point, Paren_count_n, Par_size, in_paren);
    }
    
    if(con == ";")
    {                   
        command.erase(command.begin(), command.begin() + i + 1);
        if(in_paren == true)
        {
            Par_size = Par_size - i - 1;
            if(Par_size <= 0)
            {
                Paren_count_point = Paren_count_n;
                Paren_count_n = 3;
                paren_pointer = ok;
            }
        }
        ok = true;
        Parse(command, command.size(), ok, pointer, point_count, paren_pointer, Paren_count, Paren_count_point, Paren_count_n, Par_size, in_paren);
    }
    return;
}


bool check_bracket(vector<string> command)
{
    stack<char>check;
	for(unsigned j = 0; j < command.size(); ++j)
	{
        for(unsigned i = 0; i < command.at(j).size(); ++i)
        {
    		if(command.at(j).at(i) == '(')
    		{
    			check.push(command.at(j).at(i));
    		}
    		else if(command.at(j).at(i) == ')')
    		{
    			if(check.empty())
    			{
    				return false;
    			}
    			else
    			{
    				check.pop();
    			}	
    		}
        }
	}
	return check.empty() ? true:false;
}

void getname() //get username and host name
{
    char* name;
    struct passwd* pass;
    pass = getpwuid(getuid());
    name = pass->pw_name;
    printf("%s@", name);
    
    char host[1024];
    host[1023] = '\0';
    gethostname(host, 1023);
    printf("%s$ ", host);
    return;
}

int main(){
    vector<string> command;                                                             //vector to hold our string
    string str_command;                                                                     //will simply be our input string
    int size = 0;

    while(1)
    {
        getname();                                                             
        getline(cin, str_command);                                                          
        if (str_command.find('#') != string::npos) //trim everything after #, including #
        {
            str_command.erase(str_command.find('#'), (str_command.size() - str_command.find('#')));
        }
        char_separator<char> deliminator(" ", ";");                                   // parameters for our input
        tokenizer< char_separator<char> > mytok(str_command, deliminator);
        
        for(tokenizer<char_separator<char> >::iterator it = mytok.begin(); it != mytok.end(); ++it)
        {
            command.push_back(*it);                                                       
        }
    
        bool nomatch = check_bracket(command);                                              
        if(nomatch == false)
        {
            cout << "Error, bracket are not match" << endl;
            command.clear();
        }
         Parse(command, command.size(), 1, 0, 0, 1, 0, 0, 0, size, 0);                          
    }
    
    return 0;
}