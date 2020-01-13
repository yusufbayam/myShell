#include <iostream>
#include <string>
#include <array>
#include <pwd.h>
#include <unistd.h> //fork()
#include <sys/types.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <wait.h>
#include <bits/stdc++.h>
#include <vector>
#include <fstream>

using namespace std;

void printFile(string &fileName) // prints content of the file 
{
	string myLine;
	ifstream MyFile(fileName);  // opens file for input
	while (getline(MyFile, myLine))	// read file line by line
	{
		cout << myLine << endl;			// output each line of file
	}
}

void printToOtherFile(string &inputF, string &outputF)	// print contents of inputF to outputF
{
	string myLine;			
	ifstream inputFile(inputF);			// open input file for reading
	ofstream outputFile(outputF);		// open output file for writing
	while (getline(inputFile, myLine))	// read each line of input file until it reaches end 
	{
		outputFile << myLine << endl;			// write each line of input file o output file
	}
	outputFile.close();					// close output file
}

void addToFootprint(vector<string> &footprint, string str)		// add given string to footprint vector
{
	int fpSize = footprint.size();								
	footprint.insert(footprint.begin(), str);			// insert string to head of footprint vector
	if (fpSize >= 15)							// if size of footprint vector is larger than 15 then erase last element
	{
		footprint.erase(footprint.end());
	}
}
void printFootprint(vector<string> &v)			// print footprint vector with numbers on left
{
	int i = 1;
	for (auto it = v.begin(); it != v.end(); ++it)	// iterate over passed vector and print each element
	{
		cout << i << " " << *it << endl;
		i++;
	}
}


vector<string> readInput(vector<string> &vct, string &input)	// read input from user and store each token in vector
{
	string token;			// string for reading each token
	stringstream strm(input);		//read input as string stream

	int i = 0;

	while (strm >> token)		//read all token from input stream until none left
	{
		vct.push_back(token);	// push token to vector
		i++;
	}

	return vct;
}

