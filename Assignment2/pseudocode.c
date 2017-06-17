#define MAX_SIZE 30
condition notFull, notEmpty;
int bufSize = 0;

// Keyboard
while(1) {
  get input from keyboard;
  mutex_lock(mutex);

  while (bufSize == MAX_SIZE) {
    wait(notFull, mutex);
  }
  ListAppend(input);
  bufSize++;
  
  // nothing happen if no process if wating
  signal(notEmpty);

  mutex_unlock(mutex);
}

// Sender
while(1) {
  mutex_lock(mutex);

  while (bufSize == 0) {
    wait(notEmpty, mutex);
  }
  data = ListTrim();
  bufSize--;
  
  // nothing happen if no process if wating
  signal(notFull);
  
  mutex_unlock(mutex);
}