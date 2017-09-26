#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "/tmp/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]) {
  int uid, printing, fd;
  printing = 1;

  char search_str[100];

  if (argc > 1) {
    strcpy(search_str, argv[1]);
  } else {
    search_str[0] = 0;
  }

  printf("[DEBUG] Search string: %s\n\n", search_str);

  uid = getuid();

  fd = open(FILENAME, O_RDONLY);

  if (fd == -1) {
    fatal("in main() while opening file for reading");
  }

  while (printing) {
    printing = print_notes(fd, uid, search_str);
  }

  printf("[end of note data]\n");

  close(fd);
}

int print_notes(int fd, int uid, char *search_str) {
  int note_len;
  int read_bytes;

  char byte;
  char note_buf[100];

  note_len = find_user_note(fd, uid);

  if (note_len == -1) {
    return 0;
  }

  read_bytes = read(fd, note_buf, note_len);

  printf("[DEBUG] read bytes from note: %d\n", read_bytes);

  if(read_bytes == -1) {
      fatal("can't read the user note.\n");
  };

  note_buf[note_len] = 0;

  printf("[DEBUG] note content: %s\n", note_buf);

  if (search_note(note_buf, search_str)) {
    printf("found: %s\n", note_buf);
  }

  return 1;
}

int find_user_note(int fd, int user_uid) {
  int note_uid = -1;

  unsigned char byte;

  int length;

  while (note_uid != user_uid) {
    if (read(fd, &note_uid, 4) != 4) {
      return -1;
    }

    if (read(fd, &byte, 1) != 1) {
      return -1;
    }

    byte = length = 0;

    while (byte != '\n') {
      if (read(fd, &byte, 1) != 1) {
        return -1;
      }
      length++;
    }
  }

  lseek(fd, length * -1, SEEK_CUR);

  printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);

  return length;
}

int search_note(char *note, char *keyword) {
  int i;
  int keyword_length;
  int match;

  match = 0;

  keyword_length = strlen(keyword);

  if (keyword_length == 0) {
    return 1;
  }

  for (i = 0 ; i < strlen(note) ; i++) {
    if (note[i] == keyword[match]) {
      match++;
    } else {
      if (note[i] == keyword[0]) {
        match = 1;
      } else {
        match = 0;
      }
    }

    if (match == keyword_length) {
      return 1;
    }
  }

  return 0;
}

void fatal(char *message) {
  char error_message[100];

  strcpy(error_message, "[!!] Fatal error ");
  strncat(error_message, message, 83);
  perror(error_message);
  exit(-1);
}
