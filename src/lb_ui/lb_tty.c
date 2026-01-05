#include "lb_tty.h"

static int curi;
static int ncol;
static int nrow;
static int linesize;
static FILE * fin;
static FILE * fout;
static struct lb_string * strin;
static struct lb_string * strout;

int init_tty (int * numcol, int * numrow, int * maxlinelen) {
        ncol = *numcol;
        nrow = *numrow;
        linesize = *maxlinelen;
        curi = 0;
        fin = stdin;
        fout = stdout;
        strin = init_ntstring("");
        strout = init_ntstring("");
        // TODO: set canonical, echo
        return 0;
};

int draw_tty () {
        if (fputs(strout->ntstring, fout) == EOF) return -1;
        return 0;
};

int put_tty_out (char * source) {
        free_string(strout);
        strout = init_ntstring(source);
        if (draw_tty() != 0) return -1;
        return 0;
};

int get_tty_in (char * dest);
int release_tty ();

// struct lb_stream {
//         struct lb_string * string;
//         FILE * file;
//         int maxsize;
// };

// struct lb_stream * init_stream (FILE * file, int * maxsize) {
//         struct lb_stream * stream = malloc(sizeof(struct lb_stream));
//         if (stream == NULL) {
//                 errno = ENOMEM;
//                 return NULL;
//         };
//         stream->file = file;
//         stream->string = init_ntstring("");
//         if (stream->string == NULL) {
//                 errno = ENOMEM;
//                 return NULL;
//         };
//         stream->maxsize = *maxsize;
//         return stream;
// };
//
// int free_stream (struct lb_stream ** stream) {
//         if (free_string(&((*stream)->string)) != 0) return -1;
//         free(*stream);
//         *stream = NULL;
//         return 0;
// };
//
// struct lb_tty * init_tty (FILE * fin, int * lin, FILE * fout, int * lout, int * nrow, int * ncol) {
//         struct lb_tty * tty = malloc(sizeof(struct lb_tty));
//         tty->ins = init_stream(fin, lin);
//         tty->outs = init_stream(fout, lout);
//         tty->ins->maxsize = *lin;
//         tty->outs->maxsize = *lout;
//         tty->curi = 0;
//         tty->nrow = *nrow;
//         tty->ncol = *ncol;
//         return tty;
// };
//
// int free_tty (struct lb_tty ** tty) {
//         bool tripped = false;
//         if (free_stream(&((*tty)->ins)) != 0) tripped = true;
//         if (free_stream(&((*tty)->outs)) != 0) tripped = true;
//         free(*tty);
//         *tty = NULL;
//         return tripped ? -1 : 0;
// };
//
// int put_to_tty_out (struct lb_tty * tty, struct lb_string * string) {
//         if (put_in_string(tty->outs->string, -1, string) != 0) return -1;
//         fputs(string->ntstring, tty->outs->file);
//         return 0;
// };
//
// int put_to_tty_in (struct lb_tty * tty, struct lb_string * string) {
//         if (put_in_string(tty->ins->string, tty->curi, string) != 0) return -1;
//         tty->curi += string->ntsize - 1;
//         return 0;
// };
//
// int take_from_tty_in (struct lb_tty * tty, int count) {
//         if (count == 0) return 0;
//         if (count < 0) count = tty->curi;
//         if (tty->curi < count) {
//                 errno = ERANGE;
//                 return -1;
//         };
//         tty->curi -= count;
//         if (take_from_string(tty->ins->string, tty->curi, count) != 0) return -1;
//         return 0;
// };
//
// int get_tty_in_line (struct lb_tty * tty) {
//         int len = tty->ins->maxsize;
//         char * tbuf = calloc(len, sizeof(char));
//         if (tbuf == NULL) {
//                 errno = ENOMEM;
//                 return -1;
//         };
//         int ti = 0;
//         if (clear_tty_in(tty) != 0) return -1;
//         char c;
//         struct lb_string * instr;
//         struct lb_string * outstr;
//         while (ti < len && feof(tty->ins->file) == 0) {
//                 c = fgetc(tty->ins->file);
//                 if (c == EOF || ferror(tty->ins->file) != 0) {
//                         errno = EIO;
//                         return -1;
//                 };
//                 if (c == LBF_RETURN || c == LBF_NEWLINE) {
//                         if (ti > 0) {
//                                 instr = init_utstring(tbuf, ti);
//                                 if (put_to_tty_in(tty, instr) != 0) return -1;
//                                 if (free_string(&instr) != 0) return -1;
//                         };
//                         break;
//                 };
//                 outstr = init_ntstring(LBF_RESET);
//                 // put_in_string(outstr, -1, tty->outs->string);
//                 put_in_string(outstr, -1, tty->ins->string);
//                 pis_move_cur(outstr, tty->curi + 1 + 4, tty->nrow);
//                 fputs(outstr->ntstring, tty->outs->file);
//                 free_string(&outstr);
//                 tbuf[ti] = c;
//                 ti++;
//         };
//         return 0;
// };
//
// int clear_tty_in (struct lb_tty * tty) {
//         struct lb_string * clear = init_ntstring(LBF_RESET);
//         if (put_to_tty_in(tty, clear) == -1) return -1;
//         if (free_string(&clear) == -1) return -1;
//         if (clear_string(tty->ins->string) == -1) return -1;
//         tty->curi = 0;
//         return 0;
// };
//
// int clear_tty_out (struct lb_tty * tty) {
//         struct lb_string * clear = init_ntstring(LBF_RESET);
//         if (put_to_tty_out(tty, clear) == -1) return -1;
//         if (free_string(&clear) == -1) return -1;
//         if (clear_string(tty->outs->string) == -1) return -1;
//         return 0;
// };
