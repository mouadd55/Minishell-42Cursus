#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
 #include <sys/stat.h>
int main()
{
    int fd = open("file1", O_CREAT, O_RDWR);
    stat();
}