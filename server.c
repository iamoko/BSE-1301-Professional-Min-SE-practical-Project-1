#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>
#include <mysql/mysql.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "4826";
static char *db = "recess";
#define port 4444

void sort(char *s[], int n)
{
	for (int i=1 ;i<n; i++)
	{
		char * temp = s[i];

		// Insert s[j] at its correct position
		int j = i - 1;
		while (j >= 0 && strlen(temp) < strlen(s[j]))
		{
			s[j+1] = s[j];
			j--;
		}
		s[j+1] = temp;
	}
}
void another(char strg[]);
void insert_substring(char*, char*,int);
char* substring(char*,int,int);
char *FirstCharacter(char *line);
char *LastCharacter(char *line);
char *concat(const char *s1,const char *s2);
char** Split(const char *str, char delimiter);
// Function to print the sorted array of string
void printArraystring(char *str[], int n)
{
	FILE *fr;
	fr = fopen("/var/www/html/Recess/waiting.txt", "a");
	char set[256];
	char** SplitJob;//will carry the splitted job

	for (int i=0; i<n; i++){
		SplitJob = Split(str[i],'\t');		
		strcpy(set,*(SplitJob + 1));//getting the string
		if(strcmp(set, "replace")==0){
			continue;
		}else{
			fprintf(fr,"%d\t%s\n", i+1, str[i]);
			break;
		}
		
	}

	for (int i=1; i<n; i++){
		SplitJob = Split(str[i],'\t');		
		strcpy(set,*(SplitJob + 1));//getting the string

		if(strcmp(set, "replace")==0){
			fprintf(fr,"3\t%s\n", str[i]);
		}else{
			fprintf(fr,"2\t%s\n", str[i]);
		}
		
	}
	fclose(fr);
}
int main(int argc, char const *argv[]){
	int serversocket, identification, Userid;
	struct sockaddr_in server_address;
	char message[1024];
	int newsocket, countings = 0;
	pid_t childpid;
	char** SplitJob;//will carry the splitted job
	int i,j,temp,len,  lower, upper,a,tmpn=0,fnex=0,chfound=0,stt=0,end=0;
	struct sockaddr_in newaddress;
	socklen_t addr_size;  
	FILE *fptr;
	FILE *success;
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	clock_t timo;
	char line[10][128],task[256], recieving[200], decryption[200],string[256], User_ID[256], priority[256], stored[28], commands[20][50] = {
										"double",
										"rev",
										"replace",
										"encrypt",
										"decrypt"
									};
	
	char request[256];

	serversocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serversocket<0){
		printf("Socket failed to be created");
		exit(1);
	}
    //using the socket server address
    //soocket family
    server_address.sin_family = AF_INET;
    //socket type
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //socket port
    server_address.sin_port = htons(port);
    //in client we have to connect to the server. But with the server, we have to bind the Ip address to that specific port
    int binding = bind(serversocket, (struct sockaddr *)&server_address,sizeof(server_address));
    if(binding<0){
    	printf("Server has failed to bind the Ip address to the port\n");
    	exit(1);
    }
    //Define the number of clients to be connected to that server at a time
    if(listen(serversocket, 0)==0){
    	printf("Waiting.....\n");
    }else{
    	printf("Error in listenning\n");
    }
    //create an infinite loop
    while(1){
    	newsocket = accept(serversocket, (struct sockaddr*)&newaddress, &addr_size);
		if(newsocket<0){
			exit(1);
		}
		printf("Connection Accepted From: %s :%d\n",inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));
		Userid = ntohs(newaddress.sin_port);
		if(newsocket > 0 && newsocket < 5){
			if((childpid=fork())==0){
				close(serversocket);
				while(1){
					recv(newsocket, &message, 1024, 0);
					countings++;
					// Database program to sore the data directly to the database

					MYSQL *conn;
					conn = mysql_init(NULL);
					if(strcmp(message, ":exit")==0){
						printf("\nDisconnected from: %s :%d\n",inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));

						success = fopen("/var/www/html/Recess/success.txt", "a");
						fprintf(success,"%d\t%d\n", ntohs(newaddress.sin_port), (countings-1));
						fclose(success); //Close the file

						exit(1);
					}
					char first[20];
					char last[256];
					char** SplitJob;//will carry the splitted job
				    SplitJob = Split(message,' ');
					strcpy(first,*(SplitJob));//getting the task		
					strcpy(last,*(SplitJob + 1));//getting the string
					if(strlen(last)>50){
						send(newsocket, "String is lengthy hence can't be Processed", 1024, 0);
						//Writing Data to the File
						fptr = fopen("blacklisted.txt", "a");
						fprintf(fptr,"%s\n", message);
						fclose(fptr); //Close the file
					}else{
						
						
						// Storing Command Logs to the Database Logs
					    if(!mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0)){
					        fprintf(stderr, "Error: %s\n", mysql_error(conn));
					        exit(1);
					    }else{
					    	if(strcmp(first, commands[0])==0){
								identification = 1;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Double')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(first, commands[1])==0){
								identification = 2;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Reverse')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(first, commands[2])==0){
								identification = 4;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Replace')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(first, commands[3])==0){
								identification = 5;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Encrypt')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(first, commands[4])==0){
								identification = 6;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Decrypt')", ntohs(newaddress.sin_port)+1, identification);
							}else{
								identification = 3;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Delete')", ntohs(newaddress.sin_port)+1, identification);
							}
							mysql_query(conn, request);
					    }	
						printf("\nClient: %s\n", message);
						if(first && last){
							len= strlen(last);
							//check if the first string is within the array commands
							if(strcmp(first, commands[0])==0){
								sleep(2);
								//Processing Duration
								timo = clock();//start
								char *concatenated = concat(last, last);
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;
								send(newsocket, concatenated, 1024, 0);
								//Writing Data to the File
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nDouble\n%s\n%s\n%f\n0\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 1, last, concatenated, time_taken);
								fclose(fptr); //Close the file
								printf("String doubled\n");
							}else if(strcmp(first, commands[1])==0){
								sleep(2);
								//Writing Data to the File
									fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nReverse\n%s\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 2, last);
									fclose(fptr);//close the file
								timo = clock();//start
								char *stringer = last;
								for(i=0, j=len-1; i < j; i++, j--){
									temp = stringer[i];
									stringer[i] = stringer[j];
									stringer[j] = temp;
								}	
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
								// send reversed string		
								send(newsocket, stringer, 1024, 0);
								//Writing Data to the File
									fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"%s\n%f\n0\n", stringer, time_taken);
									fclose(fptr);//close the file
								printf("String reversed\n");
							}else if(strcmp(first, commands[2])==0){
								sleep(2);
								timo = clock();//start
	 							char rest[256];
								char Str2Rep[256];//Carries the string to be replaced
								char RepTrend[256];//Carries the string with the trend of replacing
								strcpy(Str2Rep,*(SplitJob + 1));	
								strcpy(rest,*(SplitJob + 1));	
								strcpy(RepTrend,*(SplitJob + 2));
								char** RepTrendP;
								RepTrendP = Split(RepTrend,',');
								//loop to do the replacing
								for(int k=0;RepTrendP[k] != '\0';k++){
									char** tracer;
									tracer = Split(RepTrendP[k],'-');
									int p = atoi(tracer[0]);//the index to replace
									--p;
									char *rep = tracer[1];//the index to insert to the index
									*(Str2Rep + p) = *rep;
								}
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nReplace\n%s %s\n%s\n%f\n0\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 4, rest,RepTrend , Str2Rep, time_taken);
								fclose(fptr);//close the file
								send(newsocket, Str2Rep, 1024, 0);
								printf("String Replaced\n");
							}else if(strcmp(first, commands[3])==0){
								sleep(2);
								int lengths = strlen(last);
								char snums[200];
								sprintf(snums, "%d", lengths);
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nEncrypt\n%s\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 5, last);
									fclose(fptr);//close the file
								timo = clock();//start
								send(newsocket, snums, 1024, 0);
								for(int m = 0; ( m < 100 && last[m] != '\0'); m++){
								    if (islower(last[m])){ 
								        lower = last[m] + '\0' - 96;
								        int lowerer = lower + 95;
								        int morelower = lower - 9;
							            int lowi = morelower + 95;
							            int plow = lowi - 8;
							            //changing integer to character
							            char newty = lowi + '\0';
							            char nw = plow + '\0';
							            //from charcacter too string
							            char str3[2] = {newty, '\0'};
								        char str4[5] = "";
								        strcpy(str4, str3); // created string
								        //from charcacter too string
							            char str5[2] = {toupper(nw), '\0'};
							            char str6[5] = "";
							            strcpy(str6, str5); // created string
							            //converting integer to string
							            char snum[5];
							            sprintf(snum, "%d", morelower);
							            //converting the character to string
								        char peop = lowerer + '\0';
							            char str1[2] = {peop, '\0'};
							            char str2[5] = "";
							            strcpy(str2, str1);
							            char *cadinate = concat(strcpy(str6, str5),strcpy(str4, str3));
							            insert_substring(snum, cadinate, 2);
							            //concatenating the two strings
							            char *concatenated = concat(strcpy(str2, str1), snum);
							            int lenth = snprintf(NULL, 0,"%d", lower);
							            char *low = malloc(lenth + 1);
							            snprintf(low, lenth + 1, "%d", lower);
							            if(lower>9){
							                if(lower > 10){
							                    if(lower > 10 && lower < 19){
													morelower +=96;
								                    char hmm = morelower + '\0';
								                    char str7[2] = {toupper(hmm), '\0'};
								                    char str8[5] = "";
								                    strcpy(str8, str7); // created string
								                    cadinate = concat(strcpy(str2, str1), strcpy(str8, str7));
							                        insert_substring(low, cadinate, 2);
						                   		}else{  
	                       							insert_substring(low, concatenated, 2);
	               								} 
							                }else{
							                    insert_substring(low, strcpy(str2, str1), 2);
							                }
							            }else{     	
								            int pinp = lower + 96;
								           	char nami = pinp + '\0';
								           	char str20[2] = {toupper(nami), '\0'};
								            char str21[5] = "";
								            strcpy(str21, str20); // created string
							                strcpy(low, strcpy(str21, str20));
							            }
							              //prints output
								            fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a"); 
											fprintf(fptr, "%s", low);
											fclose(fptr);//close the file
							            //prints output
							           send(newsocket, low, 1024, 0);	
							        }else if(isupper(last[m])){
							            upper = last[m] + '\0' - 38; //A - 27
							            int upperer = upper + 95;
								        int upt = upper - 9; //A 
								        int apt =  upt + 95; //
							            int atp = apt - 8;
							            char lol = apt + '\0';
							            char mindo = atp + '\0';
							            //from charcacter too string
							            char str3[2] = {lol, '\0'};
							            char str4[5] = "";
							            strcpy(str4, str3); // created string
								        //from charcacter too string
								        char str5[2] = {toupper(mindo), '\0'};
								        char str6[5] = "";
							            strcpy(str6, str5); // created string
							            //converting integer to string
							            char snum1[5];
							            sprintf(snum1, "%d", upt);
							            int luna = snprintf(NULL, 0,"%d", upper);
							            char *up = malloc(luna + 1);
							            snprintf(up, luna + 1, "%d", upper);
							            char *cadinate = concat(strcpy(str6, str5),strcpy(str4, str3));    
							            //concatenating the two strings
							            char *anoth;
										if(upper > 27){
							            	int uppererd = upperer - 58;
							            	char peopy = uppererd + '\0';
							                char str96[2] = {tolower(peopy), '\0'};
							                char str95[5] = "";
							                strcpy(str95, str96);
							               if(upt > 9){
								               	int ler = atp + 8;
								           		lol = apt + '\0';
								           		int laste = upt - 9;
								           		char lasted[20];
									            sprintf(lasted, "%d", laste);
									           	if(laste >9){
										           	int mordt = laste +95;
										           	char piper = mordt + '\0';
										       		int kil = laste - 9+96;
									          		char kill = kil + '\0';
									           		char kio[2] = {toupper(kill), '\0'};
													char kiod[5] = "";
													strcpy(kiod, kio);
									            	char pol[2] = {piper, '\0'};
													char poly[5] = "";
													strcpy(poly, pol);
									            	char *kop = concat(strcpy(kiod, kio),strcpy(poly, pol));
										           	insert_substring(lasted, kop, 2);
										           	char *der = concat(strcpy(str4, str3),lasted);
										           	insert_substring(snum1, der, 2);
										           	anoth = concat(strcpy(str95, str96), snum1);	
								            	}else{
								            		char *cadinate = concat(strcpy(str4, str3),lasted);
								            		strcpy(snum1,strcpy(str4, str3));
								            	}
							                }
							            }else{
							                //converting the character ti string
							                insert_substring(snum1, cadinate, 2);
							            	//concatenating the two strings
							                char peop = upperer + '\0';
							                char str1[2] = {peop, '\0'};
								            char str2[5] = "";
								            strcpy(str2, str1);
								            //concatenating the two strings
							                anoth = concat(strcpy(str2, str1), snum1);
							            }
							            insert_substring(up, anoth, 2);
							            fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a"); 
											fprintf(fptr, "%s", up);
											fclose(fptr);//close the file
							            send(newsocket, up, 1024, 0);	
							        }
								}
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
							    fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"\n%f\n0\n",time_taken);
								fclose(fptr);//close the file
							    printf("String Encrypted\n");
							}else if(strcmp(first, commands[4])==0){
								sleep(2);
								timo = clock();//start
								char stringed[54][18],characterss[]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
								strcpy(stringed[0],"}"); strcpy(stringed[1],"A"); strcpy(stringed[2],"B"); strcpy(stringed[3],"C"); strcpy(stringed[4],"D");
								strcpy(stringed[5],"E"); strcpy(stringed[6],"F"); strcpy(stringed[7],"G"); strcpy(stringed[8],"H"); strcpy(stringed[9],"I");
								strcpy(stringed[10],"1iA0"); strcpy(stringed[11],"1jB1"); strcpy(stringed[12],"1kC2"); strcpy(stringed[13],"1lD3");
								strcpy(stringed[14],"1mE4"); strcpy(stringed[15],"1nF5"); strcpy(stringed[16],"1oG6"); strcpy(stringed[17],"1pH7");
								strcpy(stringed[18],"1qI8"); strcpy(stringed[19],"1r1Ai09");  strcpy(stringed[20],"2s1Bj10"); strcpy(stringed[21],"2t1Ck21");
								strcpy(stringed[22],"2u1Dl32");  strcpy(stringed[23],"2v1Em43"); strcpy(stringed[24],"2w1Fn54"); strcpy(stringed[25],"2x1Go65");
								strcpy(stringed[26],"2y1Hp76"); strcpy(stringed[27],"2z1Iq87"); strcpy(stringed[28],"2a1r1Ai098"); strcpy(stringed[29],"2b2s1Bj109");
								strcpy(stringed[30],"3c2t1Ck210"); strcpy(stringed[31],"3d2u1Dl321"); strcpy(stringed[32],"3e2v1Em432"); strcpy(stringed[33],"3f2w1Fn543");
								strcpy(stringed[34],"3g2x1Go654");  strcpy(stringed[35],"3h2y1Hp765"); strcpy(stringed[36],"3i2z1Iq876"); strcpy(stringed[37],"3j2a1rA1i0987");
								strcpy(stringed[38],"3k2b2sB1j1098"); strcpy(stringed[39],"3l3c2tC1k2109"); strcpy(stringed[40],"4m3d2uD1l3210"); strcpy(stringed[41],"4n3e2vE1m4321"); strcpy(stringed[42],"4o3f2wF1n5432"); strcpy(stringed[43],"4p3g2xG1o6543");
								strcpy(stringed[44],"4q3h2yH1p7654"); strcpy(stringed[45],"4r3i2zI1q8765"); strcpy(stringed[46],"4s3j2a1rA1i09876"); strcpy(stringed[47],"4t3k2b2sB1j10987");
								strcpy(stringed[48],"4u3l3c2tC1k21098"); strcpy(stringed[49],"4v4m3d2uD1l32109"); strcpy(stringed[50],"5w4n3e2vE1m43210");
								strcpy(stringed[51],"5x4o3f2wF1n54321"); strcpy(stringed[52],"5y4p3g2xG1o65432");
								do{
								    tmpn=0;
							        for(i=stt;i<=end;i++){ 
							            sprintf(&stored[tmpn],"%c",last[i]); 
							            tmpn++; 
							        }
							        for(a=1;a<=52;a++){ if(!strcmp(stored,stringed[a])){ sprintf(&decryption[fnex],"%c",characterss[a]);  fnex++; chfound=1; stt=end+1; 
							            end=end+1; 
							            another(stored); 
							            break;
							        } 
							    }
					            if(chfound==0){end=end+3; }else{chfound=0;}
						            if(last[end]==' ' || end >= strlen(last)){ 
						            	break; 
						            }
							    }while(chfound==0);
							    	send(newsocket, decryption, 1024, 0);
							    	printf("String Decrypted\n");
							    	//Stop Counting Processing time for execution of program
									timo = clock()-timo;
									double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
							    	fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nDecrypt\n%s\n%s\n%f\n0\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 6, last, decryption, time_taken);
									fclose(fptr);//close the file
							}else{
								timo = clock();//start
								// The last character
								char *prt = LastCharacter(last);
								//First Character
								char *ptr = FirstCharacter(last);
								int dec = 0;
								for(i=0; i<strlen(ptr); i++){
									dec = dec * 10 + ( ptr[i] - '0' );
								}
								dec = dec - 1;
								// Selecting the First and last Characters
								char *ward = malloc(strlen(first + 1));
								strcpy(ward, first);
								for(dec; dec<strlen(first); dec++){
									char *letter = ward + dec;
									char tempt;
									tempt = *(letter + 1);
									*letter = tempt;
								}
								int dim = 0;
								for(i=0; i<strlen(prt); i++){
									dim = dim * 10 + ( prt[i] - '0' );
								}
								dim = dim-2;
								for(dim; dim<strlen(first); dim++){
									char *letter = ward + dim;
									char tempt;
									tempt = *(letter + 1);
									*letter = tempt;
								}
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
								// send deleted characters	
								send(newsocket, ward, 1024, 0);	
								//Writing Data to the File
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%d\n%d:%d:%d\n%d-%d-%d\n%d\nDelete\n%s\n%s\n%f\n0\n",ntohs(newaddress.sin_port)+1,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 3, first, ward, time_taken);
								fclose(fptr);//close the file
								printf("Specified characters Deleted\n");
							}
						}
					}
				}
			}
		}else if(newsocket > 4){
			if((childpid=fork())==0){
				FILE *failure;
				close(serversocket);
				int busys = 0;
				while(1){
					recv(newsocket, &message, 1024, 0);

					//Exit the server's connection
					if(strcmp(message, ":exit")==0){
						printf("\nDisconnected from: %s :%d\n",inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));


						failure = fopen("/var/www/html/Recess/failure.txt", "a");
						fprintf(failure,"%d\t%d\n", ntohs(newaddress.sin_port), (busys));
						fclose(failure); //Close the file

						exit(1);
					}else{
					//The processing of the busylist file
					if(strcmp(message, ":check")==0){
						printf("Please wait as we Process the busylist\n");
						FILE *ft = NULL;
						FILE *fr;
						char *buf, c;
						size_t sz;

						
						//Processes the waiting list file and store results into ready jobs
						i = 0;
						fr = fopen("/var/www/html/Recess/waiting.txt", "r");
						while(fgets(line[i], 128, fr)){
							line[i][strlen(line[i]) - 1] = '\0';
							i++;
						} 
						if(i<3){
							printf("File small\n");
							goto B;
						}else{
						int total = 3;
						for(i = 0; i < total; i++){
							SplitJob = Split(line[i],'\t');
							strcpy(User_ID,*(SplitJob + 1));//getting the task
							strcpy(priority,*(SplitJob));//getting the task
							strcpy(task,*(SplitJob + 2));//getting the task
							strcpy(string,*(SplitJob + 3));//getting the string
							if(strcmp(task, "double")==0){
								//Processing Duration
								timo = clock();//start
								char *concatenated = concat(string, string);
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;
								printf("%s\n", concatenated);
								send(newsocket, concatenated, 1024, 0);
								
								//Writing Data to the File
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%s\n%d:%d:%d\n%d-%d-%d\n%d\nDouble\n%s\n%s\n%f\n%s\n", User_ID,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 1, string, concatenated, time_taken, priority);
								fclose(fptr); //Close the file
								printf("String doubled\n");
							}else if(strcmp(task, "rev")==0){
								int j, m;
								//Writing Data to the File
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%s\n%d:%d:%d\n%d-%d-%d\n%d\nReverse\n%s\n",User_ID,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 2, string);
								fclose(fptr);//close the file
								timo = clock();//start
								char *stringer = string;
								for(m=0, j=strlen(string)-1; m < j; m++, j--){
									temp = stringer[m];
									stringer[m] = stringer[j];
									stringer[j] = temp;
								}	
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
								//Writing Data to the File
								send(newsocket, stringer, 1024, 0);
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%s\n%f\n%s\n", stringer, time_taken, priority);
								fclose(fptr);//close the file
								printf("String reversed\n");
							}else if(strcmp(task, "replace")==0){

								timo = clock();//start
								char **moresplit;
								moresplit = Split(string,' ');
						 		char rest[256];
								char Str2Rep[256];//Carries the string to be replaced
								char RepTrend[256];//Carries the string with the trend of replacing
								strcpy(Str2Rep,*(moresplit));	
								strcpy(rest,*(moresplit));	
								strcpy(RepTrend,*(moresplit + 1));
								char** RepTrendP;
								RepTrendP = Split(RepTrend,',');
								//loop to do the replacing
								for(int k=0;RepTrendP[k] != '\0';k++){
									char** tracer;
									tracer = Split(RepTrendP[k],'-');
									int p = atoi(tracer[0]);//the index to replace
									--p;
									char *rep = tracer[1];//the index to insert to the index
									*(Str2Rep + p) = *rep;
								}
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;

								send(newsocket, Str2Rep, 1024, 0);
								fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
								fprintf(fptr,"%s\n%d:%d:%d\n%d-%d-%d\n%d\nReplace\n%s\n%s\n%f\n%s\n", User_ID,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 4, rest, Str2Rep, time_taken, priority);
								fclose(fptr);//close the file			
								printf("String Replaced\n");
							}else if(strcmp(task, "encrypt")==0){

			fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
			fprintf(fptr,"%s\n%d:%d:%d\n%d-%d-%d\n%d\nEncrypt\n%s\n",User_ID,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 5, string);
			fclose(fptr);//close the file
			timo = clock();//start
			for(int m = 0; ( m < 100 && string[m] != '\0'); m++){
								    if (islower(string[m])){ 
								        lower = string[m] + '\0' - 96;
								        int lowerer = lower + 95;
								        int morelower = lower - 9;
							            int lowi = morelower + 95;
							            int plow = lowi - 8;
							            //changing integer to character
							            char newty = lowi + '\0';
							            char nw = plow + '\0';
							            //from charcacter too string
							            char str3[2] = {newty, '\0'};
								        char str4[5] = "";
								        strcpy(str4, str3); // created string
								        //from charcacter too string
							            char str5[2] = {toupper(nw), '\0'};
							            char str6[5] = "";
							            strcpy(str6, str5); // created string
							            //converting integer to string
							            char snum[5];
							            sprintf(snum, "%d", morelower);
							            //converting the character to string
								        char peop = lowerer + '\0';
							            char str1[2] = {peop, '\0'};
							            char str2[5] = "";
							            strcpy(str2, str1);
							            char *cadinate = concat(strcpy(str6, str5),strcpy(str4, str3));
							            insert_substring(snum, cadinate, 2);
							            //concatenating the two strings
							            char *concatenated = concat(strcpy(str2, str1), snum);
							            int lenth = snprintf(NULL, 0,"%d", lower);
							            char *low = malloc(lenth + 1);
							            snprintf(low, lenth + 1, "%d", lower);
							            if(lower>9){
							                if(lower > 10){
							                    if(lower > 10 && lower < 19){
													morelower +=96;
								                    char hmm = morelower + '\0';
								                    char str7[2] = {toupper(hmm), '\0'};
								                    char str8[5] = "";
								                    strcpy(str8, str7); // created string
								                    cadinate = concat(strcpy(str2, str1), strcpy(str8, str7));
							                        insert_substring(low, cadinate, 2);
						                   		}else{  
	                       							insert_substring(low, concatenated, 2);
	               								} 
							                }else{
							                    insert_substring(low, strcpy(str2, str1), 2);
							                }
							            }else{     	
								            int pinp = lower + 96;
								           	char nami = pinp + '\0';
								           	char str20[2] = {toupper(nami), '\0'};
								            char str21[5] = "";
								            strcpy(str21, str20); // created string
							                strcpy(low, strcpy(str21, str20));
							            }
							              //prints output
								            fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a"); 
											fprintf(fptr, "%s", low);
											fclose(fptr);//close the file
							            //prints output
											 }else if(isupper(string[m])){
							            upper = string[m] + '\0' - 38; //A - 27
							            int upperer = upper + 95;
								        int upt = upper - 9; //A 
								        int apt =  upt + 95; //
							            int atp = apt - 8;
							            char lol = apt + '\0';
							            char mindo = atp + '\0';
							            //from charcacter too string
							            char str3[2] = {lol, '\0'};
							            char str4[5] = "";
							            strcpy(str4, str3); // created string
								        //from charcacter too string
								        char str5[2] = {toupper(mindo), '\0'};
								        char str6[5] = "";
							            strcpy(str6, str5); // created string
							            //converting integer to string
							            char snum1[5];
							            sprintf(snum1, "%d", upt);
							            int luna = snprintf(NULL, 0,"%d", upper);
							            char *up = malloc(luna + 1);
							            snprintf(up, luna + 1, "%d", upper);
							            char *cadinate = concat(strcpy(str6, str5),strcpy(str4, str3));    
							            //concatenating the two strings
							            char *anoth;
										if(upper > 27){
							            	int uppererd = upperer - 58;
							            	char peopy = uppererd + '\0';
							                char str96[2] = {tolower(peopy), '\0'};
							                char str95[5] = "";
							                strcpy(str95, str96);
							               if(upt > 9){
								               	int ler = atp + 8;
								           		lol = apt + '\0';
								           		int laste = upt - 9;
								           		char lasted[20];
									            sprintf(lasted, "%d", laste);
									           	if(laste >9){
										           	int mordt = laste +95;
										           	char piper = mordt + '\0';
										       		int kil = laste - 9+96;
									          		char kill = kil + '\0';
									           		char kio[2] = {toupper(kill), '\0'};
													char kiod[5] = "";
													strcpy(kiod, kio);
									            	char pol[2] = {piper, '\0'};
													char poly[5] = "";
													strcpy(poly, pol);
									            	char *kop = concat(strcpy(kiod, kio),strcpy(poly, pol));
										           	insert_substring(lasted, kop, 2);
										           	char *der = concat(strcpy(str4, str3),lasted);
										           	insert_substring(snum1, der, 2);
										           	anoth = concat(strcpy(str95, str96), snum1);	
								            	}else{
								            		char *cadinate = concat(strcpy(str4, str3),lasted);
								            		strcpy(snum1,strcpy(str4, str3));
								            	}
							                }
							            }else{
							                //converting the character ti string
							                insert_substring(snum1, cadinate, 2);
							            	//concatenating the two strings
							                char peop = upperer + '\0';
							                char str1[2] = {peop, '\0'};
								            char str2[5] = "";
								            strcpy(str2, str1);
								            //concatenating the two strings
							                anoth = concat(strcpy(str2, str1), snum1);
							            }
							            insert_substring(up, anoth, 2);
							            fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a"); 
											fprintf(fptr, "%s", up);
											fclose(fptr);//close the file
											}
								}
								//Stop Counting Processing time for execution of program
								timo = clock()-timo;
								double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
							    fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									fprintf(fptr,"\n%f\n%s\n",time_taken, priority);
								fclose(fptr);//close the file
							    printf("String Encrypted\n");

		}else if(strcmp(task, "decrypt")==0){
			timo = clock();//start
			int n = 0, a,tmpns=0,fnexs=0,chfounds=0,stts=0,end=0;
								char encryption[54][18],charactersss[]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
								strcpy(encryption[0],"}"); strcpy(encryption[1],"A"); strcpy(encryption[2],"B"); strcpy(encryption[3],"C"); strcpy(encryption[4],"D");
								strcpy(encryption[5],"E"); strcpy(encryption[6],"F"); strcpy(encryption[7],"G"); strcpy(encryption[8],"H"); strcpy(encryption[9],"I");
								strcpy(encryption[10],"1iA0"); strcpy(encryption[11],"1jB1"); strcpy(encryption[12],"1kC2"); strcpy(encryption[13],"1lD3");
								strcpy(encryption[14],"1mE4"); strcpy(encryption[15],"1nF5"); strcpy(encryption[16],"1oG6"); strcpy(encryption[17],"1pH7");
								strcpy(encryption[18],"1qI8"); strcpy(encryption[19],"1r1Ai09");  strcpy(encryption[20],"2s1Bj10"); strcpy(encryption[21],"2t1Ck21");
								strcpy(encryption[22],"2u1Dl32");  strcpy(encryption[23],"2v1Em43"); strcpy(encryption[24],"2w1Fn54"); strcpy(encryption[25],"2x1Go65");
								strcpy(encryption[26],"2y1Hp76"); strcpy(encryption[27],"2z1Iq87"); strcpy(encryption[28],"2a1r1Ai098"); strcpy(encryption[29],"2b2s1Bj109");
								strcpy(encryption[30],"3c2t1Ck210"); strcpy(encryption[31],"3d2u1Dl321"); strcpy(encryption[32],"3e2v1Em432"); strcpy(encryption[33],"3f2w1Fn543");
								strcpy(encryption[34],"3g2x1Go654");  strcpy(encryption[35],"3h2y1Hp765"); strcpy(encryption[36],"3i2z1Iq876"); strcpy(encryption[37],"3j2a1rA1i0987");
								strcpy(encryption[38],"3k2b2sB1j1098"); strcpy(encryption[39],"3l3c2tC1k2109"); strcpy(encryption[40],"4m3d2uD1l3210"); strcpy(encryption[41],"4n3e2vE1m4321"); strcpy(encryption[42],"4o3f2wF1n5432"); strcpy(encryption[43],"4p3g2xG1o6543");
								strcpy(encryption[44],"4q3h2yH1p7654"); strcpy(encryption[45],"4r3i2zI1q8765"); strcpy(encryption[46],"4s3j2a1rA1i09876"); strcpy(encryption[47],"4t3k2b2sB1j10987");
								strcpy(encryption[48],"4u3l3c2tC1k21098"); strcpy(encryption[49],"4v4m3d2uD1l32109"); strcpy(encryption[50],"5w4n3e2vE1m43210");
								strcpy(encryption[51],"5x4o3f2wF1n54321"); strcpy(encryption[52],"5y4p3g2xG1o65432");
								do{
								    tmpns=0;
							        for(n=stts;n<=end;n++){ 
							            sprintf(&stored[tmpns],"%c",string[n]); 
							            tmpns++; 
							        }
							        for(a=1;a<=52;a++){ if(!strcmp(stored,encryption[a])){ sprintf(&decryption[fnexs],"%c",charactersss[a]);  fnexs++; chfounds=1; stts=end+1; 
							            end=end+1; 
							            another(stored); 
							            break;
							        } 
							    }
					            if(chfounds==0){end=end+3; }else{chfounds=0;}
						            if(string[end]==' ' || end >= strlen(string)){ 
						            	break; 
						            }
							    }while(chfounds==0);
							    	printf("String Decrypted\n");
							    	//Stop Counting Processing time for execution of program
									timo = clock()-timo;
									double time_taken = ((double)timo)/CLOCKS_PER_SEC;	
							    	fptr = fopen("/var/www/html/Recess/ready_jobs.txt", "a");
									send(newsocket, decryption, 1024, 0);
									fprintf(fptr,"%s\n%d:%d:%d\n%d-%d-%d\n%d\nDecrypt\n%s\n%s\n%f\n%s\n", User_ID,timeinfo->tm_hour,timeinfo->tm_min, timeinfo->tm_sec,timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday, 6, string, decryption, time_taken, priority);
									fclose(fptr);//close the file

							}
							printf("%s\t%s\t%s\n",priority, task, string);
						}
						fclose(fr);

						//Now delete the data from that file
						
						int start = 1, count = total, lines = 1, dest = 0 , src = 0 , pos = - 1 ;
						fr = fopen ( "/var/www/html/Recess/waiting.txt" , "r" );
						fseek ( fr, 0 , SEEK_END ) ;
						sz = ftell( fr ) ;
						buf = malloc ( sz + 1 ) ;
						rewind ( fr ) ;
						/* Fill the buffer, count lines, and remember a few important offsets. */
						while (( buf [ ++pos ] = fgetc ( fr )) != EOF ) {
							if ( buf [ pos ] == '\n' ) {
								++lines;
								if ( lines == start ) dest = pos + 1 ;
								if ( lines == start + count ) src = pos + 1 ;
							}
						}

						if ( start + count > lines ) {
							free ( buf ) ;
							fclose ( fr );
							printf("File Empty\n");
							goto B;
						}

						/* Overwrite the lines to be deleted with the survivors. */
						memmove ( buf + dest, buf + src, pos - src );
						/* Reopen the file and write back just enough of the buffer. */
						freopen( "/var/www/html/Recess/waiting.txt" , "w" , fr ) ;
						fwrite (buf, pos - src + dest, 1 , fr );
						free (buf);
						fclose (fr);
						}

					    B:
						printf("Continue\n");
						send(newsocket, "Done", 1024, 0);
					}else{
						busys++;
						printf("Server busy\n");
						char last[256];
						char lasted[256];
					    SplitJob = Split(message,' ');	
						//store logs into the database
						MYSQL *conn;
						conn = mysql_init(NULL);
						// Storing Command Logs to the Database Logs
						if(!mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0)){
					        fprintf(stderr, "Error: %s\n", mysql_error(conn));
					        exit(1);
					    }else{
					    	if(strcmp(*(SplitJob), commands[0])==0){
								identification = 1;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Double')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(*(SplitJob), commands[1])==0){
								identification = 2;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Reverse')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(*(SplitJob), commands[2])==0){
								identification = 4;
									sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Replace')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(*(SplitJob), commands[3])==0){
								identification = 5;
									sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Encrypt')", ntohs(newaddress.sin_port)+1, identification);
							}else if(strcmp(*(SplitJob), commands[4])==0){
								identification = 6;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Decrypt')", ntohs(newaddress.sin_port)+1, identification);
							}else{
								identification = 3;
								sprintf(request , "INSERT INTO Logs(User_ID, Task_ID, Time, Date, Task) values(%d, %d, CURRENT_TIME(), CURRENT_DATE(), 'Delete')", ntohs(newaddress.sin_port)+1, identification);
							}
							mysql_query(conn, request);
					    }
						printf("Server is still busy\n");
						send(newsocket, "Server is Still busy", 1024, 0);
						//Writing Data to the File
						fptr = fopen("/var/www/html/Recess/busy_list.txt", "a");
						fprintf(fptr,"%d\t%s\t%s", Userid, *(SplitJob), *(SplitJob + 1));
						if(strcmp(*(SplitJob),"replace")==0){
							fprintf(fptr," %s", *(SplitJob + 2));
						}
						fprintf(fptr,"\n");
						fclose(fptr); //Close the file
						//Reads data from the Busylist and assign priorities to each line
						int start = 1, count = 3, lines = 1 ,num = 0, dest = 0 , src = 0 , pos = - 1 ;
						FILE *ft = NULL;
						char *buf, c;
						size_t sz;
						int minds = 0;
						ft = fopen("/var/www/html/Recess/busy_list.txt", "r");
						for (c = getc(ft); c != EOF; c = getc(ft)){
					        if (c == '\n'){ // Increment count if this character is newline
					            minds = minds + 1;
					        }
					    }
					    fclose(ft);
					    if( minds <3){
					    	printf("file small\n");
					    }else{
					    	i = 0;
					    	ft = fopen("/var/www/html/Recess/busy_list.txt", "r");
							while(fgets(line[i], 128, ft)){
								line[i][strlen(line[i]) - 1] = '\0';
								i++;
							} 
							//reads strings from file to pointer
					    	char *arr[] = {line[0], line[1], line[2]};
							int n = sizeof(arr)/sizeof(arr[0]);
							// Function to perform sorting
							sort(arr, n);
							// Calling the function to print result
							printArraystring(arr, n);

							 //Deletes 9 lines from busylist file
							ft = fopen ( "/var/www/html/Recess/busy_list.txt" , "r" );
							fseek ( ft, 0 , SEEK_END ) ;
							sz = ftell( ft ) ;
							buf = malloc ( sz + 1 ) ;
							rewind ( ft ) ;

							/* Fill the buffer, count lines, and remember a few important offsets. */
							while (( buf [ ++pos ] = fgetc ( ft )) != EOF ) {
								if ( buf [ pos ] == '\n' ) {
									++lines;
									if ( lines == start ) dest = pos + 1 ;
									if ( lines == start + count ) src = pos + 1 ;
								}
							}
							if ( start + count > lines) {
								free ( buf ) ;
								fclose ( ft );
								printf("File Empty\n");
							}
							/* Overwrite the lines to be deleted with the survivors. */
							memmove ( buf + dest, buf + src, pos - src );
							/* Reopen the file and write back just enough of the buffer. */
							freopen( "/var/www/html/Recess/busy_list.txt" , "w" , ft ) ;
							fwrite (buf, pos - src + dest, 1 , ft );
							free (buf);
							fclose (ft);
					    }
					}
					}
						
				}		
			}
		}
	}
	close(newsocket);
	return 0;
}
void another(char strg[]){ 
    for(int i=0;i<strlen(strg);i++){ 
        strg[i]='\0'; 
    } 
}
void insert_substring(char *a, char *b, int position){
    char *f, *e;
    int length;
    length = strlen(a);
    f = substring(a,1,position - 1);
    e = substring(a, position, length - position + 1);
    strcpy(a,"");
    strcat(a,f);
    free(f);
    strcat(a,b);
    strcat(a,e);
    free(e);
}
char *substring(char *string, int position, int length){
    char *pointer;
    int c;
    pointer = malloc(length + 1);
    if(pointer == NULL){
        exit(EXIT_FAILURE);
    }
    for(c=0;c<length;c++){
        *(pointer + c) = *((string + position - 1) + c);
    }
    *(pointer + c) = '\0';
    return pointer;
}
//Function for getting the First character before ','
char *FirstCharacter(char *line){
		char *character = strtok(line,",");
		return character;
}
//Function for getting the Last character after ','
char *LastCharacter(char *line){
		char *wipe = strrchr(line,',')+1;
		return wipe;
}
// Function to concatenate the strings entered by the User
char *concat(const char *s1, const char *s2){
	char *result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}
char** Split(const char *str, char delimiter){
	int len, i, j;
	char* buf;
	char** ret;
	len = strlen(str);
	buf = malloc(len + 1);
	memcpy(buf, str, len + 1);
	j = 1;
	for (i = 0; i < len; ++i){
		if (buf[i] == delimiter){
			while (buf[i + 1] == delimiter) i++;
			j++;
		}
	}
	ret = malloc(sizeof(char*) * (j + 1));
	ret[j] = NULL;
	ret[0] = buf;
	j = 1;
	for (i = 0; i < len; ++i){
		if (buf[i] == delimiter){
			buf[i] = '\0';
			while (buf[i + 1] == delimiter) i++;
			ret[j++] = &buf[i + 1];
		}
	}
	return ret;
}