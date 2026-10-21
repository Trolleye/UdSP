
#ifndef CH_ERR_H
#define CH_ERR_H

typedef enum ch_err {
    CH_OK = 0,        /* operacia prebehla uspesne       */
    CH_ERR_ARG,       /* neplatny argument               */
    CH_ERR_MEM,       /* nedostatok pamate               */
    CH_ERR_RANGE,     /* hodnota mimo povoleneho rozsahu */
    CH_ERR_FORMAT,    /* vstup nema ocakavany tvar       */
    CH_ERR_IO,        /* chyba vstupu alebo vystupu      */
    CH_ERR_NOT_IMPLEMENTED /* telo funkcie este nie je napisane (kostra) */
} ch_err;

const char *ch_err_str(ch_err err);

#endif /* CH_ERR_H */
