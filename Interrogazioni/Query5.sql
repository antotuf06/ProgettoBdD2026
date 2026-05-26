--Trovare i giocatori (nome e cognome) che hanno segnato in assoluto più punti totali 
--sommando esclusivamente le prestazioni ottenute nelle partite in cui 
--le loro squadra figuravano come squadra ospite.

SELECT T.Nome, T.Cognome, SUM(Pr.Pt_Segnati) AS punti_trasferta
FROM Tesserato T JOIN Giocatore G ON T.id = G.id
JOIN Prestazione Pr ON T.id = Pr.id_giocatore
JOIN Partita Pa ON Pa.idpartita = Pr.id_partita
WHERE G.Nome_Squadra = Pa.SqOspite
GROUP BY T.nome, T.cognome
ORDER BY punti_trasferta DESC
