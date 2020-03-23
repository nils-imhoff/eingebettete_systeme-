#include "../include/Printf.h"

static const unsigned int MAX_INT_STRING_LENGTH = lengthOfInt(UINT_MAX) + 1;
static const unsigned int MAX_INT_HEXSTRING_LENGTH = sizeof(int) * 2;
static const unsigned int MAX_INT_BINARYSTRING_LENGTH = (sizeof(int) * 8);

int lengthOfInt(unsigned int n) {
  int i = 0;
  while (n > 0) {
    n = n / 10;
    i++;
  }
  return i;
}

char *intToBaseString(int num, int base, char *c, char *end) {
  *end = '\0';
  unsigned int remainder;

  unsigned int numUns = static_cast<unsigned int>(num);

  if (numUns == 0) {
    end--;
    *end = '0';
    return end;
  }

  while (numUns != 0 && c < end) {
    end--;
    remainder = numUns % base;
    numUns = numUns / base;

    if (remainder <= 9) {
      *end = '0' + remainder;
    } else {
      *end = 'a' + remainder - 10;
    }
  }

  return end;
}

char *stringToString(char *dst, const void *end, char *toCopy,
                     unsigned int limit) {
  unsigned int loopCounter = 0;
  while (toCopy[loopCounter] != '\0' && dst < end && loopCounter < limit) {
    *dst = toCopy[loopCounter];
    dst++;
    loopCounter++;
  }

  return dst;
}

char *Printf(char *dst, const void *end, const char *fmt...) {
  if (dst == nullptr || end == nullptr || fmt == nullptr || dst >= end) {
    return nullptr;
  }

  char *returnBuffer = dst;
  bool lastWasPercent = false;
  va_list args;
  va_start(args, fmt);

  char buffer[MAX_INT_BINARYSTRING_LENGTH];

  while (dst < end && *fmt != '\0') {
    if (*fmt == '%') {
      if (lastWasPercent) {
        *dst = '%';
        dst++;
        lastWasPercent = false;
      } else {
        lastWasPercent = true;
      }
    } else {
      if (lastWasPercent) {
        switch (*fmt) {
          case 'd': {
            int i = va_arg(args, int);
            if (i < 0) {
              i = i * (-1);
              *dst = '-';
              dst++;
            }
            char *stringHelper = intToBaseString(
                i, 10, buffer, buffer + MAX_INT_STRING_LENGTH - 1);

            dst = stringToString(dst, end, stringHelper,
                                 MAX_INT_STRING_LENGTH);
            break;
          }
          case 'u': {
            int i = va_arg(args, int);
            char *stringHelper = intToBaseString(
                i, 10, buffer, buffer + MAX_INT_STRING_LENGTH - 1);

            dst = stringToString(dst, end, stringHelper,
                                 MAX_INT_STRING_LENGTH);
            break;
          }
          case 'c': {
            int i = va_arg(args, int);
            *dst = static_cast<char>(i);
            dst++;
            break;
          }
          case 's': {
            char *stringHelper = va_arg(args, char *);

            dst = stringToString(dst, end, stringHelper, UINT_MAX);
            break;
          }
          case 'x': {
            *dst = '0';
            dst++;
            if (dst < end) {
              *dst = 'x';
              dst++;
            } else {
              continue;
            }

            int i = va_arg(args, int);

            char *stringHelper = intToBaseString(
                i, 16, buffer, buffer + MAX_INT_BINARYSTRING_LENGTH - 1);

            dst = stringToString(dst, end, stringHelper,
                                 MAX_INT_HEXSTRING_LENGTH);

            break;
          }
          case 'b': {
            *dst = '0';
            dst++;
            if (dst < end) {
              *dst = 'b';
              dst++;
            } else {
              continue;
            }

            int i = va_arg(args, int);
            char *stringHelper = intToBaseString(
                i, 2, buffer, buffer + MAX_INT_BINARYSTRING_LENGTH - 1);

            dst = stringToString(dst, end, stringHelper,
                                 MAX_INT_BINARYSTRING_LENGTH);
            break;
          }
          default:
            return nullptr;
        }
        lastWasPercent = false;
      } else {
        *dst = *fmt;
        dst++;
      }
    }

    fmt++;
  }

  *dst = '\0';

  va_end(args);
  return returnBuffer;
}
