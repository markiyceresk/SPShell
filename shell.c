#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <wchar.h>
#include <locale.h>
#include <pwd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <termios.h>
#include <sys/wait.h>

char cwd[512];
char prm[512];
char *cmd;
char hostname[HOST_NAME_MAX];
char dist[90];
char *endi;
short rnew;
short cch;
int sch;
char usin[128];
struct termios oldt, newt;
short ns;
char *lsar[128];
short li;
char *mdar[128];
char *rear[512];

int main() {
	char h[128] = "/home/";
	strcat(h, getlogin());
	setvbuf(stdout, NULL, _IONBF, 0);
	// write os id to variable "endi"
	FILE *fidi = fopen("/etc/os-release", "r");
	char line[256];
	while (fgets(dist, sizeof(dist), fidi)) {
		if (strncmp(dist, "ID=", 3) == 0) {
			endi = dist + 3;
			break;
		}
	}
	fclose(fidi);
	// print ASCII art
	setlocale(LC_ALL, "");
	printf("\033[37m\033[40m|¯¯¯¯¯¯¯\033[37m\033[44m¯¯¯¯¯¯¯¯\033[37m\033[40m¯¯¯¯¯¯¯|\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[41m\033[37m|       \033[37m\033[45m        \033[41m\033[37m       |\033[0m\n");
	printf("\033[41m\033[37m|       \033[37m\033[45m        \033[41m\033[37m       |\033[0m\n");
	printf("\033[41m\033[37m|       \033[37m\033[45m        \033[41m\033[37m       |\033[0m\n");
	printf("\033[41m\033[37m|       \033[37m\033[45m        \033[41m\033[37m       |\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[37m\033[40m|       \033[37m\033[44m        \033[37m\033[40m       |\033[0m\n");
	printf("\033[37m\033[40m|_______\033[37m\033[44m________\033[37m\033[40m_______|\033[0m\n");

	// print greeting
	printf("\n!!! НЕТ ВОЙНЕ !!!\n\nHi, %s!\nWhat would you do?\n\n", getlogin());


	// =============================== MAIN CYCLE ===============================

    while (1) {

	// ----- greeting block -----

	// print greeting
	struct termios oldt, newt;
    	int rows = 0, cols = 0;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        write(STDOUT_FILENO, "\033[6n", 4);
	scanf("\033[%d;%dR", &rows, &cols);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    	if (cols > 1) printf("\n");
	gethostname(hostname, HOST_NAME_MAX);
	getcwd(cwd, sizeof(cwd));
	snprintf(prm, sizeof(prm), "%s | %s | %s $ ", getlogin(), hostname, cwd);

	// input command
	cmd = readline(prm);
	if (!cwd) break;
	if (*cwd) {
	    add_history(cmd);
	}

	// ----- command block -----

	// command q (quit)
	if (strncmp(cmd, "q", 1) == 0) {
	    printf("\n");
	    free(cmd);
	    break;

	// command cd (change directory)
	} else if (strncmp(cmd, "cd", 2) == 0) {
	    if (strncmp(cmd + 3, "~", 1) == 0) {
		chdir(h);
	    } else {
	        chdir(cmd + 3);
	    }

	// command help (have -s flag)
	} else if (strncmp(cmd, "help", 4) == 0) {
	    if (strncmp(cmd + 5, "-s", 2) == 0) {
		printf("sus, hi, potato, news, I am not a moron!");
	    } else {
		printf("q - quit\ncd <arguments> - change directory\ninst <arguments> - install packet from packet manager (Only Arch based, Gentoo, Debian based, and Fedora are supported)\nua - full system update (Only the previously listed distributions are supported)\nrps - rock paper scissors\nhelp - use '-s' flag to see references :)");
	    }

	// command sus
	} else if (strncmp(cmd, "sus", 3) == 0) {
	    printf("This looks kinda SUS...");

	// command hi
	} else if (strncmp(cmd, "hi", 2) == 0) {
		printf("Hello!");

	// command news
	} else if (strncmp(cmd, "news", 4) == 0) {
		srand(time(NULL));
		rnew = rand() % 10;
		switch (rnew) {
			case 0:
				printf("Wheatley says, he isn't a moron! Everyone is laughing, but GLaDOS is potato.");
				break;
			case 1:
				printf("Microslop (didn't) make windows open source!!!!!!!!!");
				break;
			case 2:
				printf("I use Arch BTW");
				break;
			case 3:
				printf("Vladimir Putin says, internet is drugs!");
				break;
			case 4:
				printf("CEO of OpenAI says, vibecoding is our future! Is he right?");
				break;
			case 5:
				printf("'Tomorrow's wind will blow tomorrow' - Karych");
				break;
			case 6:
				printf("Scientists have taken on one of the most difficult questions in human history: Why do people dislike Viette's theorem so much?");
				break;
			case 7:
				printf("When you feel stupid, remember that Windows users don't use the terminal on a daily basis.");
				break;
			case 8:
				printf("Roskomnadzor has banned the letter 'A'");
				break;
			case 9:
				printf("Cheese ball.");
				break;
		}

	// command rps (rock paper scissors)
	} else if (strncmp(cmd, "rps", 3) == 0) {
	    struct termios t;
	    int sch = 0, cch, u;
            char b;
	    tcgetattr(0, &t);
	    struct termios n = t;
	    n.c_lflag &= ~(ICANON | ECHO);
	    tcsetattr(0, TCSANOW, &n);
	    srand(time(0));
	    while (printf("1. Rock\n2. Paper\n3. Scissors\n4. Quit\n") && read(0, &b, 1) && b != '4') {
		u = b - '0';
		cch = rand() % 3 + 1;
		printf("\nComputer: ");
		if (cch == 1) printf("rock");
		if (cch == 2) printf("paper");
		if (cch == 3) printf("scissors");
		printf(" | ");
		if (u == cch) printf("Draw");
		else if ((u % 3) + 1 == cch) { printf("You lost"); sch--; }
		else { printf("You won"); sch++; }
		printf(" | Score: %d\n\n", sch);
	    }
	    tcsetattr(0, TCSANOW, &t);

	// command I am not a moron (reference to portal 2)
	} else if (strncmp(cmd, "I am not a moron!", 17) == 0) {
	    printf("Yes, you are!!!\n");
	    sleep(1);
	    printf("You are the moron, they built to make me an idiot!");

	// command potato (reference to PWGood)
	} else if (strncmp(cmd, "potato", 6) == 0) {
	    printf("Картошка!\n");
	    sleep(1);
	    printf("картошка\nкартошка\nкартошка!");

	// command inst (install)
	} else if (strncmp(cmd, "inst", 4) == 0) {
	    char incm[256];
	    if (strncmp(endi, "arch", 4) == 0 || strncmp(endi + 5, "arch", 4) == 0) {
                sprintf(incm, "sudo pacman -S %s", (cmd + 5)); // ARCH
            } else if (strncmp(endi + 5, "debian", 6) == 0 || strncmp(endi, "debian", 6) == 0) {
                sprintf(incm, "sudo apt install %s", (cmd + 5)); // UBUNTU or DEBIAN
            } else if (strncmp(endi, "gentoo", 6) == 0) {
                sprintf(incm, "sudo emerge --ask %s", (cmd + 5)); // GENTOO
            } else if (strncmp(endi, "fedora", 6) == 0) {
                sprintf(incm, "sudo dnf install %s", (cmd + 5)); // FEDORA
            } else {
		continue;
	    }
	    system(incm);

	// command ua (update all)
	} else if (strncmp(cmd, "ua", 2) == 0) {
	    if (strncmp(endi, "arch", 4) == 0 || strncmp(endi + 5, "arch", 4) == 0) {
		system("sudo pacman -Syu"); // ARCH or CACHY
	    } else if (strncmp(endi + 5, "debian", 6) == 0 || strncmp(endi, "debian", 6) == 0) {
		system("sudo apt update"); // UBUNTU or DEBIAN
	    } else if (strncmp(endi, "gentoo", 6) == 0) {
		system("sudo emerge --sync"); // GENTOO
		system("sudo emerge -avuDN @world");
	    } else if (strncmp(endi, "fedora", 6) == 0) {
		system("sudo dnf upgrade --refresh"); // FEDORA
	    }

	// command del (delete)
	} else if (strncmp(cmd, "del", 3) == 0) {
	    char incm[256];
	    if (strncmp(endi, "arch", 4) == 0 || strncmp(endi + 5, "arch", 4) == 0) {
		sprintf(incm, "sudo pacman -R %s", (cmd + 4)); // ARCH
	    } else if (strncmp(endi + 5, "debian", 6) == 0 || strncmp(endi, "debian", 6) == 0) {
		sprintf(incm, "sudo apt remove %s", (cmd + 4)); // UBUNTU or DEBIAN
	    } else if (strncmp(endi, "gentoo", 6) == 0) {
		sprintf(incm, "sudo emerge --deselect %s", (cmd + 4)); // GENTOO
	    } else if (strncmp(endi, "fedora", 6) == 0) {
		sprintf(incm, "sudo dnf remove %s", (cmd + 4)); // FEDORA
	    } else {
	       	continue;
	    }
	    system(incm);
	    if (strncmp(endi, "gentoo", 6) == 0) system("emerge --depclean");

	// command ls
	} else if (strncmp(cmd, "ls", 2) == 0) {
	    pid_t pid = fork();
	    if (pid == 0) {
		li = 0;
		lsar[li] = strtok(cmd, " ");
		while (lsar[li] != NULL) { li++; lsar[li] = strtok(NULL, " "); }
		execvp(lsar[0], lsar);
	    } else {
		wait(NULL);
	    }

	// command clear or pwd
	} else if (strncmp(cmd, "clear", 5) == 0 || strncmp(cmd, "cls", 3) == 0) {
		printf("\033[H\033[J");

	// command mkdir
	} else if (strncmp(cmd, "mkdir", 5) == 0 || strncmp(cmd, "md", 2) == 0) {
		pid_t pid = fork();
		if (pid == 0) {
			li = 0;
			mdar[li] = strtok(cmd, " ");
			while (mdar[li] != NULL) { li++; mdar[li] = strtok(NULL, " "); }
			mdar[0] = "mkdir";
			execvp(mdar[0], mdar);
		} else {
			wait(NULL);
		}

	// command export
	} else if (strncmp(cmd, "export", 6) == 0 || strncmp(cmd, "ex", 2) == 0) {
		char *expo;
		if (strncmp(cmd, "export", 6) == 0) {
			expo = cmd + 7;
		} else {
			expo = cmd + 3;
		}
		char *fuex = strchr(expo, '=');
		if (!fuex) {
			printf("Usage: export var=value");
		} else {
			*fuex = '\0';
			char *nm = expo;
			char *vl = fuex + 1;
			if (setenv(nm, vl, 1) != 0) perror("setenv");
		}

	// other commands are redirected by the OS
	} else {
	    system(cmd);
	}

	// clear memory
	free(cmd);
    }
    printf("\n");
    return 0;
}
