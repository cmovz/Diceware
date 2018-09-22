#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define DEFAULT_DICTIONARY_FILE "dictionary.dice"
#define MAX_WORD_SIZE 80

typedef struct {
  FILE* file;
  uint32_t word_count;
} Dictionary;

int dictionary_init(Dictionary* dictionary, char const* file)
{
  dictionary->file = fopen(file, "r");
  if(!dictionary->file)
    return 0;

  if(fread(&dictionary->word_count,
           sizeof dictionary->word_count,
           1,
           dictionary->file) != 1)
  {
    fclose(dictionary->file);
    return 0;
  }

  return 1;
}

void dictionary_destroy(Dictionary* dictionary)
{
  fclose(dictionary->file);
}

int64_t dictionary_get_word(char* buffer,
                            uint32_t size,
                            Dictionary* dictionary,
                            uint32_t index         )
{
  uint32_t start, end, max_read;

  if(index > dictionary->word_count)
    return -1;

  fseek(dictionary->file, (index + 1) * sizeof dictionary->word_count, SEEK_SET);

  if(fread(&start, sizeof start, 1, dictionary->file) != 1)
    return -1;

  if(fread(&end, sizeof end, 1, dictionary->file) != 1)
    return -1;

  max_read = (end - start > size) ? size : end - start;
  fseek(dictionary->file, start, SEEK_SET);
  return fread(buffer, 1, max_read, dictionary->file);
}

unsigned int convert_str_to_uint(char const* str)
{
  unsigned int i = 0;
  while(*str){
    i *= 10;
    i += *str - '0';
    ++str;
  }

  return i;
}

int is_valid_number(char const* str)
{
  //empty string with a '\0' is considered 0
  while(*str)
    if(!isdigit(*str++))
      return 0;

  return 1;
}

int main(int argc, char* argv[])
{
  Dictionary dictionary;
  char const* dictionary_file;
  unsigned int word_count;
  FILE* random_file;
  uint32_t max_value;

  switch(argc){
    case 2:
      dictionary_file = DEFAULT_DICTIONARY_FILE;
      break;

    case 3:
      dictionary_file = argv[2];
      break;

    default:
      fprintf(stderr, "Usage: %s 9 [dictionary_file]\n", argv[0]);
      return EXIT_FAILURE;
  }

  if(!is_valid_number(argv[1])){
    fprintf(stderr, "Error: %s is not a valid number of words\n", argv[1]);
    return EXIT_FAILURE;
  }

  word_count = convert_str_to_uint(argv[1]);

  random_file = fopen("/dev/urandom", "r");
  if(!random_file){
    perror("opening /dev/urandom");
    return EXIT_FAILURE;
  }

  if(!dictionary_init(&dictionary,  dictionary_file)){
    perror("opening dictionary file");
    goto err0;
  }

  //avoid messing up the probability of certain words
  max_value = UINT32_MAX - (UINT32_MAX % dictionary.word_count) - 1;

  while(word_count--){
    char buffer[MAX_WORD_SIZE + 1];
    uint32_t random_n;
    int ret;

    if(fread(&random_n, sizeof random_n, 1, random_file) != 1){
      perror("reading random number");
      goto err1;
    }

    //repeat to make every word as likely as any other
    if(random_n > max_value)
      continue;

    random_n %= dictionary.word_count;
    ret = dictionary_get_word(buffer, MAX_WORD_SIZE, &dictionary, random_n);

    if(ret < 1){
      perror("retrieving word from dictionary");
      goto err1;
    }

    if(ret > MAX_WORD_SIZE){
      fputs("Error: the dictionary is corrupt and/or non-compliant\n", stdout);
      goto err1;
    }

    buffer[ret] = '\0';
    fputs(buffer, stdout);
    if(word_count)
      putchar(' ');
    else
      putchar('\n');
  }

  fclose(random_file);
  dictionary_destroy(&dictionary);
  return EXIT_SUCCESS;

err1:
  dictionary_destroy(&dictionary);

err0:
  fclose(random_file);
  return EXIT_FAILURE;
}

