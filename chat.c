#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>


#define FFNAME12 "fifo1to2"
#define FFNAME21 "fifo2to1"

int main(int argc, char *argv[]) {

  int fd_read12, fd_read21, pid, pnum, anum, bytes_read, bytes_written, c=0;
  char buffer[BUFSIZ + 1], input[BUFSIZ + 1];
  bool per1 = false, per2 = false, per3 = false;
  memset(input, '\0', sizeof(input));
  memset(buffer, '\0', sizeof(buffer));

  // Create the FIFO for writing and reading from 1 to 2
  if (mkfifo(FFNAME12, 0666) == -1) {
      perror("mkfifo fails");
      exit(EXIT_FAILURE);
  }

  // Create the FIFO for writing and reading from 2 to 1
  if (mkfifo(FFNAME21, 0666) == -1) {
      perror("mkfifo fails");
      exit(EXIT_FAILURE);
  }

  // Open the FIFO for reading and writing from 1 to 2
  if ((fd_read12 = open(FFNAME12, O_RDWR)) == -1) {
      perror("open fails");
      exit(EXIT_FAILURE);
  }

  // Open the FIFO for reading and writing from 2 to 1
  if ((fd_read21 = open(FFNAME21, O_RDWR)) == -1) {
      perror("open fails");
      exit(EXIT_FAILURE);
  }
 

      printf("Enter the number of a process (1/2) or type 'end chat'to end the program: ");
      if (fgets(input, BUFSIZ, stdin) == NULL) {
        fprintf(stderr, "Error reading input from stdin\n");
        exit(EXIT_FAILURE);
      }
      puts("");
  
      // Check if the input is "end chat"
      if (strcmp(input, "end chat") == 0) {
          return 0;
      }
  
      pnum = atoi(input);
  
      if (pnum == 1){
        per1 = true;
        per2 = false;
      }else if (pnum ==2){
        per1 = false;
        per2 = true;
      }
      
      memset(input, '\0', sizeof(input));
      printf("Enter the action number (1 for read and 2 for write): ");
      if (fgets(input, BUFSIZ, stdin) == NULL) {
        fprintf(stderr, "Error reading input from stdin\n");
        exit(EXIT_FAILURE);
      }
      anum = atoi(input);
      memset(input, '\0', sizeof(input));
      puts("");
  

        pid = fork();
        if (pid == -1){
        fprintf(stderr, "Fork failed!\n");
        exit(0);
        }
      
    

    
while(1){

  if (c>0){
     printf("Enter the number of a process (1/2) or type 'end chat'to end the program: ");
      if (fgets(input, BUFSIZ, stdin) == NULL) {
        fprintf(stderr, "Error reading input from stdin\n");
        exit(EXIT_FAILURE);
      }
      puts("");
  
      // Check if the input is "end chat"
      if (strcmp(input, "end chat") == 0) {
          break;
      }
  
      pnum = atoi(input);
  
      if (pnum == 1){
        per1 = true;
        per2 = false;
      }else if (pnum ==2){
        per1 = false;
        per2 = true;
      }
      
      memset(input, '\0', sizeof(input));
      printf("Enter the action number (1 for read and 2 for write): ");
      if (fgets(input, BUFSIZ, stdin) == NULL) {
        fprintf(stderr, "Error reading input from stdin\n");
        exit(EXIT_FAILURE);
      }
      anum = atoi(input);
      memset(input, '\0', sizeof(input));
      puts("");
  c=0;
  }
  
// ============================
    if(per2 == true && per1 == false){
      if (pid == 0 && pnum == 2){
        if (anum == 1){
          printf("Process 2 reading from process 1: \n");
          // Read from the FIFO until there is no more data available
          while ((bytes_read = read(fd_read12, buffer, BUFSIZ)) > 0) {
              printf("%d bytes have been read: %s\n",bytes_read, buffer);
              memset(buffer, '\0', sizeof(buffer));
          }
          if (bytes_read == 0) printf("All messages have been read.\n");
          else if (bytes_read == -1) {
            perror("read fails");
            exit(EXIT_FAILURE);
          }
        }
        
        if (anum == 2){
          printf("Enter the message that will be written from process 2 to process 1: ");
          if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input from stdin\n");
            exit(EXIT_FAILURE);
          }
          puts("");

          bytes_written = write(fd_read21, buffer, strlen(buffer));
          if (bytes_written == -1) {
            perror("write fails");
            exit(EXIT_FAILURE);
          }
          printf("%d bytes have been written from process 2 to process 1\n", bytes_written);
          memset(buffer, '\0', sizeof(buffer));
          bytes_written = 0;
        }
        c++;
      } 
    }
// ================================
    else if (per1 == true && per2 == false){
      if (pid > 0 && pnum == 1){
        if (anum == 1){
          printf("Process 1 reading from process 2: \n");
          // Read from the FIFO until there is no more data available
          while ((bytes_read = read(fd_read21, buffer, BUFSIZ)) > 0) {
              printf("%d bytes have been read: %s\n",bytes_read, buffer);
              memset(buffer, '\0', sizeof(buffer));
          }
          if (bytes_read == 0) printf("All messages have been read.\n");
          else if (bytes_read == -1) {
            perror("read fails");
            exit(EXIT_FAILURE);
          }
        }
        
        if (anum == 2){
          printf("Enter the message that will be written from process 1 to process 2: ");
          if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input from stdin\n");
            exit(EXIT_FAILURE);
          }
          puts("");

          bytes_written = write(fd_read12, buffer, strlen(buffer));
          if (bytes_written == -1) {
            perror("write fails");
            exit(EXIT_FAILURE);
          }
          printf("%d bytes have been written from process 1 to process 2\n", bytes_written);
          memset(buffer, '\0', sizeof(buffer));
          bytes_written = 0;
        }
        c++;
      }
      
    }

  }

  // Close the FIFOs
  close(fd_read12);
  close(fd_read21);

  // Remove the FIFO
  unlink(FFNAME12);
  unlink(FFNAME21);

  return 0;
}