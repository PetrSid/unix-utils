#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[])
{
	if(argc < 2){
		return 2;
	}

	const char* path = argv[1];

	int fd = open(path, O_RDONLY);

	// Check File Discripter
	if(fd == -1){

		return 1; // Error code
	}

	char buf[BUF_SIZE];

	while(1)
	{
		// ssize_t
		ssize_t rb = read(fd, buf, sizeof(buf));
		if(rb == 0){ break; }
		if(rb == -1)
		{
			// Error File is broken
			break;
		}
		write(STDOUT_FILENO, buf, rb);
	}
	close(fd);
	return 0;
}
