#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t lock1, lock2;
char* get_sentence(int number);
void* playgame(void* i);
void* sleepthread(void* i);

void handler(int signum);
char* sentence1 = "Society often forgives the criminal; it never forgives the dreamer.\n";
char* sentence2 = "Hear no evil, speak no evil and you'll never be invited to a party.\n";
char* sentence3 = "Moderation is a fatal thing. Nothing succeeds like excess.\n";
char* sentence4 = "They spoil every romance by trying to make it last forever.\n";
char* sentence5 = "Morality, like art, means drawing a line somewhere.\n";

int failed_tries = 0; //check for thread safety
char input[500];
char* sentence;
int n = 10;
pthread_t thread1, thread2;

int main() {

int ch;
signal(SIGUSR1, handler);
pthread_mutex_lock(&lock1);
pthread_mutex_unlock(&lock1);
int i;
time_t t;
void* placeholder = NULL;
int randomize[] = {0,1,2,3,4};
int r = 0;
int play = 1;
void* ret = NULL;
srand((unsigned) time(&t));


for (i = 4; i > 0 ; i--) {    // shuffle array

    int randomIndex = rand() % i;

    int temp = randomize[i];
    randomize[i] = randomize[randomIndex];

    randomize[randomIndex] = temp;
}
i = 0;
printf("Lets start the game: Copy the sentences that show up below in 10 seconds.\n");
printf("You will get 3 attemps at the same sentence\n");
printf("\n");


while (play == 1) {
  printf("You have %d seconds to type the following sentence: \n", n);
  printf("\n");

  sentence = get_sentence(randomize[i]);
  printf("%s\n", sentence);
  // creating new thread
  r = pthread_create(&thread1, NULL, &playgame, &placeholder);
  r = pthread_create(&thread2, NULL, &sleepthread, &placeholder);


  pthread_join(thread1, NULL);
  pthread_kill(thread2, SIGUSR1);

  if (strcmp(sentence, input) == 0) {
    printf("\n");
    printf("Congratulations! Next sentence:\n");
    printf("Please press enter to continue\n");
    printf("\n");

    pthread_mutex_lock(&lock1);
    if (n == 1) {
      n = 1;
    } else {
    n--;
    }
    pthread_mutex_unlock(&lock1);
    i++;
    failed_tries = 0;
  } else {
    printf("\n");
    printf("That was incorrect. You have %d tries left for this statement.\n", (2-failed_tries));
      printf("Please press enter to continue\n");
    failed_tries++;
    }
    if (failed_tries > 2) {
      printf("\n");
      printf("You lose the game!\n");
      play = 0;
      return -1;
    }
    if (i > 4) {
      printf("game over\n");
      play = 0;
      return 0;
    }
    while ((ch = getchar()) != '\n');
  }
  return 0;
}


void* sleepthread(void* ph) {

  void* placeholder = NULL;
  pthread_mutex_lock(&lock1);
  int number = n;
  printf("%d\n", number);
  pthread_mutex_unlock(&lock1);

    sleep(number);

    pthread_mutex_lock(&lock2);
    pthread_kill(thread1, SIGUSR1);
    pthread_mutex_unlock(&lock2);
  return placeholder;
}


void* playgame(void* ph) {
  void* placeholder = NULL;

    fgets(input, 500, stdin);

    return placeholder;
}

/*
* This method returns a randomly chosen sentence from the 5 sentences.
*/
char* get_sentence(int number) {
char* ret = NULL;
  switch(number) {

    case 0: ret = sentence1;
    break;

    case 1: ret = sentence2;
    break;

    case 2: ret = sentence3;
    break;

    case 3: ret = sentence4;
    break;

    case 4: ret = sentence5;
    break;

    default:
    printf("Error message\n");
  }

return ret;
}


void handler(int signum) {
  pthread_exit(NULL);
}
