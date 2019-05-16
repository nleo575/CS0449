/********************************************
*	Author: Nicolas Leo						*
*	PeopleSoft: 3466098						*
*	Class: CS 0449							*
*	Project 1: ID3 Tag Editor				*
********************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct id3tag //Struct to describe an ID3 tag
{
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	char zero_byte_sep;
	char track;
	char genre;
};

void print_tag(struct id3tag);

int main(int argc, char const *argv[])
{
	FILE *fp;
	struct id3tag tag;
	int invalidargc = 0; //Keeps tally of invalid command line arguments. 

	if (argc < 2) //Check for minimum # of arguments (2: program & file name)
    {
        printf("Please enter a valid filename.\n");
        exit (1);
    }

    if (argc %2 == 1) //Program will exit if incorrect # of arguments entered
    {
    	printf("You entered an correct number of arguments!\n");
    	printf("You may set the following fields using -FIELDNAME VALUE\n");
    	printf("\t-%-8sSong title (max 30 chars)\n", "title");
    	printf("\t-%-8sName of Artist (max 30 chars)\n", "artist");
    	printf("\t-album   Name of Album (max 30 chars)\n");
    	printf("\t-year    Year song released (yyyy)\n");
    	printf("\t-comment Comment string (max 28 chars)\n");
    	printf("\t-track   Track number\n");

    	exit (1);
    }

	// Open fp for both reading and writing
	fp = fopen(argv[1], "rb+");

	//If error opening file
    if(fp == NULL)
    {
        printf("Invalid filename entered.\n");
        exit (1);
    }

    //Read to the end of file, then back 128 bytes
	fseek(fp, 0L, SEEK_END);
	fseek(fp, -128, SEEK_END);

	//Read in last 128 bytes of the file.
	fread(&tag, sizeof(tag), 1, fp);

	//Check for 'TAG' to indicate an ID3 tag is present in the file
	if (tag.tag[0] == 'T' && tag.tag[1] == 'A' && tag.tag[2] == 'G')
	{	//File has a valid tag
		
		if (argc == 2) //Read-only mode
		{
			print_tag(tag);
			fclose(fp);
			return 0;
		}
		else //Edit-tag mode
		{
			printf("Original Tag\n");
			print_tag(tag);
			fseek(fp, -128, SEEK_END); //Move file pointer back 128 bits to write new tag
		}
	}
	else //If no tag, may need to append data to the end
	{
		printf("This file doesn't have a valid ID3 Tag.\n");
		if (argc == 2) //File doesn't contain an ID3 tag
		{
			fclose(fp);
			return 0;
		}
		else //Append a new ID3 tag to the end of the file.
		{
			fclose(fp);
			fp = fopen(argv[1], "ab"); //Reopen file in append mode
			//initialize tag to empty values.
			strncpy(tag.tag, "TAG", 3);
			strncpy(tag.title, "\0", 30);
			strncpy(tag.artist, "\0", 30);
			strncpy(tag.album, "\0", 30);
			strncpy(tag.year, "\0", 4);
			strncpy(tag.comment, "\0", 28);
			tag.zero_byte_sep = '\0';
			tag.track = '\0';
			tag.genre = 0;

			printf("Data will be appended to the end of the file.\n");
		}
	}

	//Checks command line arguments for fields to update
	if (argc > 2)
	{
		int i;
		for (i = 2; i < argc; i+=2)
		{
			if(strcmp(argv[i], "-title") == 0)
				strncpy(tag.title, argv[i+1], 30);
			else if(strcmp(argv[i], "-artist") == 0)
				strncpy(tag.artist, argv[i+1], 30);
			else if(strcmp(argv[i], "-album") == 0)
				strncpy(tag.album, argv[i+1], 30);
			else if(strcmp(argv[i], "-year") == 0)
				strncpy(tag.year, argv[i+1], 4);
			else if(strcmp(argv[i], "-comment") == 0)
				strncpy(tag.comment, argv[i+1], 28);
			else if(strcmp(argv[i], "-track") == 0)
				tag.track = atoi(argv[i+1])&0xff; //Converts text to 8 bit unsigned int
			else
				invalidargc+=2;
		}

		printf("\nUpdated Tag\n");
		print_tag(tag);
	}

	//Only write if there was at least 1 valid ID3 tag edit
	if ((argc - 2 - invalidargc)/2 > 0)
		fwrite(&tag, sizeof(tag), 1, fp);

	fclose(fp);
	return 0;
}

/*
	Prints out the contents of the ID3 tag.
	Used %c instead of %s for strings since fields may not be null terminated.
*/
void print_tag(struct id3tag id3t)
{
	int i;
	
	printf("Title:   ");
	for (i = 0; i < 30; ++i)
		printf("%c", id3t.title[i]);

	printf("\nArtist:  ");
	for (i = 0; i < 30; ++i)
		printf("%c", id3t.artist[i]);

	printf("\nAlbum:   ");
	for (i = 0; i < 30; ++i)
		printf("%c", id3t.album[i]);

	printf("\nYear:    ");
	for (i = 0; i < 4; ++i)
		printf("%c", id3t.year[i]);

	printf("\nComment: ");
	for (i = 0; i < 28; ++i)
		printf("%c", id3t.comment[i]);

	unsigned int tnum = id3t.track&0x000000ff;

	printf("\nTrack #: %u\n", tnum);
	printf("Genre:   ");

	//Prints out the correct genre
	switch(id3t.genre)
	{
		case 0: 	printf("Blues"); 			break;
		case 1: 	printf("Classic rock"); 	break;
		case 2: 	printf("Country");			break;
		case 3: 	printf("Dance"); 			break;
		case 4: 	printf("Disco"); 			break;
		case 5: 	printf("Funk"); 			break;
		case 6: 	printf("Grunge"); 			break;
		case 7: 	printf("Hip-Hop");			break;
		case 8: 	printf("Jazz"); 			break;
		case 9: 	printf("Metal"); 			break;
		case 10:	printf("New Age"); 			break;
		case 11:	printf("Oldies"); 			break;
		case 12:	printf("Other"); 			break;
		case 13:	printf("Pop"); 				break;
		case 14:	printf("Rhythm and Blues"); break;
		case 15:	printf("Rap"); 				break;
		case 16:	printf("Reggae"); 			break;
		case 17:	printf("Rock"); 			break;
		case 18:	printf("Techno"); 			break;
		case 19:	printf("Industrial"); 		break;
		case 20:	printf("Alternative"); 		break;
		case 21:	printf("Ska"); 				break;
		case 22:	printf("Death Metal"); 		break;
		case 23:	printf("Pranks"); 			break;
		case 24:	printf("Soundtrack"); 		break;
		case 25:	printf("Euro-Techno"); 		break;
		case 26:	printf("Ambient"); 			break;
		case 27:	printf("Trip-Hop"); 		break;
		case 28:	printf("Vocal"); 			break;
		case 29:	printf("Jazz & Funk"); 		break;
		case 30:	printf("Fusion"); 			break;
		case 31:	printf("Trance"); 			break;
		case 32:	printf("Classical"); 		break;
		case 33:	printf("Instrumental"); 	break;
		case 34:	printf("Acid"); 			break;
		case 35:	printf("House"); 			break;
		case 36:	printf("Game"); 			break;
		case 37:	printf("Sound clip"); 		break;
		case 38:	printf("Gospel"); 			break;
		case 39:	printf("Noise"); 			break;
		case 40:	printf("Alternative Rock"); break;
		case 41:	printf("Bass"); 			break;
		case 42:	printf("Soul"); 			break;
		case 43:	printf("Punk"); 			break;
		case 44:	printf("Space"); 			break;
		case 45:	printf("Meditative"); 		break;
		case 46:	printf("Instrumental Pop"); break;
		case 47:	printf("Instrumental Rock");break;
		case 48:	printf("Ethnic"); 			break;
		case 49:	printf("Gothic"); 			break;
		case 50:	printf("Darkwave"); 		break;
		case 51:	printf("Techno-Industrial");break;
		case 52:	printf("Electronic"); 		break;
		case 53:	printf("Pop-Folk"); 		break;
		case 54:	printf("Eurodance"); 		break;
		case 55:	printf("Dream"); 			break;
		case 56:	printf("Southern Rock"); 	break;
		case 57:	printf("Comedy"); 			break;
		case 58:	printf("Cult"); 			break;
		case 59:	printf("Gangsta"); 			break;
		case 60:	printf("Top 40"); 			break;
		case 61:	printf("Christian Rap"); 	break;
		case 62:	printf("Pop/Funk"); 		break;
		case 63:	printf("Jungle"); 			break;
		case 64:	printf("Native US"); 		break;
		case 65:	printf("Cabaret"); 			break;
		case 66:	printf("New Wave"); 		break;
		case 67:	printf("Psychedelic"); 		break;
		case 68:	printf("Rave"); 			break;
		case 69:	printf("Show tunes"); 		break;
		case 70:	printf("Trailer"); 			break;
		case 71:	printf("Lo-Fi"); 			break;
		case 72:	printf("Tribal"); 			break;
		case 73:	printf("Acid Punk"); 		break;
		case 74:	printf("Acid Jazz"); 		break;
		case 75:	printf("Polka"); 			break;
		case 76:	printf("Retro"); 			break;
		case 77:	printf("Musical"); 			break;
		case 78:	printf("Rock ’n’ Roll"); 	break;
		case 79:	printf("Hard Rock"); 		break;
		default: 	printf("Unknown Genre"); 	
	}
	printf("\n");
}