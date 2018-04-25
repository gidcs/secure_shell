#include <iostream>
#include <sstream>
#include <vector>
#include "src/func.hpp"

using namespace std;

vector<string> get_args(){
    vector<string> cmd;
    string cmd_args;
    istringstream iss;
    string token;

    getline(cin, cmd_args);
    iss.str(cmd_args);
    while(getline(iss, token, ' ')){
        if(token.size()){
            cmd.push_back(token);
        }
    }
    return cmd;
}

int main(int argc, char *argv[]){
    int uid, gid;
    int ret;
    vector<string> cmd;
    if(argc != 3){
        cout << "Usage:" << endl;
        cout << "   " << argv[0] << " <uid> <gid>" << endl;
        exit(-1);
    }
    uid = atoi(argv[1]);
    gid = atoi(argv[2]);
    ret = func_setuid(uid, gid);
    if(ret != 0){
        ERR("Failed on setuid/setgid.");
        exit(-1);
    }
    while(1){
        cout << "> ";
        cmd = get_args();
        if(cmd.size() == 0){
            continue;
        }
        if(cmd[0].compare("cat") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_cat(cmd[1]);
        }
        else if(cmd[0].compare("cd") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_cd(cmd[1]);
        }
        else if(cmd[0].compare("chmod") == 0){
            if(cmd.size() != 3){
                ERR("Too less/much arguments.");
                continue;
            }
            func_chmod(cmd[1], cmd[2]);
        }
        else if(cmd[0].compare("echo") == 0){
            if(cmd.size() != 3){
                ERR("Too less/much arguments.");
                continue;
            }
            func_echo(cmd[1], cmd[2]);
        }
        else if(cmd[0].compare("exit") == 0 ||
                cmd[0].compare("quit") == 0){
            break;
        }
        else if(cmd[0].compare("find") == 0){
            if(cmd.size() == 1){
                cmd.push_back(".");
            }
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_find(cmd[1]);
        }
        else if(cmd[0].compare("help") == 0){
            func_help();
        }
        else if(cmd[0].compare("id") == 0){
            func_id();
        }
        else if(cmd[0].compare("mkdir") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_mkdir(cmd[1]);
        }
        else if(cmd[0].compare("pwd") == 0){
            func_pwd();
        }
        else if(cmd[0].compare("rm") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_rm(cmd[1]);
        }
        else if(cmd[0].compare("rmdir") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_rmdir(cmd[1]);
        }
        else if(cmd[0].compare("stat") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_stat(cmd[1]);
        }
        else if(cmd[0].compare("touch") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_touch(cmd[1]);
        }
        else if(cmd[0].compare("umask") == 0){
            if(cmd.size() != 2){
                ERR("Too less/much arguments.");
                continue;
            }
            func_umask(cmd[1]);
        }
        else{
            ERR("Command not found.");
        }
    }
    return 0;
}
