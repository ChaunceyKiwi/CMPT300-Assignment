#define MAX_SIZE 30
condition notFull, notEmpty;

// Keyboard
while(1) {
  get input from keyboard;
  mutex_lock(mutex);

  while (ListCount() == MAX_SIZE) {
    wait(notFull, mutex);
  }
  ListAppend(input);
  
  // nothing happen if no process if wating
  signal(notEmpty);

  mutex_unlock(mutex);
}

// Sender
while(1) {
  mutex_lock(mutex);

  while (ListCount() == 0) {
    wait(notEmpty, mutex);
  }
  data = ListTrim();
  
  // nothing happen if no process if wating
  signal(notFull);
  
  mutex_unlock(mutex);
}