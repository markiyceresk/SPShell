#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
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
char *fullcmd;
char *cmd[512];
char *writeto[512];
char hostname[HOST_NAME_MAX];
short rnew;
char usin[128];
struct termios oldt, newt;
char *lsar[128];
char *mdar[128];
char *rear[512];
char *inar[128];
int cyclenum;

int i;
char s[256];

struct exec {
	char *all[256];
	char *mkdir[128];
} exec;

struct cur {
	int row;
	int col;
} cur;

struct pid {
	pid_t a;
	pid_t b;
	pid_t c;
	pid_t d;
} pid;

struct dist {
	char string[64];
	char *name;
	FILE *file;
} dist;

struct user {
	char name[64];
	char dir[70];
} user;

void getuser() {
	if (geteuid() == 0) {
		strcpy(user.name, "root");
		strcpy(user.dir, "/root");
	} else {
		strcpy(user.name, getenv("USER"));
		sprintf(user.dir, "/home/%s", user.name);
	}
}

void package(char *packages) {
	pid.a = fork();
	if (pid.a == 0) {
		char *exec[256];
		if (strncmp(dist.name, "arch", 4) == 0 || strncmp(dist.name + 5, "arch", 4) == 0) { // ARCH BASED
			if (strncmp(cmd[cyclenum], "inst", 4) == 0) { char *exec[] = {"sudo", "pacman", "-S", packages, NULL}; }
			else if (strcmp(cmd[cyclenum], "ua") == 0) { char *exec[] = {"sudo", "pacman", "-Syu", NULL}; }
			else if (strncmp(cmd[cyclenum], "rem", 3) == 0) { char *exec[] = {"sudo", "pacman", "-R", packages, NULL}; }
    	} else if (strncmp(dist.name + 5, "debian", 6) == 0 || strncmp(dist.name, "debian", 6) == 0) { // DEBIAN BASED
			if (strncmp(cmd[cyclenum], "inst", 4) == 0) { char *exec[] = {"sudo", "apt", "install", packages, NULL}; }
			else if (strcmp(cmd[cyclenum], "ua") == 0) { char *exec[] = {"sudo", "apt", "update", packages, NULL}; }
			else if (strncmp(cmd[cyclenum], "rem", 3) == 0) { char *exec[] = {"sudo", "apt", "remove", NULL}; }
    	} else if (strncmp(dist.name, "gentoo", 6) == 0) { // GENTOO
			if (strncmp(cmd[cyclenum], "inst", 4) == 0) { char *exec[] = {"sudo", "emerge", "--ask", packages, NULL}; }
			else if (strcmp(cmd[cyclenum], "ua") == 0) { char *exec[] = {"sudo", "emerge", "--sync", "&&", "emerge", "avuDN", "@world", NULL}; }
			else if (strncmp(cmd[cyclenum], "rem", 3) == 0) { char *exec[] = {"sudo", "emerge", "--deselect", packages, NULL}; }
    	} else if (strncmp(dist.name, "fedora", 6) == 0) { // FEDORA
			if (strncmp(cmd[cyclenum], "inst", 4) == 0) { char *exec[] = {"sudo", "dnf", "install", packages, NULL}; }
			else if (strcmp(cmd[cyclenum], "ua") == 0) { char *exec[] = {"sudo", "dnf", "update", "--refresh", NULL}; }
			else if (strncmp(cmd[cyclenum], "rem", 3) == 0) { char *exec[] = {"sudo", "dnf", "remove", packages, NULL}; }
    	} else {
			printf("Your system isn't supported");
		}
		execvp(exec[0], exec);
		perror("execvp failed");
		return;
	} else {
		wait(NULL);
	}
}

void getdist() {  // write current directory to variable dist.name
	dist.file = fopen("/etc/os-release", "r");
	if (dist.file) {
		while (fgets(dist.string, sizeof(dist.string), dist.file)) {
			if (strncmp(dist.string, "ID=", 3) == 0) {
				dist.name = dist.string + 3;
				break;
			}
		}
	} else {
		dist.name = "none";
	}
	fclose(dist.file);
}

void getlogo() {  // print ASCII art
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
}

