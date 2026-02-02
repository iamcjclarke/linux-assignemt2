/*

  Word Count using dedicated lists

*/

/*
Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/resource.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount *word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64

/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 * Useful functions: fgetc(), isalpha().
 */
int num_words(FILE* infile) {
  int c;
  int in_word = 0;
  int count = 0;

  while ((c = fgetc(infile)) != EOF) {
    if (isalpha(c)) {
      if (!in_word) {
        in_word = 1;
        count++;
      }
    } else {
      in_word = 0;
    }
  }

  rewind(infile);
  return count;
}



/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 * Useful functions: fgetc(), isalpha(), tolower(), add_word().
 */
void count_words(WordCount **wclist, FILE *infile) {
  char buffer[MAX_WORD_LEN];
  int idx = 0;
  int c;

  while ((c = fgetc(infile)) != EOF) {
    if (isalpha(c)) {
      if (idx < MAX_WORD_LEN - 1) {
        buffer[idx++] = tolower(c);
      }
    } else {
      if (idx > 0) {
        buffer[idx] = '\0';
        add_word(wclist, buffer);
        idx = 0;
      }
    }
  }

  if (idx > 0) {
    buffer[idx] = '\0';
    add_word(wclist, buffer);
  }

  rewind(infile);
}


/*
 * Comparator to sort list by frequency.
 * Useful function: strcmp().
 */
static bool wordcount_less(const WordCount *wc1, const WordCount *wc2) {
  return 0;
}

// In trying times, displays a helpful message.
static int display_help(void) {
	printf("Flags:\n"
	    "--count (-c): Count the total amount of words in the file, or STDIN if a file is not specified. This is default behavior if no flag is specified.\n"
	    "--frequency (-f): Count the frequency of each word in the file, or STDIN if a file is not specified.\n"
	    "--help (-h): Displays this help message.\n");
	return 0;
}
static void print_limits(void) {
  struct rlimit lim;

  // Stack size (bytes)
  if (getrlimit(RLIMIT_STACK, &lim) == 0) {
    if (lim.rlim_cur == RLIM_INFINITY) {
      printf("stack size: unlimited\n");
    } else {
      printf("stack size: %lu\n", (unsigned long)lim.rlim_cur);
    }
  } else {
    perror("getrlimit(RLIMIT_STACK)");
  }

  // Max number of processes (per user)
  if (getrlimit(RLIMIT_NPROC, &lim) == 0) {
    if (lim.rlim_cur == RLIM_INFINITY) {
      printf("process limit: unlimited\n");
    } else {
      printf("process limit: %lu\n", (unsigned long)lim.rlim_cur);
    }
  } else {
    perror("getrlimit(RLIMIT_NPROC)");
  }

  // Max number of open files (file descriptors)
  if (getrlimit(RLIMIT_NOFILE, &lim) == 0) {
    if (lim.rlim_cur == RLIM_INFINITY) {
      printf("max file descriptors: unlimited\n");
    } else {
      printf("max file descriptors: %lu\n", (unsigned long)lim.rlim_cur);
    }
  } else {
    perror("getrlimit(RLIMIT_NOFILE)");
  }
}

/*
 * Handle command line flags and arguments.
 */
int main (int argc, char *argv[]) {

  // Count Mode (default): outputs the total amount of words counted
  bool count_mode = true;
  int total_words = 0;

  // Freq Mode: outputs the frequency of each word
  bool freq_mode = false;

  FILE *infile = NULL;

  // Variables for command line argument parsing
  int i;
  static struct option long_options[] =
  {
      {"count", no_argument, 0, 'c'},
      {"frequency", no_argument, 0, 'f'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}
  };

  // Sets flags
  while ((i = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
      switch (i) {
          case 'c':
              count_mode = true;
              freq_mode = false;
              break;
          case 'f':
              count_mode = false;
              freq_mode = true;
              break;
          case 'h':
              return display_help();
      }
  }

  if (!count_mode && !freq_mode) {
    printf("Please specify a mode.\n");
    return display_help();
  }

  /* Create the empty data structure */
  init_words(&word_counts);

  if ((argc - optind) < 1) {
    // No input file specified, instead, read from STDIN instead.
    infile = stdin;
  } else {
    infile = fopen(argv[optind], "r");
    if (!infile) {
      perror(argv[optind]);
      return 1;
    }
  }
  print_limits();



  if (count_mode) {
  if (count_mode) {
    total_words = num_words(infile);
    printf("The total number of words is: %i\n", total_words);
  } else {
    wordcount_sort(&word_counts, wordcount_less);

    printf("The frequencies of each word are: \n");
    fprint_words(word_counts, stdout);
}
  return 0;
}
}
