#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 4096

int cp_file(const char* src, const char* dst);
int cp_check(struct stat* inf,
	     const char* src, const char* dst);

int cp_bytes_file(int fd_src, int fd_dst);
int cp_dir(const char* src, const char* dst);



int main(int argc, char* argv[])
{
	int flags, opt;
	
	flags = 0;
	while((opt = getopt(argc, argv, "fd")) != -1)
	{
		switch(opt)
		{
			case 'f': flags = 1; break;
			case 'd': flags = 2; break;
			default: 
				return 1;
		}	
	}
	if (argc - optind < 2){
		return 1;
	}

	const char* src = argv[optind];
	const char* dst = argv[optind + 1];

	if(flags == 1){
		cp_file(src, dst);
	}
	else if(flags == 2){
		// cp_dir();
	}
	else{
		return 1;
	}

	return 0;
}



int cp_file(const char* src, const char* dst)
{
	int fd_src = open(src, O_RDONLY);
	if(fd_src == -1){
		return 1;
	}
	
	struct stat info;
	int st = stat(dst, &info);

	if(st == -1){
		// No such file or directory
		// JUST CREAT NEW FILE
		creat(dst,
			S_IRUSR | S_IWUSR |
			S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH );
		stat(dst, &info);
	}

	int fd_dst = cp_check(&info, src, dst);
	if (fd_dst == -1){
		return -1;
	}
	
	int succ = cp_bytes_file(fd_src, fd_dst);
	if(succ == -1){ return -1; }



	// CLOSE FILES
	close(fd_src);
	close(fd_src);

	return 0;
}


int cp_check(struct stat* inf,
	     const char* src,  const char* dst)
{

	if(S_ISREG(inf->st_mode)){
		return open(dst, O_WRONLY);
	}
	else if(S_ISDIR(inf->st_mode))
	{	
		int dir_fd = open(dst, O_RDONLY);
		if(dir_fd == -1){ 
			return -1; 
		}
		int fd_file = openat(dir_fd, src,
				O_WRONLY | O_CREAT,
				S_IRUSR  | S_IWUSR |
				S_IRGRP  | S_IWGRP |
				S_IROTH  | S_IWOTH );
		close(dir_fd);
		return fd_file;
	}
	else{
		// This is not file or dir
		return -1;
	}
}

int cp_bytes_file(int fd_src, int fd_dst)
{
	char buf[BUF_SIZE];
	while(1){
		ssize_t rb = read(fd_src, buf, sizeof(buf));
		if(rb == 0) { break; }
		if(rb == -1){ return -1; }
		
		write(fd_dst, buf, rb);
	}
	return 0;
}


int cp_dir(const char* src, const char* dst){
	return 0;
}

