--Calcolare la somma totale degli assist e la media dei punti realizzati 
--raggrupando per ruolo dei giocatori (guardie, ali, pivot).

SELECT SUM(P.assist) AS assist_totali, AVG(P.pt_segnati), G.ruolo
FROM Giocatore G JOIN Prestazione P ON G.id=P.id_giocatore
GROUP BY G.ruolo
