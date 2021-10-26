#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <fstream>
using namespace std;

void error(){
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(0);
}

vector<char*> charVect(vector<string> lineVect) {   // make lineVect of strings into lineVect of char*
    vector<char*> charVect;
    for(unsigned int i = 0; i < lineVect.size(); i++){
        charVect.push_back(const_cast<char*>(lineVect[i].c_str()));
    }
    charVect.push_back(nullptr);
  return charVect;
}

void fork_func(vector<string> paths, vector<string> lineVect){
    string command = lineVect[0];
    for(unsigned int i = 0; i < paths.size(); i++){
        string currPath = paths[i] + "/" + command;
        pid_t pid = fork();
        
        int status = -1;
        if (pid == 0)       // if child path, try to execute
        {
            int fd = open(lineVect[1].c_str(), O_RDWR);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);

            if (execv(currPath.c_str(), &charVect(lineVect)[0]) < 0) {
                cout << errno;
                error();
            }
        }
        else{
            waitpid(pid, &status, 0); // ls
        }
        
        if (status == 0){
            break;
        }
    } // end for
}

void command_ls(vector<string> vect, vector<string> paths){
    if (vect.size() >= 2){          // ls was given file path
        string path = vect[1];
        string path1 = "/bin" + path;
        const char* charPath1 = path1.c_str();
        string path2 ="/usr/bin" + path;
        const char* charPath2 = path2.c_str();
        
        if ((access(charPath1, X_OK) == -1) && (access(charPath2, X_OK) == -1)){     // fails to access from both paths, print error and exit
            string error = "ls: " + path + ": No such file or directory\n";
            const char * s = error.c_str();
            char error_message[error.size()];
            strcpy(error_message, s);
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
        }
    }
    else{       // no file passed
        fork_func(paths, vect);
    }
}

void command_cd(vector<string> vect){
    if (vect.size() == 2){     // one arg passed in
        const char* path = vect[1].c_str();
        // cout << path << endl;
        if (chdir(path) == -1){     // chdir failed, throw error
            error();
        }
    }
    else {       // cd given 0 or too many args
        error();
    }
}

vector<string> splitLine(string line){      // returns vector of diff words in line
    vector<string> vect;
    string word = "";
    for(unsigned int i = 0; i < line.size(); i++){               // get command from line
        if (line[i] == '>'){
            vect.push_back(">");
            word = "";
        }

        if (line[i] == ' '){
            vect.push_back(word);
            word = "";
        }
        else{
            word += line[i];
        }
    }
    vect.push_back(word);
    return vect;
}

void interactiveModeParsing(){

    vector<string> paths;
    paths.push_back("/bin");
    string line = "";
    while(getline(cin, line)){
        if (line == "exit"){
            exit(0);
        }
        vector<string> lineVect = splitLine(line);      // calls splitLine for vector of words
        string command = lineVect[0];
        if (command == "ls"){
            command_ls(lineVect, paths);
        }
        else if (command == "cd"){
            command_cd(lineVect);
        }
        else if (command == "path"){
            paths.clear();
            for(unsigned int i = 1; i < lineVect.size(); i++){
                paths.push_back(lineVect[i]);
            }
        }
        else{
            // try to execute other commands, if not error
            error();
        }
        cout << "wish> ";
    }
    
}

void batchModeParsing(char *argv[]){
    ifstream file;
    file.open(argv[1]);
    if (!file.is_open()){               // open file, throw error if it did not open properly, then exit
        error();
    }
    vector<string> paths;
    paths.push_back("/bin");
    string line = "";
    while(getline(file, line)){
        if (line == "exit"){
            exit(0);
        }
        vector<string> lineVect = splitLine(line);      // calls splitLine for vector of words
        string command = lineVect[0];
        
        if (command == "ls"){
            command_ls(lineVect, paths);
        }
        else if (command == "cd"){
            command_cd(lineVect);
        }
        else if (command == "path"){
            paths.clear();
            for(unsigned int i = 1; i < lineVect.size(); i++){
                paths.push_back(lineVect[i]);
            }
        }
        else{
            error();
        }
    }
}

int main(int argc, char *argv[]){
    if (argc == 1){ // interactive mode
        interactiveModeParsing();
    }
    else{  // batch mode
        batchModeParsing(argv);
    }
    return 0;
}