int main()
{

	string username;
	vector<string> footprint;		// vector for storing executed commands

	username = getlogin();		// get username
	int i = 0;
	while (1)
	{

		vector<string> inputs;		// vector for storing input as tokens
		cout << username << " >>> ";	// print username at start of the shell

		string inputLine;
		getline(cin, inputLine);	// read input from terminal

		readInput(inputs, inputLine);	// read input and tokenize it then store them in inputs vector

		int inputSize = inputs.size();

		if (inputSize == 1)			// if inputSize = 1 enter this if condition (listdir, currentpath, footprint, exit)
		{

			if (inputs[0] == "listdir")  // if input is "listdir" enter this if branch
			{
				addToFootprint(footprint, inputLine);	// add command to footprint vector
				pid_t pid = fork();  // fork for executing listdir command
				if (pid == -1)
				{ 					// fork failed
				}
				else if (pid == 0)
				{ 					// child process

					execlp("ls", "ls", NULL);	// execute "listdir" command
					exit(0);		// terminate child process
				}
				else				// parent process
				{ 					
					wait(NULL);		//wait for child to terminate
				}
			}

			else if (inputs[0] == "currentpath")	// if input is "currentpath" enter this else if branch
			{
				addToFootprint(footprint, inputLine);	// add command to footprint vector
				pid_t pid = fork();						// fork for execute command
				if (pid == -1)							// if fork fails
				{
				}
				else if (pid == 0)						// child process
				{	
					execlp("pwd", "pwd", NULL);			// execute "currentpath"
					exit(0);					// terminate child process
				}
				else		//parent process
				{
					wait(NULL);		// wait for child to terminate
				}
			}

			else if (inputs[0] == "footprint")   //enter this branch is given command is "footprint" 
			{
				addToFootprint(footprint, inputLine);  // add command to footprint vector before printing it
				pid_t pid = fork();
				if (pid == -1)			// fork fail
				{	
				}
				else if (pid == 0)		// child process
				{
					printFootprint(footprint);		// print footprint vector
					exit(0);			// terminate process
				}
				else
				{
					wait(NULL);			// wait for child to terminate
				}
			}

			else if (inputs[0] == "exit")	// if input is exit enter this branch
			{
				break;
			}
			else{
				cout << "Command not supported" << endl;
			}
		}

		else if (inputSize == 2)		// if inputSize = 2 (listdir -a, printfile fileName)
		{
			if (inputs[0] == "listdir" && inputs[1] == "-a") // if command is listdir -a enter this branch
			{
				addToFootprint(footprint, inputLine);		// add command to footprint vector
				pid_t pid = fork();
				if (pid == -1)
				{ // fork failed
				}
				else if (pid == 0)			// child process
				{ 						

					execlp("ls", "ls", "-a", NULL);		// execute listdir -a
					exit(0);
				}
				else		// parent process
				{ 
					wait(NULL);		//wait for child to terminate
				}
			}
			if (inputs[0] == "printfile")		// if inputsize is 2 and first input is printfile then command is "printfile FILENAME"
			{
				addToFootprint(footprint, inputLine);	// add valid command to footprint vector
			

				string *p;
				p = &inputs[1];			// fileName is second element of inputs vector

				pid_t pid = fork();			// fork failed
				if (pid == -1)
				{ 
				}
				else if (pid == 0)		// child process
				{ 
					char cstr[inputs[1].size() + 1];  	// change filename to char array from string
					strcpy(cstr, inputs[1].c_str());
					printFile(*p);		// print file contents

					exit(0);
				}
				else	//wait for child to terminate
				{ 
					wait(NULL);
				}
			}
		}

		else if (inputSize == 4)	// if input size = 4,then enter this branch either "printfile f1 > f2" or "listdir | grep text"
		{
			if (inputs[0] == "printfile")
			{
				addToFootprint(footprint, inputLine);
				string *fName = &inputs[1];

				string *p;  
				p = &inputs[1];  // take name of input file
				string *o;
				o = &inputs[3]; // take name of output file
				pid_t pid = fork();
				if (pid == -1)	// fork failed
				{ 
				}
				else if (pid == 0)	// child process
				{ 
					char cstr[inputs[1].size() + 1];  
					strcpy(cstr, inputs[1].c_str());
					printToOtherFile(*p, *o);		// write contents of input file to output file

					exit(0);
				}
				else	//wait for child to terminate
				{ 
					wait(NULL);
				}
			}
			else if (inputs[0] == "listdir")		// if input is "listdir | grep text"
			{
				addToFootprint(footprint, inputLine);
				int fds[2];		// file descriptor
				pipe(fds);		// pipe process
				pid_t pid1, pid2;

				if (pid1 = fork() == 0) // child process #1, this process will write
				{

					close(fds[0]);		// close read end since it is not used
					dup2(fds[1], 1);	// make write end = STDOUT
					close(fds[1]);		// close write end

					execlp("ls", "ls", NULL);  // execute listdir command

					exit(0);
				}
				else		// parent process
				{
					pid2 = fork();

					if (pid2 < 0)
					{
					}
					else if (pid2 == 0)		//child process 2
					{ 

						close(fds[1]);		// close write end of process
						dup2(fds[0], 0);	// make read end = STDIN
						close(fds[0]);		// close read end

						string str = inputs[3];
						char *cstr = new char[str.length() + 1];		// convert string to char array because execlp takes arguments as char array
						std::strcpy(cstr, str.c_str());

						char *p = std::strtok(cstr, " ");
						while (p != 0)
						{
							p = std::strtok(NULL, " ");
						}
						execlp("grep","grep", cstr, NULL);		// take input of child process 1 and execute grep with given argument
						exit(0);			// terminate child process
					}
					else
					{
						close(fds[1]);
						wait(NULL);		// wait child to terminate
						wait(NULL);		// wait child to terminate
					}
				}
			}
		}

		else if (inputSize == 5)		// if input size = 5 , command is "listdir -a | grep text"
		{
			addToFootprint(footprint, inputLine);
			int fds[2];				// file descriptor
			pipe(fds);				// create pipe
			pid_t pid1, pid2;		

			if (pid1 = fork() == 0) // child process #1, this process will write
			{
				close(fds[0]);		// close read end of child process 1
				dup2(fds[1], 1);	// make write end as STDOUT
				close(fds[1]);		// close write end

				execlp("ls", "ls", "-a", NULL);

				exit(0);
			}
			else
			{
				pid2 = fork();

				if (pid2 < 0)
				{
				}
				else if (pid2 == 0)
				{ 							//child process 2, this process will read
					close(fds[1]);			// close write end
					dup2(fds[0], 0);		// dup2 read end
					close(fds[0]);			// close read end

					string str = inputs[4];			

					char *cstr = new char[str.length() + 1];		// convert string to char array because execlp arguments are char array
					std::strcpy(cstr, str.c_str());

					char *p = std::strtok(cstr, " ");
					while (p != 0)
					{
						p = std::strtok(NULL, " ");
					}
					execlp("grep","grep", cstr, NULL);  // execute grep from ls command with given arguments
					exit(0);
				}
				else		// parent process
				{
					close(fds[1]);		//close read end
					wait(NULL);			// wait childs to terminate
					wait(NULL);
				}
			}
		}
		else
		{
			cout << "bum bum pow patlanzi hojam" << endl;
		}
	}
	return 0;
}