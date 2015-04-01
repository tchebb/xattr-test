#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/xattr.h>

#define BUFFER_SIZE 1024

void print_attribute_list (char* list, int len);
void print_each_attribute (char *file, char *list, int len);
char *next_item(char* list);
void print_buffer (char* buffer, int len);

int main (int argc, char **argv)
{
	char list[BUFFER_SIZE] = {0};
	int ret;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}

	// Ask the kernel for the list of attributes.
	printf("Listing attributes of \"%s\"\n", argv[1]);
	ret = listxattr(argv[1], list, BUFFER_SIZE);
	printf("listxattr() returned %d\n", ret);

	if (ret == -1) {
		printf("listxattr() failed: %s\n", strerror(errno));
	} else {
		printf("  List contents:\n");
		print_attribute_list(list, ret);
	}
	printf("\n");

	// Get each attribute in the list--none of these calls should fail.
	print_each_attribute(argv[1], list, ret);

	return 0;
}

/* print_attribute_list
 * Print each attribute in a NUL-separated list on its own line.
 *
 * Parameters:
 *   list: The list of attributes
 *   len: Length of the list
 */
void print_attribute_list (char* list, int len) {
	char *end = list + len;

	while (list < end) {
		printf("    %s\n", list);
		list = next_item(list);
	}
}

/* print_each_attribute
 * Retrieve every xattr in a list from a certain file and print them out.
 *
 * Parameters:
 *   file: File to read attrs from
 *   list: NUL-separated list of attribute names to read
 *   len: length of list
 */
void print_each_attribute (char *file, char *list, int len) {
	char buffer[BUFFER_SIZE] = {0};
	char *end = list + len;
	int ret;

	while (list < end) {
		printf("Getting attribute \"%s\"\n", list);
		ret = getxattr(file, list, buffer, BUFFER_SIZE);
		printf("getxattr() returned %d\n", ret);
		if (ret == -1) {
			printf("getxattr() failed: %s\n", strerror(errno));
		} else {
			printf("  Attribute value:\n");
			print_buffer(buffer, ret);
		}
		list = next_item(list);
		printf("\n");
	}
}

/* next_item
 * Find the beginning of the next item in a NUL-separated list.
 *
 * Parameters:
 *   list: The beginning of the current item in the list
 * Returns: The beginning of the next item in the list
 */
char *next_item(char* list) {
	while (*(list++));
	return list;
}

/* print_buffer
 * Print each byte of a character buffer in hex, nicely formatted.
 *
 * Parameters:
 *   buffer: Buffer to print
 *   len: Length of buffer
 */
void print_buffer (char* buffer, int len) {
	int i;
	for (i = 0; i < len; ++i) {
		// Add indentation for readability
		if (i % 8 == 0)
			printf("    ");

		printf("0x%02x", buffer[i]);

		// Break up the lines
		if ((i + 1) % 8 == 0)
			printf("\n");
		else
			printf(" ");
	}
}
