--Elencare i nomi delle squadre che, sommando i falli commessi da tutti i loro giocatori nelle partite, 
--hanno superato i 150 falli di squadra totali

SELECT S.nome_squadra, SUM(P.falli)
FROM Squadra S JOIN Giocatore G ON S.nome_squadra=G.nome_squadra
JOIN Prestazione P ON G.ID=P.id_giocatore
GROUP BY S.nome_squadra
HAVING SUM(P.falli)>150