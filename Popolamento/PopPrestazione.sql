INSERT INTO PRESTAZIONE (ID_Giocatore, ID_Partita, Pt_Segnati, Assist, Min_Giocati, Falli)
SELECT 
    g.ID AS ID_Giocatore,
    p.IDPartita AS ID_Partita,
    
    -- Calcola i punti basandosi sulla media reale (PPG) +/- 7 punti di variazione casuale
    -- GREATEST(0, ...) assicura che nessuno faccia punti negativi
    GREATEST(0, CAST(g.PPG + (random() * 14 - 7) AS INT)) AS Pt_Segnati,
    
    -- Genera un numero di assist casuale tra 0 e 10
    CAST(random() * 11 AS INT) AS Assist,
    
    -- Genera i minuti giocati casuali tra 10 e 40 
    CAST(random() * 31 + 10 AS INT) AS Min_Giocati,
    
    -- Genera i falli casuali tra 0 e 5 
    CAST(random() * 6 AS INT) AS Falli

FROM PARTITA p
JOIN GIOCATORE g ON g.Nome_Squadra = p.SqCasa OR g.Nome_Squadra = p.SqOspite;