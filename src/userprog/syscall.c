#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "user/syscall.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  printf ("system call!\n");
  thread_exit ();
}

void halt (void)
{
  shutdown_power_off();
}

void exit  (int status)
{
  thread_exit();
}

// wait for thread to die and return status.
// Didn't check for detail conditions.
int wait  (pid_t pid)
{
  return process_wait(pid);
}

bool create  (const char *file, unsigned initial_size)
{
  bool status = filesys_create(file, initial_size);
  return status;
}

bool remove  (const char *file)
{
  bool status = filesys_remove(file);
  return status;
}

// Don't know exactly what is file descriptors is and where to get it.

// int open (const char *file)
// {
//   const char *newfile = filesys_open(file);
//   if (newfile = NULL) {
//
//   }
// }

int filesize (int fd)
{
  enum intr_level old_level;
  old_level = intr_disable ();
  int length = file_length(fd);
  intr_set_level (old_level);
  return length;
}


// Didn't check if file could not be read
int read  (int fd, void *buffer, unsigned size)
{
  enum intr_level old_level;
  old_level = intr_disable();
  int actual_read = file_read(fd, buffer, size);
  intr_set_level(old_level);
  return actual_read;
}

// Not sure if it will write all of buffer at once into a file.
int write  (int fd, const void *buffer, unsigned size)
{
  enum intr_level old_level;
  old_level = intr_disable();
  int actual_write = file_write(fd, buffer, size);
  intr_set_level(old_level);
  return actual_write;
}
