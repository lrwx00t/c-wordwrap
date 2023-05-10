#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SUMMARY:
// CASE I and Case II: New Line or SPACE:
//  - Check if we have words in the buffer
//     * None: we check the spaces if they are beyond the limit, if so we reset current to 0
//     * We have words, check if they are beyond the limit, if not add them to current with spaces
//       ** Add the words and space to main buffer
//     * reset space

// CASE III: letter
// - add it to word buffer
// check if we are beyond the limit:
//   - if so, add a new line
//   - reset space
#define BUFFER_SIZE 70
#define SPACE ' '

char buffer[BUFFER_SIZE];

void print_buffer_without_newline(const char *buffer)
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == '\n')
        {
            printf("\\n");
        }
        else
        {
            printf("%c", buffer[i]);
        }
    }
}

void reset_buffer(char *buffer)
{
    memset(buffer, '\0', BUFFER_SIZE); // fill the buffer with null characters
}

void push(char *buffer, char value)
{
    size_t len = strlen(buffer);
    if (len < BUFFER_SIZE - 1)
    {
        buffer[len] = value;    // store the value in the buffer at the end
        buffer[len + 1] = '\0'; // add null terminator to mark end of string
    }
}

char *wordwrap(char *input, int limit)
{
    // printf("%d", strlen(input));
    reset_buffer(buffer);
    char words_buffer[BUFFER_SIZE] = "";
    char space_buffer[BUFFER_SIZE] = "";
    int current = 0;
    for (size_t i = 0; i < strlen(input); i++)
    {
        // CASE I: SPACE
        if (input[i] == SPACE)
        {
            // if this ends a word, add it to buffer and reset
            // also, if there is no space in the buffer (multiple spaces should be appended not to be reset)
            if (strlen(words_buffer) > 0 || strlen(space_buffer) == 0)
            {
                // move current to the sum of space and word_buffers len
                current += strlen(space_buffer) + strlen(words_buffer);
                // reset all buffers
                strcat(buffer, space_buffer);
                reset_buffer(space_buffer);
                strcat(buffer, words_buffer);
                reset_buffer(words_buffer);
            }
            push(space_buffer, SPACE);
        }
        else if (input[i] == '\n')
        {
            // no words in the buffer, we have a new line
            if (strlen(words_buffer) == 0)
            {
                // if we are above the limit, reset current
                // remember it's a new line
                if (current + strlen(space_buffer) > limit)
                {
                    current = 0;
                }
                // we are not beyond the limit
                else
                {
                    // we add the current space to to current
                    current += strlen(space_buffer);
                    // copy copy space to buffer
                    strcat(buffer, space_buffer);
                }
                // reset buffer, we are done here
                reset_buffer(space_buffer);
            }
            // we have words in the buffer
            else
            {
                // move current to the sum of space and word_buffers len
                current += strlen(space_buffer) + strlen(words_buffer);
                // reset all buffers
                strcat(buffer, space_buffer);
                reset_buffer(space_buffer);
                strcat(buffer, words_buffer);
                reset_buffer(words_buffer);
            }
            push(buffer, input[i]);
            current = 0;
        }
        // CASE III: alphanumeric
        // if we have a alphanumeric char
        else
        {
            // first things first, add it to the words buffer
            push(words_buffer, input[i]);
            // insert a line if we are:
            // 1) the word_buff below the limit (large words can't be wrapped)
            // 2) total of current "position" and buffers is larger than the limit
            if ((current + strlen(words_buffer) + strlen(space_buffer) > limit) && strlen(words_buffer) < limit)
            {
                // insert a new line
                push(buffer, '\n');
                // current is zero since this is a new line
                current = 0;
                // no more spaces to be kept
                reset_buffer(space_buffer);
            }
        }
    }
    strcat(buffer, words_buffer);
    // char *str = (char *)malloc(strlen(buffer) + 1);
    // strcpy(str, buffer);
    // printf("%s", buffer);
    return buffer;
}

void test_wrap(char *actual, char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        printf("The buffers are equal.\n");
    }
    else
    {
        printf("\n\nThe buffers are not equal:\n");
        printf("[");
        print_buffer_without_newline(actual);
        printf("]");
        printf("\n");
        printf("[");
        print_buffer_without_newline(expected);
        printf("]");
    }
}

int main()
{
    char dummy_buffer1[] = " This is a list: \n\n\t* foo\n\t* bar\n\n\n\t* baz  \nBAM    ";
    test_wrap(wordwrap(dummy_buffer1, 6), " This\nis a\nlist: \n\n\t* foo\n\t* bar\n\n\n\t* baz\nBAM");
    char dummy_buffer2[] = " This is a list: \n\n\t* foo\n\t* bar\n\n\n\t* baz  \nBAM    ";
    test_wrap(wordwrap(dummy_buffer2, 6), " This\nis a\nlist: \n\n\t* foo\n\t* bar\n\n\n\t* baz\nBAM");
    return 1;
}
