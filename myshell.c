#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define ERR_EXIT( msg ) \
	do { 		\
		printf("[%s][%d] %s:%s\n" , __FILE__ , __LINE__ , msg , strerror(errno)) ; \
		exit( EXIT_FAILURE ) ; \
	} while ( 0 )
int get_input( char *buf) {
		memset( buf , 0x00 , sizeof(buf) ) ;
		if ( scanf("%[^\n]%*c",buf) != 1 ) {
			int c ;
			do c=getchar() ; while( c != '\n' ) ;
		}
		return strlen( buf ) ;
}

void do_action( int argc , char* argv[] ) {
	// 子进程运行，子进程不结束，父进程一直处于等待状态
	pid_t pid ;
	switch ( pid = fork() ) {
		case -1 :
			printf("当前shell正在维护~~~~\n") ;
		//	ERR_EXIT( "fork" ) ;
			return ;
		case 0 : {
			int ret ; 
			ret = execvp( argv[0] , argv) ;// 子进程中用argv[0],也就是输入命令的第一个参数替子进程的空间
			printf("%s:command not found\n",argv[0]) ;
			exit( 1 ) ;
		}
		break ;
		default : {
			int status ;
			waitpid( pid , &status , 0 ) ;//父进程等待子进程死亡
		}
		break ;
	}
}	
void do_parse( char* buf) {
	int argc = 0 ;
	char* argv[10] ;
	#define OUT 0
	#define IN 1
	
	int status = OUT ;
	int i ;
	
	for ( i=0 ; buf[i] !='\0' ; i++ ) {
		if ( status == OUT && !isspace(buf[i]) ) {
			argv[argc] = buf + i ;
			argc++ ;
			status = IN ;
		}
		if ( status == IN && isspace(buf[i]) ) {
			status = OUT ;
			buf[i] = '\0' ;
		}
	}
/*
	for( i = 0 ;i<argc;i++) {
		printf("%s\n",*(argv+i)) ;
	}
*/
	argv[argc] = NULL ;
	do_action( argc , argv ) ;
}
int main( void ) {
	char buf[512] ;
	while ( 1 ) {
		printf("[zhenghong@shell] #") ;
		if ( get_input( buf ) ==0 ) // 读取一行数据
			continue ;
		do_parse( buf ) ;// 解析数据
//		printf("=>[%s]\n",buf) ;
	}
	return 0 ;
}
