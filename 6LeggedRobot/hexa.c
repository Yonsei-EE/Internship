# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/signal.h>
# include <sys/ioctl.h>
# include <sys/poll.h>

# include <termios.h>

int open_serial( char* dev_name, int baud ) //, int vtime, int vmin )
{
	int fd;
	struct termios newtio;

	// 시리얼 포트를 연다
	fd = open(dev_name, O_RDWR|O_NOCTTY);
		// O_RDWR	// 읽기 / 쓰기 모드로 장치를 연다
		// O_NOCTTY	// 데이터 전송시 <ctrl>+c 문자가 와도 프로그램 종료되지 않기 위해 controlling tty를 막음

	if ( fd < 0 )
	{
		// 시리얼 포트 열기 실패 시		
		printf("Device OPEN FAIL %s\n", dev_name);
	}
	else
		printf("Device OPEN SUCCESS %s\n", dev_name);

	// 시리얼 포트 환경 설정
	memset(&newtio, 0, sizeof(newtio));
	newtio.c_iflag = IGNPAR | ICRNL;
		// IGNPAR	// Parity 에러가 있는 문자 바이트를 무시	// Non-Parity
		// ICRL		// CR 문자를 NL문자로 변환 처리

	newtio.c_oflag = 0;

	newtio.c_cflag = CS8 | CLOCAL | CREAD;	// No-RTS / No-CTS

	switch( baud )
	{
		case 115200	:	newtio.c_cflag |= B115200 ;	break ;
		case 57600	:	newtio.c_cflag |= B57600 ;	break ;
		case 38400	:	newtio.c_cflag |= B38400 ;	break ;
		case 19200	:	newtio.c_cflag |= B19200 ; 	break ;
		case 9600	:	newtio.c_cflag |= B9600 ;	break ;
		default		:	newtio.c_cflag |= B115200 ; 	break ;
	}

	// Set input mode ( non-canonical, no echo ... )
	newtio.c_lflag = ICANON ;
	tcflush(fd, TCIFLUSH) ;			// 통신 수행 전 처리 되지 않은 데이터를 모두 비운다
	tcsetattr(fd, TCSANOW, &newtio) ;	// 모뎀 라인을 초기화 하고 포트 Setting을 마친다

	return fd ;
}

// 시리얼 포트를 닫는다
void close_serial( char* dev_name, int fd )
{
	close(fd) ;
	printf("Device CLOSE SUCCESS %s\n", dev_name) ;
}

void intial(int* fd)
{
	int i;
	char buf[120];
	
	sprintf(buf, "#%dP%d\r", 22 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 2 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 5 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 8 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 28 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 25 ,1700) ;
	write(*fd, buf, strlen(buf));

	sprintf(buf, "#%dP%d\r", 1 ,1600) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 21 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 4 ,1400) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 7 ,1450) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 27 ,1300) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 24 ,1550) ;
	write(*fd, buf, strlen(buf));

	sprintf(buf, "#%dP%d\r", 20 ,3000) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 3 ,1600) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 23 ,1550) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 6 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 26 ,1550) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 0 ,1550) ;
	write(*fd, buf, strlen(buf));
}

void raiseLegs_set1(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 1 ,1900) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 7 ,1800) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 24 ,1800) ;
	write(*fd, buf, strlen(buf));
}

void raiseLegs_set2(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 4 ,1800) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 27 ,1800) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 21 ,1800) ;
	write(*fd, buf, strlen(buf));
}

void lowerLegs_set1(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 1 ,1600) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 7 ,1450) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 24 ,1550) ;
	write(*fd, buf, strlen(buf));
}

void lowerLegs_set2(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 4 ,1400) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 27 ,1300) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 21 ,1500) ;
	write(*fd, buf, strlen(buf));
}

void rotateLegs_set1(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 2 ,1200) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 8 ,1200) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 25 ,1200) ;
	write(*fd, buf, strlen(buf));
}

void rotateLegs_set2(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 5 ,1100) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 28 ,1100) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 22 ,1100) ;
	write(*fd, buf, strlen(buf));
}

