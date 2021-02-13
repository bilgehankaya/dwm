
/* #include <stdio.h> */
/* #include <stdlib.h> */

char* getterm(){
    char* term = getenv("TERMINAL");
    if(!term)
        term = "alacritty";
    return term;
}

/* int main(int argc, char *argv[]) */
/* { */
/*     const char *term = getterm(); */
/*     printf("Terminal is : %s", term); */
/* } */
