#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h> 

// Definisco i parametri di connessione come da lab6.pdf
#define PG_HOST "localhost" 
#define PG_USER "postgres"  
#define PG_DB "ProgettoNBA" 
#define PG_PASS "admin"     
#define PG_PORT 5432

// Funzione di supporto per uscire in caso di errore
void do_exit(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

// Funzione generica per stampare i risultati formattati
void stampa_risultato(PGresult *res) {
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Nessun dato restituito o errore nella query.\n");
        return;
    }
    
    int numTuple = PQntuples(res);
    int numAttributi = PQnfields(res);
    
    printf("\n");
    for (int i = 0; i < numAttributi; i++) {
        fprintf(stdout, "%-25s", PQfname(res, i));
    }
    fprintf(stdout, "\n");
    
    for (int i = 0; i < numAttributi; i++) {
        fprintf(stdout, "-------------------------");
    }
    fprintf(stdout, "\n");
    
    for (int i = 0; i < numTuple; i++) {
        for (int j = 0; j < numAttributi; j++) {
            fprintf(stdout, "%-25s", PQgetvalue(res, i, j));
        }
        fprintf(stdout, "\n");
    }
    printf("\n(%d righe restituite)\n", numTuple);
}

int main() {
    // 1. Creo la stringa di connessione tramite sprintf
    char conninfo[250];
    sprintf(conninfo, "user=%s password=%s dbname=%s host=%s port=%d ",
            PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORT);

    // 2. Eseguo la connessione al database
    PGconn *conn;
    conn = PQconnectdb(conninfo);

    // 3. Verifico lo stato di connessione
    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Errore di connessione: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    printf("Connessione al database stabilita con successo!\n");
    
    int scelta = -1;
    char buffer[100];

    // 4. Inizio del ciclo per il menu interattivo
    while (1) {
        printf("\n================ MENU PROGETTO BASI DI DATI ================\n");
        printf("1. I trascinatori della lega (PPG > 20, Min > 25)\n");
        printf("2. Le franchigie piu' indisciplinate (> 150 falli)\n");
        printf("3. Le arene piu' 'calde' (Media punti)\n");
        printf("4. Efficienza per ruolo (Assist e Media punti)\n");
        printf("5. Eroi in trasferta (Punti massimi fuori casa)\n");
        printf("0. Esci\n");
        printf("============================================================\n");
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

        // 5. Esecuzione delle query del progetto
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
                    "SELECT S.nome_squadra, SUM(P.falli) AS Falli_Totali "
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
                    "GROUP BY A.citta, A.nomearena "
                    "ORDER BY media_punti_partita DESC;"
                );
                break;
            case 4:
                printf("\n--- ESECUZIONE QUERY 4 ---\n");
                res = PQexec(conn,
                    "SELECT G.ruolo, SUM(P.assist) AS assist_totali, AVG(P.pt_segnati) AS media_punti_realizzati "
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
                    "LIMIT 1;"
                );
                break;
            default:
                printf("\nScelta non valida. Riprova.\n");
                continue;
        }

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            fprintf(stderr, "Errore durante l'esecuzione della query: %s\n", PQerrorMessage(conn));
        } else {
            stampa_risultato(res);
        }
        PQclear(res);
    }

    PQfinish(conn);
    return 0;
}