void revertLegs_set1(int* fd)
{
	char buf[120];
	
	sprintf(buf, "#%dP%d\r", 2 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 8 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 25 ,1700) ;
	write(*fd, buf, strlen(buf));
}

void revertLegs_set2(int* fd)
{
	char buf[120];

	sprintf(buf, "#%dP%d\r", 5 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 28 ,1500) ;
	write(*fd, buf, strlen(buf));
	sprintf(buf, "#%dP%d\r", 22 ,1500) ;
	write(*fd, buf, strlen(buf));
}

void test(int* fd)
{
	raiseLegs_set1(fd);
	usleep(250000);
	rotateLegs_set2(fd);
	usleep(250000);
	lowerLegs_set1(fd);
	usleep(250000);
	raiseLegs_set2(fd);
	usleep(250000);
	revertLegs_set2(fd);
	usleep(250000);
	lowerLegs_set2(fd);
	usleep(250000);
}

// MAIN FUNCTION
int main( int argc, char** argv )
{
	int fd ;		// 시리얼포트 파일 핸들
	int baud ;		// 전송 속도
	int rdcnt ;
	char dev_name[128] ;	// 시리얼포트 노드 파일 이름
	char buf[128] ;		// 데이터 버퍼

	// 서보모터 제어를 위한 변수 선언
	int flag = 0 ;	
	int order = 1500 ;	
	int set;
	char cmd;

	if( argc != 3 )
	{
		printf("Sample_serial [device] [baud]\n" \
			    "Device : /dev/ttySAC0 ...  \n" \
			    "Baud   : 2400 ... 115200 \n" ) ;

		return -1 ;
	}
	
	printf("Serial Test Start... (%s)\n", __DATE__) ;

	// 인자를 얻어온다.
	strcpy(dev_name, argv[1]) ;		// 노드 파일 이름
	baud = strtoul( argv[2], NULL, 10 ) ;	// 전송 속도

	// 시리얼 포트를 연다s
	fd = open_serial( dev_name, baud ) ; //, 0,x` 0 ) ;
	if( fd < 0 )
		return -2 ;

	// 서보모터 위치 초기화 ( Center )
	sprintf(buf, "#1P%d\r", order) ;
	write(fd, buf, strlen(buf));		

	/****************************************/
	/** Program Cording Area for USER or PROGRAMMER **/
	/****************************************/
	
	intial(&fd);
	sleep(1);
	/*
	while(1)
	{
		scanf("%c%d", &cmd, &set);
		if(cmd=='w' && set==1)
		{
			printf("raiseLegs_set1\n");
			raiseLegs_set1(&fd);
		}
		else if(cmd=='w' && set==2)
		{
			printf("raiseLegs_set2\n");
			raiseLegs_set2(&fd);
		}
		else if(cmd=='s' && set==1)
		{
			printf("lowerLegs_set1\n");
			lowerLegs_set1(&fd);
		}
		else if(cmd=='s' && set==2)
		{
			printf("lowerLegs_set2\n");
			lowerLegs_set2(&fd);
		}
		else if(cmd=='a' && set==1)
		{
			printf("rotateLegs_set1\n");
			rotateLegs_set1(&fd);
		}
		else if(cmd=='a' && set==2)
		{
			printf("rotateLegs_set2\n");
			rotateLegs_set2(&fd);
		}
		else if(cmd=='d' && set==1)
		{
			printf("revertLegs_set1\n");
			revertLegs_set1(&fd);
		}
		else if(cmd=='d' && set==2)
		{
			printf("revertLegs_set2\n");
			revertLegs_set2(&fd);
		}
		//printf("cmd: %d order: %d\n", leg, order);
		//sprintf(buf, "#%dP%d\r", leg, order);
		//write(fd, buf, strlen(buf));
	}
	*/
	do
	{
		flag++;
		test(&fd);	
	}
	while( flag < 100 ) ;
	usleep(1000000);
	intial(&fd);

	/****************************************/

	// 시리얼 포트를 닫는다.
	close_serial( dev_name, fd ) ;
	printf("Serial TEST END\n") ;

	return 0 ;
}

