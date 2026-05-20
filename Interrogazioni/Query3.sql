--Calcolare la media complessiva dei punti segnati in ogni singola arena.

SELECT A.citta, A.nomearena, SUM(Pr.pt_segnati)/COUNT(DISTINCT Pa.idpartita) AS media_punti_partita
FROM ARENA A JOIN PARTITA Pa ON A.nomearena = Pa.nome_arena
JOIN PRESTAZIONE Pr ON Pa.idpartita = Pr.id_partita
GROUP BY A.nomearena
ORDER BY media_punti_partita DESC;