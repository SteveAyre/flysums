#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <openssl/evp.h>

#define BUFFER_SIZE 4096
#define DEFAULT_ALGORITHM "MD5"

int main(int argc, char *argv[])
{
	const char *digest_name = "MD5", *sums_filename = NULL, *filename = NULL;
	const EVP_MD *digest_type;
	char buffer[BUFFER_SIZE];
	size_t bytes;
	EVP_MD_CTX ctx;
	unsigned char digest[EVP_MAX_MD_SIZE];
	unsigned int digest_len = EVP_MAX_MD_SIZE;
	int c, i, errors=0;
	FILE *fp = NULL;

	/* Get filenames from args */
	while((c = getopt(argc, argv, "f:a:h")) != -1) {
		switch(c) {
			case 'f':
				filename = optarg;
				break;
			case 'a':
				digest_name = optarg;
				break;
			case 'h':
				errors++; // Trigger usage message
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else {
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				}
				errors++;
				break;
			default:
				fprintf(stderr, "Unknown getopts error\n");
				errors++;
				break;
		}
	}
	if (argc <= optind) {
		fprintf(stderr, "SUMS_FILE argument is missing\n");
		errors++;
	}
	if (errors) {
		fprintf(stderr, "Usage: %s [OPTIONS] SUMS_FILE\n", argv[0]);
		fprintf(stderr, "\t-f FILENAME  Name of the file to save to SUMS_FILE (optional)\n");
		fprintf(stderr, "\t-a ALGORITHM Algorithm to use (default '%s')\n", DEFAULT_ALGORITHM);
		fprintf(stderr, "\t-h           This help message\n");
		fprintf(stderr, "\n");
		return -1;
	}
	sums_filename = argv[optind];

	/* Initialise OpenSSL, select the digest algorithm to use */
	OpenSSL_add_all_digests();
	if (!(digest_type = EVP_get_digestbyname(digest_name))) {
		fprintf(stderr, "Unknown digest '%s'\n", digest_name);
		return -1;
	}

	/* Open sums file for writing (before piping data to avoid an error afterwards if fopen fails) */
	if (!(fp = fopen(sums_filename, "w"))) {
		perror("fopen");
		return -1;
	}

	/* Read from STDIN, calculate hash on the fly, pipe data out onto STDOUT */
	EVP_DigestInit(&ctx, digest_type);
	for(;;) {
		bytes = fread(buffer, sizeof(char), BUFFER_SIZE, stdin);
		if (bytes <= 0) break;
		fwrite(buffer, sizeof(char), bytes, stdout);
		EVP_DigestUpdate(&ctx, buffer, bytes);
	}
	if (ferror(fp) && !feof(fp)) {
		fprintf(stderr, "Unknown error reading from stdin\n");
		return -1;
	}
	EVP_DigestFinal(&ctx, digest, &digest_len);

	/* Write digest */
	for(i=0; i<digest_len; i++) fprintf(fp, "%02x", digest[i]);
	if (filename) fprintf(fp, " *%s", filename);
	fprintf(fp, "\n");
	fclose(fp);
	
	return 0;
}

