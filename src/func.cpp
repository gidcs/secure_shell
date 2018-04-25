#include "func.hpp"

mode_t mask;

void print_err(int errsv){
    switch(errsv){
        case EACCES:
            ERR("Permission denied.");
            break;
        case ELOOP:
            ERR("A loop exists in symbolic links.");
            break;
        case ENAMETOOLONG:
            ERR("The length of the path exceeds.");
            break;
        case ENOENT:
            ERR("No such file or directory");
            break;
        case ENOTDIR:
            ERR("Not a directory.");
            break;
        case EPERM:
            ERR("Permission denied.")
            break;
        case EEXIST:
            ERR("Pathname already exists.");
            break;
        case EBADF:
            ERR("Invalid fd.")
            break;
        case EMFILE:
            ERR("Too many file descriptors");
            break;
        case ENFILE:
            ERR("Too many opened fd.");
            break;
        case ENOMEM:
            ERR("Insufficient memory.");
            break;
        case EBUSY:
            ERR("Pathname is currently in use by someone.");
            break;
        case EFAULT:
            ERR("Pathname points outside your accessible address space.");
            break;
        case ENOTEMPTY:
            ERR("Pathname not empty.");
            break;
        case EISDIR:
            ERR("Pathname refers to a directory.");
            break;
        case EIO:
            ERR("An I/O error occurred,");
            break;
        default:
            ERR("Unknown error.");
    }
}

string format_type(mode_t st_mode){
    switch(st_mode){
        case S_IFBLK:
            return "block device";
        case S_IFCHR:
            return "character device";
        case S_IFDIR:
            return "directory";
        case S_IFIFO:
            return "FIFO/pipe";
        case S_IFLNK:
            return "symbolic link";
        case S_IFREG:
            return "regular file";
        case S_IFSOCK:
            return "socket";
        default:
            return "unknown?";
    }
}

string format_size(off_t st_size){
    int unit = 0;
    double size = st_size;
    stringstream ss;
    string ret;
    while(size >= 1024.0){
        size = size / 1024.0;
        unit++;
    }

    if(unit != 0){
        ss << fixed << setprecision(1) << size;
        ss >> ret;
    }
    else{
        ss << size;
        ss >> ret;
    }
    if(unit == 0){
        //do nothing
    }
    else if(unit == 1){
        ret += "KB";
    }
    else if(unit == 2){
        ret += "MB";
    }
    else if(unit == 3){
        ret += "GB";
    }
    else if(unit == 4){
        ret += "TB";
    }
    return ret;
}

string format_mode(mode_t st_mode){
    string ret;
    stringstream ss;
    ss << oct << st_mode;
    ss >> ret;
    /*if(ret.size() > 4){
        ret = ret.substr(ret.size()-4);
    }*/
    return ret;
}

string format_device(dev_t st_dev){
    stringstream ss;
    string ret;
    ss << major(st_dev) << "," << minor(st_dev);
    ss >> ret;
    return ret;
}

void func_cat(string p){
    int fd;
    char buf[2048];
    size_t nbytes;
    ssize_t bytes_read;
    fd = open(p.c_str(), O_RDONLY);
    if(fd == -1){
        print_err(errno);
    }
    else{
        while(1){
            nbytes = sizeof(buf);
            bytes_read = read(fd, buf, nbytes);
            if(bytes_read == 0){
                break;
            }
            else if(bytes_read == -1){
                if(errno == EINTR){
                    continue;
                }
                else{
                    ERR("Unknown error.");
                    break;
                }
            }
            cout << buf;
        }
        close(fd);
    }
}

void func_cd(string p){
    int ret;
    ret = chdir(p.c_str());
    if(ret != 0){
        print_err(errno);
    }
}

void func_chmod(string m, string p){
    stringstream ss;
    mode_t mode;
    int ret;
    ss.str(m);
    ss >> oct >> mode;
    ret = chmod(p.c_str(), mode);
    if(ret != 0){
        print_err(errno);
    }
}

void func_echo(string s, string p){
    int fd;
    size_t nwrite;
    ssize_t bytes_write;
    char *buf, *buf_bak;
    fd = open(p.c_str(), O_WRONLY | O_APPEND | O_CREAT, ~mask & 0666);
    if(fd == -1){
        print_err(errno);
    }
    else{
        s += "\n";
        buf = strdup(s.c_str());
        buf_bak = buf;
        nwrite = s.size();
        while(nwrite){
            bytes_write = write(fd, buf, nwrite);
            if(bytes_write == -1){
                if(errno == EINTR){
                    continue;
                }
                else{
                    ERR("Unknown error.");
                    break;
                }
            }
            buf += bytes_write;
            nwrite -= bytes_write;
        }
        free(buf_bak);
        close(fd);
    }
}

