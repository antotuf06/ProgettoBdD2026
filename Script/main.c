#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h> 

// Definire i parametri di connessione
#define PG_HOST "localhost" 
#define PG_USER "postgres"  
#define PG_DB "progetto_nba" 
#define PG_PASS "admin"     
#define PG_PORT 5432

// Funzione per stampare i risultati delle query
void stampa_risultato(PGresult *res) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Nessun dato restituito o errore nella query.\n");
        return;
    }
    
    int numTuple = PQntuples(res);
    int numAttributi = PQnfields(res);
    
    printf("\n");
    for (int i = 0; i < numAttributi; i++) {
        printf("%s\t\t", PQfname(res, i));
    }
    printf("\n");
    
    for (int i = 0; i < numAttributi; i++) {
        printf("-------------------------");
    }
    printf("\n");
    
    for (int i = 0; i < numTuple; i++) {
        for (int j = 0; j < numAttributi; j++) {
            printf("%s\t\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }
    printf("\n(%d righe restituite)\n", numTuple);
}

int main() {
    char conninfo[250];
    sprintf(conninfo, "user=%s password=%s dbname=%s host=%s port=%d ",
            PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORT);

    // Connessione al DB
    PGconn *conn;
    conn = PQconnectdb(conninfo);

    // Verificare la connessione
    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Errore di connessione: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    printf("Connessione avvenuta correttamente\n");
    
    int scelta = -1;
    char buffer[100];

    while (1) {
        printf("\n================ QUERY ESEGUIBILI ================\n");
        printf("1. Giocatori con almeno 20 punti di media e 25 minuti giocati di media\n");
        printf("2. Le franchigie con piu' di 150 falli totali\n");
        printf("3. Le arene dove vengono segnati piu' punti\n");
        printf("4. Numero di assist e media punti per ogni ruolo\n");
        printf("5. Giocatori con piu' punti totali nelle partite fuori casa\n");
        printf("0. Esci\n");
        printf("====================================================\n");
        printf("Seleziona una query da eseguire: ");

        if (scanf("%d", &scelta) != 1) {
            scanf("%s", buffer);
            continue;
        }

        if (scelta == 0) {
            printf("Chiusura del programma...\n");
            break;
        }

        PGresult *res = NULL;

        // Esecuzione delle query
        switch (scelta) {
            case 1:
                printf("\n--- ESECUZIONE QUERY 1 ---\n");
                res = PQexec(conn,
                    "SELECT T.Nome, T.Cognome, T.ID, G.PPG "
                    "FROM Tesserato T JOIN Giocatore G ON T.ID=G.ID "
                    "JOIN Prestazione P ON T.ID=P.id_giocatore "
                    "GROUP BY T.Nome, T.Cognome, T.ID, G.PPG "
                    "HAVING G.PPG>20.0 AND AVG(P.min_giocati)>25;"
                );
                break;
            case 2:
                printf("\n--- ESECUZIONE QUERY 2 ---\n");
                res = PQexec(conn,
                    "SELECT S.nome_squadra, SUM(P.falli)"
                    "FROM Squadra S JOIN Giocatore G ON S.nome_squadra=G.nome_squadra "
                    "JOIN Prestazione P ON G.ID=P.id_giocatore "
                    "GROUP BY S.nome_squadra "
                    "HAVING SUM(P.falli)>150;"
                );
                break;
            case 3:
                printf("\n--- ESECUZIONE QUERY 3 ---\n");
                res = PQexec(conn,
                    "SELECT A.citta, A.nomearena, SUM(Pr.pt_segnati)/COUNT(DISTINCT Pa.idpartita) AS media_punti_partita "
                    "FROM ARENA A JOIN PARTITA Pa ON A.nomearena = Pa.nome_arena "
                    "JOIN PRESTAZIONE Pr ON Pa.idpartita = Pr.id_partita "
                    "GROUP BY A.nomearena "
                    "ORDER BY media_punti_partita DESC;"
                );
                break;
            case 4:
                printf("\n--- ESECUZIONE QUERY 4 ---\n");
                res = PQexec(conn,
                    "SELECT SUM(P.assist) AS assist_totali, AVG(P.pt_segnati) AS media_punti_realizzati, G.ruolo"
                    "FROM Giocatore G JOIN Prestazione P ON G.id=P.id_giocatore "
                    "GROUP BY G.ruolo;"
                );
                break;
            case 5:
                printf("\n--- ESECUZIONE QUERY 5 ---\n");
                res = PQexec(conn,
                    "SELECT T.Nome, T.Cognome, SUM(Pr.Pt_Segnati) AS punti_trasferta "
                    "FROM Tesserato T JOIN Giocatore G ON T.id = G.id "
                    "JOIN Prestazione Pr ON T.id = Pr.id_giocatore "
                    "JOIN Partita Pa ON Pa.idpartita = Pr.id_partita "
                    "WHERE G.Nome_Squadra = Pa.SqOspite "
                    "GROUP BY T.nome, T.cognome "
                    "ORDER BY punti_trasferta DESC "
                );
                break;
            default:
                printf("\nScelta non valida.\n");
                continue;
        }

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore durante l'esecuzione della query: %s\n", PQerrorMessage(conn));
        } else {
            stampa_risultato(res);
        }
        PQclear(res);
    }

    PQfinish(conn);
    return 0;
}