typedef struct Fila Fila;
typedef struct Processo Processo;
enum status {NOVO, PRONTO};

Processo* BuscaProcessoNome(Fila* f, char* buscado);
Processo* BuscaProcessoTI(Fila* f, int buscado);

Processo* CopiaProcesso(Processo* p);
Processo* CriaProcesso(int ti, int d, char* programa, Processo* dependencia);
Fila* CriaFila();

void printa_fila(Fila* f, char* nome);
void InsereFinal(Fila* f, Processo* p);
void InsereProcesso(Fila* f, Processo* p);
void InsereApos(Fila* f, Processo* p, Processo* novo);
void ArrumaFila(Fila* f1, Fila* f2);
int checa_string(char* item);