void func_find(string p){
    DIR *dp;
    struct dirent *dirp;
    string name;
    dp = opendir(p.c_str());
    if(dp == NULL){
        print_err(errno);
    }
    else{
        while(1){
            dirp = readdir(dp);
            if(dirp == NULL) break;
            name = dirp->d_name;
            if(name.compare(".") == 0 ||
                name.compare("..") == 0)
                continue;
            func_stat(p + "/" + name, SIMPLE_STAT);
        }
        closedir(dp);
    }
}

void func_help(){
    cout << "Help:" << endl;
    cout << "cat {file}:              Display content of {file}." << endl;
    cout << "cd {dir}:                Switch current working directory to {dir}." << endl;
    cout << "chmod {mode} {file/dir}: Change the mode (permission) of a file or directory." << endl;
    cout << "                         {mode} is an octal number." << endl;
    cout << "echo {str} [filename]:   Display {str}. If [filename] is given," << endl;
    cout << "                         open [filename] and append {str} to the file." << endl;
    cout << "exit:                    Leave the shell." << endl;
    cout << "find [dir]:              List files/dirs in the current working directory" << endl;
    cout << "                         or [dir] if it is given." << endl;
    cout << "                         Minimum outputs contatin file type, size, and name." << endl;
    cout << "help:                    Display help message." << endl;
    cout << "id:                      Show current euid and egid number." << endl;
    cout << "mkdir {dir}:             Create a new directory {dir}." << endl;
    cout << "pwd:                     Print the current working directory." << endl;
    cout << "rm {file}:               Remove a file." << endl;
    cout << "rmdir {dir}:             Remove an empty directory." << endl;
    cout << "stat {file/dir}:         Display detailed information of the given file/dir." << endl;
    cout << "touch {file}:            Create {file} if it does not exist," << endl;
    cout << "                         or update its access and modification timestamp." << endl;
    cout << "umask {mode}:            Change the umask of the current session." << endl;
}

void func_id(){
    cout << "EUID: " << geteuid() << endl;
    cout << "EGID: " << getegid() << endl;
}

void func_mkdir(string p){
    int ret;
    ret = mkdir(p.c_str(), ~mask & 0777);
    if(ret != 0){
        print_err(errno);
    }
}

void func_pwd(){
    char *cwd = getcwd(NULL, 0);
    cout << cwd << endl;
    free(cwd);
}

void func_rm(string p){
    int ret;
    ret = unlink(p.c_str());
    if(ret != 0){
        print_err(errno);
    }
}

void func_rmdir(string p){
    int ret;
    ret = rmdir(p.c_str());
    if(ret != 0){
        print_err(errno);
    }
}

void func_stat(string p, int method){
    int ret;
    struct stat sb;
    string name;
    string type;
    string size;
    string mode;
    string device;
    size_t found;
    ret = stat(p.c_str(), &sb);
    if(ret == -1){
        print_err(errno);
    }
    else{
        if(method == SIMPLE_STAT){
            found = p.find_last_of("/");
            name = p.substr(found + 1);
            type = format_type(sb.st_mode & S_IFMT);
            if(type.find("device") != string::npos){
                size = format_device(sb.st_dev);
            }
            else{
                size = format_size(sb.st_size);
            }
            cout << left << setw(20) << name;
            cout << setw(20) << type;
            cout << right << setw(10) << size << endl;
        }
        else{
            name = p;
            type = format_type(sb.st_mode & S_IFMT);
            size = format_size(sb.st_size);
            mode = format_mode(sb.st_mode & ~S_IFMT);
            device = format_device(sb.st_dev);
            cout << left << setw(25) << "File" << ": "
                 << name << endl;
            cout << setw(25) << "Type" << ": "
                 << type << endl;
            cout << setw(25) << "Device" << ": "
                 << device << endl;
            cout << setw(25) << "Mode" << ": "
                 << mode << endl;
            cout << setw(25) << "Ownership" << ": "
                 << "UID=" <<sb.st_uid << endl;
            cout << setw(25) << "" << "  "
                 << "GID=" << sb.st_gid << endl;
            cout << setw(25) << "Size" << ": "
                 << size << endl;
            cout << setw(25) << "Last status change" << ": "
                 << ctime(&(sb.st_ctime));
            cout << setw(25) << "Last file access" << ": "
                 << ctime(&(sb.st_atime));
            cout << setw(25) << "Last file modification" << ": "
                 << ctime(&(sb.st_mtime));
        }
    }
}

void func_touch(string p){
    int fd;
    int ret;
    fd = open(p.c_str(), O_RDONLY);
    if(fd == -1){
        fd = creat(p.c_str(), ~mask & 0666);
        if(fd == -1){
            print_err(errno);
        }
        else{
            close(fd);
        }
    }
    else{
        close(fd);
        ret = utime(p.c_str(), NULL);
        if(ret != 0){
            print_err(errno);
        }
    }
}

void func_umask(string m){
    stringstream ss;
    ss.str(m);
    ss >> oct >> mask;
    umask(mask);
}

int func_setuid(int uid, int gid){
    int ret;
    ret = setgid(gid);
    if(ret != 0) return ret;
    ret = setuid(uid);
    if(ret != 0) return ret;
    return 0;
}
