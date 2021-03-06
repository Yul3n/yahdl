/* Example:
 * chip and3(a, b, c) is
 *   let o1 = and(a, b);
 *   and(o1, c);
 * end and3
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

FILE *       in_file;
struct token unused_token;

static struct token mktoken(unsigned int);
static struct token mktide(char *);
static struct token get_token(void);
static int keyword(char *);

struct token next_token(void);
void putback_token(struct token);
void init_lexer(char *);

static struct token
mktoken(unsigned int type)
{
	return (struct token){.type = type};
}

static struct token
mktide(char *ide)
{
	return (struct token){.type = T_IDE, .ide = ide};
}

static int
keyword(char *s)
{
	switch (*s) {
	case 'c':
		if (!strcmp(s + 1, "hip"))
			return T_CHIP;

		break;
	case 'e':
		if (!strcmp(s + 1, "nd"))
			return T_END;

		break;
	case 'i':
		if (!strcmp(s + 1, "s"))
			return T_IS;

		break;
	case 'l':
		if (!strcmp(s + 1, "et"))
			return T_LET;

		break;
	}
	return -1;
}

static struct token
get_token(void)
{
	int c;

	while ((c = getc(in_file)) == ' ' || c == '\t');
	if (c == EOF)
		return mktoken(T_EOF);
	else if (isalpha(c)) {
		ungetc(c, in_file);
		char *ide;
		char *s = (ide = malloc(256)) - 1;
		int   type;
		while (isalpha(c = getc(in_file)))
			*(++s) = c;
		*(s + 1) = '\0';
		type     = keyword(ide);
		ungetc(c, in_file);
		if (type >= 0)
			return mktoken(type);

		return mktide(ide);
	} else
		switch (c) {
		case ',': return mktoken(T_COMA);

		case ' ':
		case '\t':
		case '\n': return get_token();

		case ';': return mktoken(T_SEMI);

		case '=':
			return mktoken(T_EQU);

		case '(':
			return mktoken(T_LPAR);

		case ')':
			return mktoken(T_RPAR);

		default:
			exit(1);
		}
}

struct token
next_token(void)
{
	if (unused_token.type == T_EOF)
		return get_token();
	else {
		struct token t = unused_token;
		unused_token = mktoken(T_EOF);
		return t;
	}
}

void
putback_token(struct token t)
{
	unused_token = t;
}

void
init_lexer(char *f)
{
	in_file = fopen(f, "r");
	if (!in_file) {
		printf("Couldn't open file %s", f);
		exit(1);
	}
	unused_token = mktoken(T_EOF);
}

void
close_lexer(void)
{
	fclose(in_file);
}