void greeting() {  // print greeting
	getlogo();
	printf("\n!!! НЕТ ВОЙНЕ !!!\n\nHi, %s!\nWhat would you do?\n\n", user.name);
}


int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	getuser();
	getdist();
	greeting();

	// =============================== MAIN CYCLE ===============================

    while (1) {

		// ----- GREETING -----

		// print greeting
		gethostname(hostname, HOST_NAME_MAX);
		getcwd(cwd, sizeof(cwd));
		snprintf(prm, sizeof(prm), "%s | %s | %s $ ", user.name, hostname, cwd);

		// input command
		fullcmd = readline(prm);
		if (!fullcmd) break;
		if (*cwd) {
		    add_history(fullcmd);
		}

		i = 0;
		cmd[i] = strtok(fullcmd, ";");
		while (cmd[i] != NULL) { i++; cmd[i] = strtok(NULL, "="); }

		for (cyclenum = 0; cmd[cyclenum] != NULL; cyclenum++) {

			if (strchr(cmd[cyclenum], '>')) {
				i = 0;
				int fd;
				writeto[i] = strtok(cmd[cyclenum], ">");
				while (writeto[i] != NULL) { i++; writeto[i] = strtok(NULL, " "); }
        		fd = open(writeto[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        		dup2(fd, STDOUT_FILENO); 
        		close(fd);
				cmd[cyclenum] = writeto[0];
			} else if (strstr(cmd[cyclenum], ">>")) {
				int fd;
				i = 0;
				writeto[i] = strtok(cmd[cyclenum], ">");
				while (writeto[i] != NULL) { i++; writeto[i] = strtok(NULL, " "); }
        		if (fd = open(writeto[1], O_WRONLY | O_CREAT | O_APPEND, 0644)) { perror(""); }
        		dup2(fd, STDOUT_FILENO); 
        		close(fd);
				cmd[cyclenum] = writeto[0];
			}

			// ----- COMMANDS -----

			// command q (quit)
			if (strcmp(cmd[cyclenum], "q") == 0 || strcmp(cmd[cyclenum], "exit") == 0) {
			    free(cmd[cyclenum]);
			    break;

			// command cd (change directory)
			} else if (strncmp(cmd[cyclenum], "cd", 2) == 0) {
				if (strncmp(cmd[cyclenum] + 3, "~", 1) == 0) {
					sprintf(s, "%s%s", user.dir, cmd[cyclenum] + 4);
					if (chdir(s) != 0) perror("chdir");
				} else if (strncmp(cmd[cyclenum] + 2, " ", 1)) {
					if (chdir(user.dir) != 0) perror("chdir");
				} else {
					if (chdir(cmd[cyclenum] + 3) != 0) perror("chdir");
				}

			// command help (have -s flag)
			} else if (strncmp(cmd[cyclenum], "help", 4) == 0) {
			    if (strncmp(cmd[cyclenum] + 5, "-s", 2) == 0) {
					printf("sus, hi, potato, news, I am not a moron!");
			    } else {
					printf("q - quit\ncd <arguments> - change directory\ninst <arguments> - install packet from packet manager (Only Arch based, Gentoo, Debian based, and Fedora are supported)\nua - full system update (Only the previously listed distributions are supported)\nrps - rock paper scissors\nhelp - use '-s' flag to see references :)");
			    }

			// command inst (install)
			} else if (strncmp(cmd[cyclenum], "inst ", 5) == 0) {
				package(cmd[cyclenum] + 5);

			// command ua (update all)
			} else if (strcmp(cmd[cyclenum], "ua") == 0) {
				package(NULL);

			// command del (delete)
			} else if (strncmp(cmd[cyclenum], "rem ", 3) == 0) {
	    	    package(cmd[cyclenum] + 4);

			// cls (made for windows users)
			} else if (strcmp(cmd[cyclenum], "cls") == 0) {
				printf("\033[H\033[J");

			// mkdir
			} else if (strncmp(cmd[cyclenum], "md ", 3) == 0) {
				pid.a = fork();
				if (pid.a == 0) {
					i = 0;
					exec.mkdir[i] = strtok(cmd[cyclenum], " ");
					while (exec.mkdir[i] != NULL) { i++; exec.mkdir[i] = strtok(NULL, " "); }
					exec.mkdir[0] = "mkdir";
					execvp(exec.mkdir[0], exec.mkdir);
					perror("execvp failed");
				} else {
					wait(NULL);
				}

			// export
			} else if (strncmp(cmd[cyclenum], "export ", 7) == 0 || strncmp(cmd[cyclenum], "ex ", 3) == 0) {
				struct export { char *strall; char *all[256]; } export;
				if (strncmp(cmd[cyclenum], "export", 6) == 0) {
					export.strall = cmd[cyclenum] + 7;
				} else {
					export.strall = cmd[cyclenum] + 3;
				}
				i = 0;
				export.all[i] = strtok(export.strall, "=");
				export.all[1] = strtok(NULL, "\n");
				if (setenv(export.all[0], export.all[1], 1) != 0) { perror("setenv"); }

			// ----- GAMES -----

			// command rps (rock paper scissors)
			} else if (strcmp(cmd[cyclenum], "rps") == 0) {
				struct choose {
					short player;
					short computer;
					int score;
				} choose;
		    	struct termios t;
				char b;
		    	tcgetattr(0, &t);
		    	struct termios n = t;
		    	n.c_lflag &= ~(ICANON | ECHO);
		    	tcsetattr(0, TCSANOW, &n);
		    	srand(time(0));
				while (printf("1. Rock\n2. Paper\n3. Scissors\n4. Quit\n") && read(0, &choose.player, 1) && choose.player != '4') {
					choose.player -= '0';
					choose.computer = rand() % 3 + 1;
					printf("\nComputer: ");
					if (choose.computer == 1) { printf("rock"); }
					else if (choose.computer == 2) { printf("paper"); }
					else if (choose.computer == 3) { printf("scissors"); }
					printf(" | ");
					if (choose.player == choose.computer) printf("Draw");
					else if ((choose.player % 3) + 1 == choose.computer) { printf("You lost"); choose.score--; }
					else { printf("You won"); choose.score++; }
					printf(" | Score: %d\n\n", choose.score);
		    	}
		    	tcsetattr(0, TCSANOW, &t);

			// ---- JUST FUNNY :) -----

			// command news
			} else if (strcmp(cmd[cyclenum], "news") == 0) {
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

			// command sus
			} else if (strcmp(cmd[cyclenum], "sus") == 0) {
				printf("This looks kinda SUS...");

			// command hi
			} else if (strcmp(cmd[cyclenum], "hi") == 0) {
				printf("Hello!");

			// command I am not a moron (reference to portal 2)
			} else if (strcmp(cmd[cyclenum], "I am not a moron!") == 0) {
		    	printf("Yes, you are!!!\n");
		    	sleep(1);
		    	printf("You are the moron, they built to make me an idiot!");

			// command potato (reference to PWGood)
			} else if (strcmp(cmd[cyclenum], "potato") == 0) {
		    	printf("Картошка!\n");
		    	sleep(1);
		    	printf("картошка\nкартошка\nкартошка!");

			// other commands are redirected by the OS
			} else {
		    	pid.a = fork();
				if (pid.a == 0) {
					i = 0;
					exec.all[i] = strtok(cmd[cyclenum], " ");
					while (exec.all[i] != NULL) { i++; exec.all[i] = strtok(NULL, " "); }
					for (i = 0; exec.all[i] != NULL; i++) { if (exec.all[i][0] == '$') { char *val = getenv(exec.all[i] + 1); if (val) exec.all[i] = val; } }
					execvp(exec.all[0], exec.all);
					perror("");
					break;
				} else {
					wait(NULL);
				}
			}
			tcgetattr(STDIN_FILENO, &oldt);
	    	newt = oldt;
		    newt.c_lflag &= ~(ICANON | ECHO);
		    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		    write(STDOUT_FILENO, "\033[6n", 4);
			scanf("\033[%d;%dR", &cur.row, &cur.col);
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		    if (cur.col > 1) printf("\n");
		}
	}

    printf("\n");
    return 0;
}
