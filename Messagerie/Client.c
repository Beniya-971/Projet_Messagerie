#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<time.h>
#include<signal.h>

//Il faut exécuter avec:
//./Client ip port

// Pour fils communique avec serveur ( tuples)
/*
pipe (int tab[2])
close(tab[0]) dans celui qui veut écrire
close (tab[1]) dans celui qui veut lire
write(tab,&buffer,size)
read(tab,&buffer,size)
*/

// Pour communiquer avec le fils  créer par le serveur (socket)

