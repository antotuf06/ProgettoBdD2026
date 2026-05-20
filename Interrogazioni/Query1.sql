--Trovare nome e cognome dei giocatori che, analizzando le loro prestazioni nelle partite disputate, 
--mantengono una media effettiva superiore a 20 punti a referto e giocano in media più di 25 minuti a partita.

SELECT T.Nome, T.Cognome, T.ID, G.PPG
FROM Tesserato T JOIN Giocatore G ON T.ID=G.ID
JOIN Prestazione P ON T.ID=P.id_giocatore
GROUP BY T.Nome, T.Cognome, T.ID, G.PPG
HAVING G.PPG>20.0 AND AVG(P.min_giocati)>25