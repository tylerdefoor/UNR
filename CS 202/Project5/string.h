#ifndef STRING_H
#define STRING_H

/**
  *Compares two char* variables
  *If the significant characters are the same, returns true
  */
bool compare ( char* string1, char* string2 );

/**
  *Compares a string literal and a char*
  *If the significant characters are the same, returns true
  */
bool compare ( const char* string1, char* string2 );

/**
  *Compares a char* and a string literal
  *If the significant characters are the same, returns true
  */
bool compare ( char* string1, const char* string2 );

/**
  *Compares two string literals
  *If the significant characters are the same, returns true
  */
bool compare ( const char* string1, const char* string2 );

/**
  *Copies source into dest
  *Assumes source has a non-garbage value
  *Assumes dest has a non-garbage value
  */
void copy ( char* source, char* dest );

/**
  *Copies source into dest
  *Assumes source has a non-garbage value
  *Assumes dest has a non-garbage value
  */
void copy ( const char* source, char* dest );

/**
  *Concatenates two char* variables into dest
  *The new string is string1 followed by string2
  *Assumes that dest is set to NULL
  */
void concat ( char* string1, char* string2, char* dest );

/**
  *Concatenates a string literal and a char* into dest
  *The new string is string1 followed by string2
  *Assumes that dest is set to NULL
  */
void concat ( const char* string1, char* string2, char* dest );

/**
  *Concatenates a char* and a string literal into dest
  *The new string is string1 followed by string2
  *Assumes that dest is set to NULL
  */
void concat ( char* string1, const char* string2, char* dest );

/**
  *Concatenates two string literals into dest
  *The new string is string1 followed by string2
  *Assumes that dest is set to NULL
  */
void concat ( const char* string1, const char* string2, char* dest );

/**
  *Returns the length of a char*
  */
int length ( char* );

/**
  *Returns the length of a string literal
  */
int length ( const char* );

#endif
