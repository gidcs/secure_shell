#include <iostream>
#include <sstream>          //stringstream
#include <cmath>            //fmod
#include <iomanip>          //setw
#include <vector>           //vector
#include <algorithm>        //sort
#include <unistd.h>         //getcwd, chdir, read, write
#include <errno.h>          //errno
#include <fcntl.h>          //open
#include <string.h>         //strdup
#include <sys/types.h>      //umask
#include <sys/sysmacros.h>  //major, minor
#include <sys/stat.h>       //umask
#include <dirent.h>         //opendir, readdir
#include <utime.h>          //utime

using namespace std;

#define ERR(s) \
    cerr << "ERR: " << s << endl;

#define SIMPLE_STAT 0
#define NORMAL_STAT 1

void func_cat(string p);
void func_cd(string p);
void func_chmod(string m, string p);
void func_echo(string s, string p);
void func_find(string p);
void func_help();
void func_id();
void func_mkdir(string p);
void func_pwd();
void func_rm(string p);
void func_rmdir(string p);
void func_stat(string p, int method=NORMAL_STAT);
void func_touch(string p);
void func_umask(string m);
int func_setuid(int uid, int gid);
